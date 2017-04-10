/* This code was modified from the Non-Canonical Input Processing example on: http://tldp.org/HOWTO/Serial-Programming-HOWTO/x115.html
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#define BAUDRATE B115200
#define MODEMDEVICE "/dev/ttyUSB0"




void serial_init()
{
int fd,c, res;
struct termios oldtio,newtio;


fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY ); 
if (fd <0) {perror(MODEMDEVICE); exit(-1); }

tcgetattr(fd,&oldtio); /* save current port settings */

bzero(&newtio, sizeof(newtio));
newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
newtio.c_iflag = IGNPAR;
newtio.c_oflag = 0;

/* set input mode (non-canonical, no echo,...) */
newtio.c_lflag = 0;
 
newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
newtio.c_cc[VMIN]     = 4;   /* blocking read until 4 chars received */

tcflush(fd, TCIFLUSH);
tcsetattr(fd,TCSANOW,&newtio);

sleep(2);

}
