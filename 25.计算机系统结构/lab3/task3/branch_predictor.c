#include "branch_predictor.h"
/* ************** */
/* TASK3: 20181011365 2018011363 */
/* ************** */

#define PHT_SIZE 2048
#define GHR_MASK 0x7ff       // (PHT_SIZE - 1)
#define BTB_BASE_OFFSET 512  // PHT_SIZE * 2 / 8
#define BTB_SIZE 128

#define BOOL(x) ((x) ? (1) : (0))
#define SWAP(a, b) \
  a ^= b;          \
  b ^= a;          \
  a ^= b;

#ifdef SIMULATOR
char predictor_space[PREDICTOR_SIZE];
#else
char* predictor_space;
#endif

uint32_t GHR;
char* PHT;  // 2bit per pattern

typedef struct {
  uint16_t pc;
  uint16_t target;
} BTBEntry;  // sizeof(BTBEntry) = 32
BTBEntry* BTB;
char BTB_bitmap[BTB_SIZE / 8];
uint32_t BTB_iter;

/* BITMAP FUNCTION */

void set_bitmap(char* bitset, uint32_t idx) {
  bitset[idx / 8] |= (1 << (idx % 8));
}

void clear_bitmap(char* bitset, uint32_t idx) {
  bitset[idx / 8] &= ~(1 << (idx % 8));
}

uint32_t test_bitmap(char* bitset, uint32_t idx) {  // 返回 0 或 1
  return BOOL(bitset[idx / 8] & (1 << (idx % 8)));
}

/* PHT FUNCTION */

uint32_t jump(void) {  // 高位为 1 则 jump
  return test_bitmap(PHT, (GHR & GHR_MASK) << 1);
}

void update_PHT(uint32_t jump) {
  uint32_t pattern = (GHR & GHR_MASK) << 1,
           history =
               (test_bitmap(PHT, pattern) << 1) + test_bitmap(PHT, pattern + 1);
  if (jump && history < 0b11) {
    history++;
  } else if (!jump && history > 0b00) {
    history--;
  }
  (history & 0b10) ? set_bitmap(PHT, pattern) : clear_bitmap(PHT, pattern);
  (history & 0b01) ? set_bitmap(PHT, pattern + 1)
                   : clear_bitmap(PHT, pattern + 1);
}

/* BTB FUNCTION */

uint16_t search_BTB(uint16_t pc) {
  uint32_t i;
  uint16_t res = 0;
  for (i = 0; i < BTB_SIZE; ++i) {
    if (test_bitmap(BTB_bitmap, i) && pc == BTB[i].pc) {
      res = BTB[i].target;
    }
  }
  return res;
}

void set_BTB(uint16_t pc, uint16_t target) {
  uint32_t i;
  // 先在 BTB 中寻找，如果找到对应 key 直接修改
  for (i = 0; i < BTB_SIZE; ++i) {
    if (test_bitmap(BTB_bitmap, i) && pc == BTB[i].pc) {
      BTB[i].target = target;
      return;
    }
  }
  // 找不到，创建新的 BTB 项
  BTB[BTB_iter].pc = pc;
  BTB[BTB_iter].target = target;
  set_bitmap(BTB_bitmap, BTB_iter);
  BTB_iter = (BTB_iter + 1) % BTB_SIZE;
}

void branch_predictor_init(void) {
  uint32_t i;
#ifndef SIMULATOR
  predictor_space = vmalloc(PREDICTOR_SIZE * sizeof(char));
#endif
  // DONE: other code if necessary
  PHT = predictor_space;
  for (i = 0; i < PHT_SIZE; ++i) {  // 初始化为 weak jump (10)
    set_bitmap(PHT, 2 * i);
    clear_bitmap(PHT, 2 * i + 1);
  }
  BTB = (BTBEntry*)(predictor_space + BTB_BASE_OFFSET);
  for (i = 0; i < BTB_SIZE / 8; ++i) {
    BTB_bitmap[i] = 0;
  }
  BTB_iter = 0;
  GHR = 0;
}

va_t get_from_branch_predictor(reg64_t pc) {
  // DONE: once your branch predictor get an instruction address, it should give
  // a predicted target address
  if (jump()) {
    va_t target = search_BTB((uint16_t)pc);  // just low 16 bit
    if (target != 0)
      return (pc & (~0xffff)) + target;
    else  // 在 BTB 中没有找到
      return pc;
  }
  return pc + 4;  // not jump
}

void branch_predictor_update(reg64_t pc, va_t target) {
  // DONE: the debugger will give the correct target address once a branch
  // instrucion executed, and you need to update the branch predictor
  int jump = (target != pc + 4);
  // 更新 BTB, GHR, PHT
  update_PHT(jump);
  GHR = (GHR << 1) + jump;
  if (jump) {
    set_BTB((uint16_t)pc, (uint16_t)target);
  }
}

void branch_predictor_release(void) {
#ifndef SIMULATOR
  vfree(predictor_space);
#endif
  // DONE: other code if necessary
}