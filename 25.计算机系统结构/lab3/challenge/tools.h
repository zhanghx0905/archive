#ifndef TOOLS_H
#define TOOLS_H

#include <linux/io.h>

typedef struct {
    uint64_t data0;
    uint64_t data1;
    uint32_t set;
    uint32_t way;
    uint64_t tag;
    uint32_t valid;
    uint32_t offset;
}msg_t;
/* Store 64bit value val to an address str
 * @str: the address to contain the value
 * @val: the 64bit value
 */
void ultostr(char* str, uint64_t val);
// void utostr(char* str, uint32_t val);
/* Store a msg_t struct to an address buf
 * @buf: the address to contain the value
 * @msg: the struct msg_t
 */
void msg_encode(char* buf, msg_t* msg);

#endif