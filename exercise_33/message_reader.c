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
    char messsage[128];
    int i;
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
    ioctl( file_desc, IOCTL_MSG_SLOT_CHANNEL, atoi(argv[2]));
    if (read(file_desc, messsage, 5) < 0){
        printf("Error: %s\n", strerror(errno));
        exit(1);
    }
    printf("read succesfully the message: %s\n", messsage);
//    TODO: use write() system call
    close(file_desc);
    return 0;
}


