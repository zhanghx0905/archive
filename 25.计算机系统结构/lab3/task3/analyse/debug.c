#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#define device_name "/dev/test"
#define MSG_BUFFER_LEN 3000000                 // 设备内容的最大长度

typedef struct  {
    uint64_t pc;
    uint64_t dir;
    uint64_t pre_dir;
    uint64_t tar;
    uint64_t pre_tar;
    uint64_t ins;
}msg_t;

msg_t msg;

uint64_t _strtoul(char* str) {
    uint64_t res = 0;
    for (int i = 0; i < 8; ++ i) {
        res += (uint64_t) str[i] << (i << 3);
    }
    return res;
}

int main () {
    int fd = open(device_name, O_RDONLY);
    if (fd < 0) {
        printf("cannot open the file\n");
        return -1;
    }
    char buf[MSG_BUFFER_LEN];
    read(fd, buf, sizeof(uint64_t));
    int cnt = (int)_strtoul(buf);
    printf("%d\n", cnt);
    read(fd, buf, 48 * cnt + sizeof(uint64_t));
    uint32_t ins;
    uint64_t pc, tar, pre_tar;
    int dir, pre_dir;
    printf("pc, direction, predicted dir, target, prddicted tar, instruction\n");
    for(int i = 0; i < cnt; ++ i) {
        int offset = sizeof(msg_t) * i + 8;
        pc = (uint64_t) _strtoul(buf + offset);
        dir = (int) _strtoul(buf + offset + 8);
        pre_dir = (int) _strtoul(buf + offset + 16);
        tar = _strtoul(buf + offset + 24);
        pre_tar = _strtoul(buf + offset + 32);
        ins = (uint32_t) _strtoul(buf + offset + 40);
        printf("0x%lx, %d, %d, 0x%lx, 0x%lx, 0x%x\n", pc, dir, pre_dir, tar, pre_tar, ins);
    }

    return 0;
}