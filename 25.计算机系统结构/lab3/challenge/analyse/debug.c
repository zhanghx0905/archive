#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define device_name "/dev/test"
#define MSG_BUFFER_LEN 3000000 // 设备内容的最大长度

typedef struct {
    uint32_t set;
    uint32_t way;
    uint32_t offset;
    uint64_t tag;
    uint32_t valid;
    uint64_t data0;
    uint64_t data1;
} msg_t;

msg_t msg;

uint64_t _strtoul(char* str)
{
    uint64_t res = 0;
    for (int i = 0; i < 8; ++i) {
        res += (uint64_t)str[i] << (i << 3);
    }
    return res;
}

int main()
{
    int fd = open(device_name, O_RDONLY);
    if (fd < 0) {
        printf("cannot open the file\n");
        return -1;
    }
    char buf[MSG_BUFFER_LEN];
    read(fd, buf, sizeof(uint64_t));
    uint32_t cnt = 128 * 8 * 4;
    read(fd, buf, 56 * cnt);
    printf("set, way, offset, valid, tag, data\n");
    for (uint32_t i = 0; i < cnt; ++i) {
        uint32_t offset = 56 * i;
        msg.set = (uint32_t)_strtoul(buf + offset);
        msg.way = (uint32_t)_strtoul(buf + offset + 8);
        msg.offset = (uint32_t)_strtoul(buf + offset + 16);
        msg.valid = (uint32_t)_strtoul(buf + offset + 24);
        msg.tag = (uint64_t)_strtoul(buf + offset + 32);
        msg.data0 = (uint64_t)_strtoul(buf + offset + 40);
        msg.data1 = (uint64_t)_strtoul(buf + offset + 48);
        printf("%d, %d, %d, %d, 0x%lx, 0x%lx, 0x%lx\n", msg.set, msg.way, msg.offset, msg.valid, msg.tag, msg.data0, msg.data1);
    }

    return 0;
}