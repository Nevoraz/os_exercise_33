current status:

now the original ioctl version is in the file, I need to change it

Tips for ex3

(1) work with the makefile from last week's example (with includes)
(2) implementing msg -> msg is identified by <int,int> where first one 0-255 and second one is long-max. msg length is 128 ?
(3) messages can include NULL and so on (so no strcopy)
(4) user opened a device file, after that did ioctl

240 is the major.

c 240 17 fd = open("device file,...)"
iocti(...,3000)

global data structure should hold the channel number (linked list!!!)

release all memory allocation (!!!!!) its critical.

performance is not important here

global static driver

for all minor - we should keep a stage.


file operations struct inode * points on a struct that holds the origin file... we want the minor number (iminor intrclude)

void *private_data - it is null at the beggining, we should put a pointer to datastructure that includes the minor number of the currently open file, and the channel we are working on (it is 0 at first).
keeps the state of the current file.

release its after close to relasee the private data structure.
