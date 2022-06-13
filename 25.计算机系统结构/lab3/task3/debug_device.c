#include "debug_device.h"

struct file_operations file_ops = {
   .read = device_read,
   .write = device_write,
   .open = device_open,
   .release = device_close,
};

int major_num;                        // 设备注册相关
int device_open_count = 0;            // 设备打开数量
char *msg_buffer;                     // 用于维护设备内容
char *msg_ptr;                        // 维护设备的读写位置


void device_init(void) {
    /* registe the device */
    major_num = register_chrdev(0, DEVICE_NAME, &file_ops);
    msg_buffer = vmalloc(sizeof(char) * MSG_BUFFER_LEN);
}

void device_release(void) {
    /* unregister the device */
    unregister_chrdev(major_num, DEVICE_NAME);
    vfree(msg_buffer);
}

ssize_t device_read(struct file *flip, char *buffer, size_t len, loff_t *offset) {
    int bytes_read = 0;
    msg_ptr = msg_buffer;
    while (len > 0) {
        put_user(*(msg_ptr ++), buffer ++);  // 复制用户空间的一段内存空间
        len --;
        bytes_read++;
    }
    return bytes_read;
}

ssize_t device_write(struct file *flip, const char *buffer, size_t len, loff_t *offset) {
    if(copy_from_user(msg_ptr, buffer, sizeof(char)) == -1)
        return -EINVAL;
    return 0;
}

int device_open(struct inode *inode, struct file *file) {
    if (device_open_count)
        return -EBUSY;
    device_open_count ++;
    try_module_get(THIS_MODULE);
    return 0;
}

int device_close(struct inode *inode, struct file *file) {
    device_open_count --;
    module_put(THIS_MODULE);
    return 0;
}