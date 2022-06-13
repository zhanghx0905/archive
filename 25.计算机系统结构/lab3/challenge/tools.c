#include "tools.h"

void ultostr(char* str, uint64_t val) {
    int i;
    for (i = 0; i < 8; ++ i) {
        str[i] = val & 0xff;
        val = val >> 8;
    }
}


void msg_encode(char* buf, msg_t* msg) {
    ultostr(buf, msg->set);
    ultostr(buf + 8, msg->way);
    ultostr(buf + 16, msg->offset);
    ultostr(buf + 24, msg->valid);
    ultostr(buf + 32, msg->tag);
    ultostr(buf + 40, msg->data0);
    ultostr(buf + 48, msg->data1);
}
