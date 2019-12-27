#include "message_slot.h"
#include <fcntl.h>      /* open */
#include <unistd.h>     /* exit */
#include <sys/ioctl.h>  /* ioctl */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


int main(int argc, char **argv){
    int file_desc;
    char message[128] = "";
    int i, message_length;
    printf("argc = %d\n",argc);
    for(i=0;i<argc;i++)
    {
        printf("%d = %s\n",i, argv[i]);
    }
    printf("inside reader\n");
    file_desc = open( argv[1], O_RDWR );
    if( file_desc < 0 ){
        printf("Error: %s\n", strerror(errno));
        exit(1);
    }
    ioctl( file_desc, IOCTL_MSG_SLOT_CHANNEL, atol(argv[2]));
    message_length = read(file_desc, message, 1);
    if ( message_length < 0){
        printf("Error: %s\n", strerror(errno));
        exit(1);
    }
    printf("read succesfully the message: %s\n", message);
    write(STDOUT_FILENO, message, message_length);//    TODO: use write() system call
    close(file_desc);
    return 0;
}


