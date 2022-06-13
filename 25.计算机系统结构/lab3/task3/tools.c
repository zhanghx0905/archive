#include "tools.h"

void ultostr(char* str, uint64_t val) {
    int i;
    for (i = 0; i < 8; ++ i) {
        str[i] = val & 0xff;
        val = val >> 8;
    }
}

void msg_encode(char* buf, msg_t* msg) {
    ultostr(buf, msg->pc);
    ultostr(buf + 8, msg->dir);
    ultostr(buf + 16, msg->pre_dir);
    ultostr(buf + 24, msg->tar);
    ultostr(buf + 32, msg->pre_tar);
    ultostr(buf + 40, msg->ins);
}

uint64_t signed_extern(uint64_t input, int highest_pos) {
    int sbit = (input >> highest_pos) & 1;
    if(sbit == 0)  // positive
        return input;
    else {  // nagetive
        uint64_t x = -1;
        return input + (x << (highest_pos + 1));
    }
}