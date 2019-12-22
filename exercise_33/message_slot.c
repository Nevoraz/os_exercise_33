// Declare what kind of code we want
// from the header files. Defining __KERNEL__
// and MODULE allows us to access kernel-level
// code not usually available to userspace programs.
#undef __KERNEL__
#define __KERNEL__
#undef MODULE
#define MODULE


#include <linux/kernel.h>   /* We're doing kernel work */
#include <linux/module.h>   /* Specifically, a module */
#include <linux/fs.h>       /* for register_chrdev */
#include <linux/uaccess.h>  /* for get_user  and put_user */
#include <linux/string.h>   /* for memset. NOTE - not string.h!*/
MODULE_LICENSE("GPL");
struct node {
    char * data;
    int key; // the key is the channel
    struct node * next;
};
static void insertFirst(int key, char data[]);
static struct node * find(int key);
static struct node * delete(int key);
//Our custom definitions of IOCTL operations
#include "message_slot.h"
struct chardev_info{
    spinlock_t lock;
};
// used to prevent concurent access into the same device
static int dev_open_flag = 0;
static struct chardev_info device_info;
// The message the device will give when asked
static char the_message[BUF_LEN]; // TODO: Note that the message can contain any sequence of bytes, it is not necessarily a C string
static int minor_num = 0;
static long current_channel = 0;// TODO: check if it should be int instead of long
static struct node *head = NULL;
static struct node *current = NULL;
//================== DEVICE FUNCTIONS ===========================
static int device_open( struct inode* inode, struct file*  file ){
    unsigned long flags; // for spinlock
    printk("Invoking device_open(%p)\n", file);
    // We don't want to talk to two processes at the same time
    spin_lock_irqsave(&device_info.lock, flags);
    if( 1 == dev_open_flag ){
        spin_unlock_irqrestore(&device_info.lock, flags);
        return -EBUSY;
    }
    ++dev_open_flag;
    spin_unlock_irqrestore(&device_info.lock, flags);
    return SUCCESS;
}
//---------------------------------------------------------------
static int device_release( struct inode* inode,struct file*  file){
    unsigned long flags; // for spinlock
    printk("Invoking device_release(%p,%p)\n", inode, file);
    // ready for our next caller
    spin_lock_irqsave(&device_info.lock, flags);
    --dev_open_flag;
    spin_unlock_irqrestore(&device_info.lock, flags);
    return SUCCESS;
}
//---------------------------------------------------------------
// a process which has already opened
// the device file attempts to read from it
static ssize_t device_read( struct file* file, char __user* buffer, size_t length, loff_t* offset ){
    printk( "Invocing device_read(%p,%d) - " "the message: %s)\n", file, (int)length, the_message );
    return -EINVAL;
}
//---------------------------------------------------------------
// a processs which has already opened
// the device file attempts to write to it
static ssize_t device_write( struct file* file, const char __user* buffer, size_t length, loff_t* offset){
    int i;
    if (current_channel == 0) {
        
        return -EINVAL;
    }
    printk("Invoking device_write(%p,%d)\n", file, (int)length);
    for( i = 0; i < length && i < BUF_LEN; ++i ){
        get_user(the_message[i], &buffer[i]);
        the_message[i] += 1;
    }
    insertFirst(current_channel, the_message);
    // TODO create new message array and than copy
    // return the number of input characters used
    return i;
    //    TODO: If the passed message length is 0 or more than 128, returns -1 and errno is set to EMSGSIZE.
    //    TODO: In any other error case (for example, failing to allocate memory), returns -1 and errno is set appropriately (you are free to choose the exact value)
}
//----------------------------------------------------------------
static long device_ioctl( struct   file* file,
                         unsigned int   ioctl_command_id,
                         unsigned long  channel_to_set )
{
    if( IOCTL_MSG_SLOT_CHANNEL == ioctl_command_id ){// Get the parameter given to ioctl by the process
        if (channel_to_set == 0)
            return -EINVAL;
        printk( "Invoking ioctl: setting channel to %ld\n", channel_to_set );
        current_channel = channel_to_set;//TODO: validate it sets the file descriptor’s channel id
    }
    else
        return -EINVAL;
    return SUCCESS;
}
//==================== DEVICE SETUP =============================
// This structure will hold the functions to be called
// when a process does something to the device we created
struct file_operations Fops ={
    .read           = device_read,
    .write          = device_write,
    .open           = device_open,
    .unlocked_ioctl = device_ioctl, .release        = device_release,
};
//---------------------------------------------------------------
// Initialize the module - Register the character device
static int __init simple_init(void){
    int rc = -1;
    // init dev struct
    memset( &device_info, 0, sizeof(struct chardev_info) );
    spin_lock_init( &device_info.lock );
    // Register driver capabilities. Obtain major num
    rc = register_chrdev( MAJOR_NUM, DEVICE_RANGE_NAME, &Fops );
    // Negative values signify an error
    if( rc < 0 ){
        printk( KERN_ALERT "%s registraion failed for  %d\n", DEVICE_FILE_NAME, MAJOR_NUM );
        return rc;
    }
    printk( "Registeration is successful. ");
    printk( "If you want to talk to the device driver,\n" );
    printk( "you have to create a device file:\n" );
    printk( "mknod /dev/%s%d c %d minor number\n", DEVICE_FILE_NAME, minor_num, MAJOR_NUM );
    printk( "Dont forget to rm the device file and rmmod when you're done\n" );
    return 0;
}
//---------------------------------------------------------------
static void __exit simple_cleanup(void){
    // Unregister the device
    // Should always succeed
    unregister_chrdev(MAJOR_NUM, DEVICE_RANGE_NAME);
}
//---------------------------------------------------------------
module_init(simple_init);
module_exit(simple_cleanup);
//========================= END OF FILE =========================

// ================ linked list implementation ===============
// display the list
static void printList(void) {
    struct node * ptr = head;
    printk("\n[ ");
    // start from the beginning
    while (ptr != NULL) {
        printk("(%d,%d) ", ptr -> key, ptr -> data);
        ptr = ptr -> next;
    }
    printk(" ]");
}
// insert link at the first location
static void insertFirst(int key, char * data) { // create a link
    struct node * link = (struct node *)kalloc(sizeof(struct node));
    link -> key = key;
    link -> data = data;
    // point it to old first node
    link -> next = head;
    // point first to new first node
    head = link;
}
// is list empty
static int isEmpty(void) {
    return head == NULL;
}
// find a link with given key
static struct node * find(int key) { // start from the first link
    struct node * current = head;
    // if list is empty
    if (head == NULL) {
        return NULL;
    }
    // navigate through list
    while (current -> key != key) { // if it is last node
        if (current -> next == NULL) {
            return NULL;
        } else { // go to next link
            current = current -> next;
        }
    }
    // if data found, return the current Link
    return current;
}
// delete a link with given key
static struct node * delete(int key) { // start from the first link
    struct node * current = head;
    struct node * previous = NULL;
    // if list is empty
    if (head == NULL) {
        return NULL;
    }
    // navigate through list
    while (current -> key != key) { // if it is last node
        if (current -> next == NULL) {
            return NULL;
        } else { // store reference to current link
            previous = current;
            // move to next link
            current = current -> next;
        }
    }
    // found a match, update the link
    if (current == head) { // change first to point to next link
        head = head -> next;
    } else { // bypass the current link
        previous -> next = current -> next;
    }
    return current;
}
// ================ end of linked list implementation ===============
