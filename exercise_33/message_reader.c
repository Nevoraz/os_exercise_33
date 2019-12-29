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
    int message_length;
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
    if ( write(STDOUT_FILENO, message, message_length) < 0){
        printf("Error: %s\n", strerror(errno));
        exit(1);
    }
    close(file_desc);
    return 0;
}


