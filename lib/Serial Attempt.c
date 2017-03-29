      #include <sys/types.h>
      #include <sys/stat.h>
      #include <fcntl.h>
      #include <termios.h>
      #include <stdio.h>
      #include <stdlib.h>
      #include <strings.h>
      #include <unistd.h>
        
      #define BAUDRATE B115200
      #define MODEMDEVICE "/dev/ttyACM0"
      #define _POSIX_SOURCE 1 /* POSIX compliant source */
      #define FALSE 0
      #define TRUE 1
        
      volatile int STOP=FALSE; 
       
      int main()
      {
        int fd,c, res;
        struct termios oldtio,newtio;
        unsigned char buf[255];
        
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

	int n = 0;
	buf[0] = 0x0a;
	buf[1] = 0x00;
	buf[2] = 0x00;
	buf[3] = 0x00;
	buf[4] = 0x00;
	buf[5] = 0x00;
	buf[6] = 0xff;

	printf("Writing to engraver \n");
	int i;
	for(i = 0; i<7; i = i+1){
	printf("0x%x ", buf[i]);

	}
	printf("\n");

	n = write(fd, buf, 7);
	if(n<0)
	  fputs("write() failed\n", stderr);
        
        
        //while (STOP==FALSE) {       /* loop for input */
          res = read(fd,buf,255);   /* returns after 4 chars have been input */
          buf[res]=0; 		/* so we can printf... */

	printf("Received from engraver \n");
	for(i=0; i<res; i = i+1){
          printf("0x%x ", buf[i]);
	}
	printf("\n");
        //  if (buf[0]=='z') STOP=TRUE;
        //}
        tcsetattr(fd,TCSANOW,&oldtio);
	return 0;
}
