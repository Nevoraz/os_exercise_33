#include "message_slot.h"
#include <fcntl.h>      /* open */
#include <unistd.h>     /* exit */
#include <sys/ioctl.h>  /* ioctl */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    int file_desc;
    char messsage[128];
    printf("inside reader\n");
    file_desc = open( argv[1], O_RDWR );
    if( file_desc < 0 ){
        printf ("Can't open device file: %s\n",
                DEVICE_FILE_NAME);
        exit(-1);
    }
    ioctl( file_desc, IOCTL_MSG_SLOT_CHANNEL, argv[2]);
    (void)read(file_desc, messsage, 5);
    printf("read succesfully the message: %s\n", messsage);
//    TODO: use write() system call
    close(file_desc);
    return 0;
}


