#include "message_slot.h"
#include <fcntl.h>      /* open */
#include <unistd.h>     /* exit */
#include <sys/ioctl.h>  /* ioctl */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
    int file_desc;
    printf("inside sender\n");
    file_desc = open( argv[1], O_RDWR );
    if( file_desc < 0 ){
        printf ("Can't open device file: %s\n", argv[1]);
        exit(-1);
    }
    ioctl( file_desc, IOCTL_MSG_SLOT_CHANNEL, argv[2]);
    (void)write( file_desc, argv[3], strlen(argv[3]));
    close(file_desc);
    printf("sender ended successfully\n");
    return 0;
}

