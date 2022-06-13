#ifndef DEBUG_DEVICE_H
#define DEBUG_DEVICE_H

#include <linux/device.h> 
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO

#define DEVICE_NAME "test"                  // 设备名称
#define MSG_BUFFER_LEN 500000                 // 设备内容的最大长度

/*init a device*/
void device_init(void);

/*release a device*/
void device_release(void);

/*When a process in user space open the registed device, it will recall this function*/
int device_open(struct inode *node, struct file *file);

/* When a process in user space read the registed device, it will recall this function
 * @buf: a virtual address in user space
 * @len: length of bytes
 */
ssize_t device_read(struct file *file,char *buf, size_t len,loff_t *offset);

/*When a process in user space write the registed device, it will recall this function*/
ssize_t device_write(struct file *file,const char *buf, size_t len,loff_t* offset);

/*When a process in user space close the registed device, it will recall this function*/
int device_close(struct inode *node,struct file *file);

#endif