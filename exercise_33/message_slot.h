#ifndef message_slot_h
#define message_slot_h

#include <linux/ioctl.h>

// The major device number..
// We don't rely on dynamic registration
// any more. We want ioctls to know this
// number at compile time.
#define MAJOR_NUM 240

// Set the message of the device driver
#define IOCTL_SET_ENC _IOW(MAJOR_NUM, 0, unsigned long)

#define DEVICE_RANGE_NAME "char_dev" //TODO: check this line
#define BUF_LEN 128
#define DEVICE_FILE_NAME "simple_char_dev" //TODO: check this line
#define SUCCESS 0 //TODO: check this line

#endif /* message_slot_h */
