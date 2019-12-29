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
    file_desc = open( argv[1], O_RDWR );
    if( file_desc < 0 ){
        printf("Error: %s\n", strerror(errno));
        exit(1);
    }
    ioctl( file_desc, IOCTL_MSG_SLOT_CHANNEL, atol(argv[2]));
    if(write( file_desc, argv[3], strlen(argv[3])) != strlen(argv[3])){
        printf("Error: %s\n", strerror(errno));
        exit(1);
    }
    close(file_desc);
    return 0;
}
