#ifndef BRANCH_PREDICTOR_H
#define BRANCH_PREDICTOR_H

#include "register.h"

#define PREDICTOR_SIZE 1024

/* initial the branch predictor when the kernel module is insmoded */
void branch_predictor_init(void);

/* when given an address (pc) of a branch instruction, return the predicted target address */
va_t get_from_branch_predictor(reg64_t pc);

/* after an execution of a branch instrucion, the debugger will call this function to update the content of the branch predictor */
void branch_predictor_update(reg64_t pc, va_t target);

/* release the kernel space that had been allocated in function branch_predictor_init() when rmmod the kernel module*/
void branch_predictor_release(void);

#endif