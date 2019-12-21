#ifndef message_slot_h
#define message_slot_h
#include <linux/ioctl.h>
// The major device number..
// We don't rely on dynamic registration
// any more. We want ioctls to know this
// number at compile time.
#define MAJOR_NUM 240
#define MINOR_NUM
// Set the message of the device driver
//#define IOCTL_MSG_SLOT_CHANNEL _IOW(MAJOR_NUM, 0, unsigned int)//TODO: change the 0 to MINOR_NUM
#define IOCTL_SET_ENC _IOW(MAJOR_NUM, 0, unsigned long)

#define DEVICE_RANGE_NAME "char_dev" // TODO: check this line
#define BUF_LEN 128
#define DEVICE_FILE_NAME "slot" // TODO: check this line
#define SUCCESS 0 // TODO: check this line
#endif /* message_slot_h */
