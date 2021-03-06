/*
University of Kentucky 
CS 499 Spring 2017
A Linux based program to run the HTPOW brand laser engravers.
Authors: Lucian Hymer, Grant Sparks, Patrick Thompson

streamer.c handles making the serial connection to the engraver 
and communicating all of the commands to the engraver. It takes the 
pixel data and turns it into instructions the engraver understands. 
It also handles construction the instructions for all of the other 
commands such as setting laser intensity and burn time.
*/

#include <string.h>
#include <errno.h>
#include "streamer.h"
#include "tests/streamer_test_overrides.h"

//This function handles streaming the instructions to the engraver. 
//It sets up the serial initialization, sends the first 5 instructions 
//to begin carving, then sends all of the individual pixels to carve, and 
//finally ends with the last instructions to close out the carving.
void stream(PixelatorState *pixelator, HTPewPewOpts options){
  if(pixelator == NULL){
    ferr("Failed to stream. Invalid pixelator state provided.\n");
    return;
  }
  //Set up the serial connection and store the file descriptor in the 
  //pixelator state struct.
  pixelator->carver_handle = initialize_serial_port(options);
  //Allocate memory for the read buffer.
  pixelator->readbuffer    = (uint8_t *) malloc( READ_BUFFER_SIZE * sizeof pixelator->readbuffer );
  
  // Initialize carver, which returns a pointer to the first pixel.
  //This sends the first 5 instructions to the engraver and gets the 
  //first pixel from the pixelator.
  Pixel *first_pixel = initialize_carver(pixelator, options);
   
  // Carve the image. Returns the counter for the final
  // pixel, which is used in the finalization command.
  //Carve image sends each pixel to carve to the engraver.
  uint8_t last_pixel_counter = carve_image(pixelator, first_pixel);

  // Finalize the carve. Sends the last commands to the engraver 
  //to stop engraving so it is ready for other instructions.
  finalize_carving( pixelator, last_pixel_counter );

  //Free the read buffer.
  free(pixelator->readbuffer);
}

//This function sends the appropriate command to carve a single pixel to the engraver.
void send_pixel_command( PixelatorState *pixelator, uint8_t command, Pixel *pixel, uint8_t aux_code ){
  if(pixelator == NULL){
    ferr("Failed to send pixel command. Invalid pixelator state provided.\n");
    return;
  }
  if(pixel == NULL){
    ferr("Failed to send pixel command. Invalid pixel provided.\n");
    return;
  }
  
  //Create a buffer to construct the 7 bytes of the command.
  uint8_t *pixel_command_buffer = (uint8_t *) malloc( COMMAND_SIZE * sizeof pixel_command_buffer );

  //Build the instruction to send to the engraver.
  pixel_command_buffer[0] = command;
  pixel_command_buffer[1] = pixel->x/COORDINATE_RADIX;
  pixel_command_buffer[2] = pixel->x%COORDINATE_RADIX;
  pixel_command_buffer[3] = pixel->y/COORDINATE_RADIX;
  pixel_command_buffer[4] = pixel->y%COORDINATE_RADIX;
  pixel_command_buffer[5] = aux_code;
  pixel_command_buffer[6] = 0xff;

  //Send the 7 byte instruction over the serial connection.
  send_command( pixelator, pixel_command_buffer);

  //De-allocate the buffer holding the instruction.
  free( pixel_command_buffer );
}

//This function keeps track of the number at the beginning of the engrave pixel 
//instruction. The number goes through the range 3e - 78 in hex and has to roll 
//over to 3e after 78. This function determines what the next number should be.
//It also uses this count to make a read call that will wait for a message from 
//the engraver so that we know the engraver has room in its instruction buffer for 
//more instructions.
uint8_t get_next_pixel_count( PixelatorState *pixelator, uint8_t previous_pixel_count ){
  if(pixelator == NULL){
    ferr("Failed to get next pixel count. Invalid pixelator state provided.\n");
    return -1;
  }
  uint8_t next_pixel_count = 0;
  
  //If the count is at 78 in hex then the next count needs to be 3e 
  //and we have sent 30 instructions since the last read so read again.
  if( previous_pixel_count == MAX_PIXEL_COUNTER_BW ){
    wait_for_carver_response( pixelator );
    
    next_pixel_count = MIN_PIXEL_COUNTER_BW;

    //If the count is at 5a in hex then the next count needs to be 5b 
    //and we have sent 30 instructions since the last read so read again.
  } else if( previous_pixel_count == HALF_PIXEL_COUNTER_BW ){

    wait_for_carver_response( pixelator );

    next_pixel_count = previous_pixel_count + 1;

    //If the instruction count somehow got out of range then exit the program.
  } else if( previous_pixel_count > MAX_PIXEL_COUNTER_BW || previous_pixel_count < MIN_PIXEL_COUNTER_BW ){

    ferr( "Pixel count out of bounds with value %#04x. Something went wrong.\n", previous_pixel_count );
    return -1;
  } else {

    next_pixel_count = previous_pixel_count + 1;

  }

  //Return the instruction count just calculated.
  return next_pixel_count;
}

//This function starts by sending the initialization command 
//to the engraver (1a 00 00 00 00 00 ff). Then sends the first 5 
//commands to the engraver that lets it know how big the image 
//is and that the next instructions will be pixel locations to engrave.
Pixel *initialize_carver(PixelatorState *pixelator, HTPewPewOpts options){
  if(pixelator == NULL){
    ferr("Failed to initialize carver. Invalid pixelator state provided.\n");
    return NULL;
  }

  uint8_t *command_buffer = (uint8_t *) malloc( COMMAND_SIZE * sizeof command_buffer );
  
  // Send draw border box command
  command_buffer[0] = HANDSHAKE_CMD;
  command_buffer[1] = 0x00;
  command_buffer[2] = 0x00;
  command_buffer[3] = 0x00;
  command_buffer[4] = 0x00;
  command_buffer[5] = 0x00;
  command_buffer[6] = 0xff;

  send_command( pixelator, command_buffer );

  //Wait for the bytes from the engraver so that the read buffer isn't full.
  wait_for_carver_response( pixelator );

  //Set laser intensity.
  change_laser_intensity(pixelator, options);
  
  //Set dwell time.
  change_laser_dwell_time(pixelator, options);

  //Set fan speed.
  change_fan_speed(pixelator, options);


  // Send top left border command and confirm it worked.
  Pixel *top_left = get_top_left_pixel(pixelator);
  if( top_left == NULL ){
    ferr( "Error setting image size. Top left corner not found" );
    exit(-1);
  }

  send_pixel_command( pixelator, SET_BORDER_CMD, top_left, 0x00 );

  // Send bottom right border command and confirm it worked.
  Pixel *bottom_right = get_bottom_right_pixel(pixelator);
  if( bottom_right == NULL ){
    ferr( "Error setting image size. Bottom right corner not found" );
    exit(-1);
  }

  send_pixel_command( pixelator, SET_BORDER_CMD, bottom_right, 0x01 );

  // Send draw border box command
  command_buffer[0] = DRAW_BOX_CMD;
  command_buffer[1] = 0x00;
  command_buffer[2] = 0x00;
  command_buffer[3] = 0x00;
  command_buffer[4] = 0x00;
  command_buffer[5] = 0x00;
  command_buffer[6] = 0xff;

  send_command( pixelator, command_buffer );

  // Get the first pixel
  Pixel *first_pixel = get_next_pixel( pixelator );
  if( first_pixel == NULL ){
    ferr( "Unable to retrieve first pixel information" );
    exit(-1);
  }

  send_pixel_command( pixelator, GOTO_CMD, first_pixel, 0x00 );

  // Initialize carving
  command_buffer[0] = INIT_CMD;
  command_buffer[1] = 0x01;
  command_buffer[2] = 0x01;
  command_buffer[3] = 0x00;
  command_buffer[4] = 0x00;
  command_buffer[5] = 0x00;
  command_buffer[6] = 0xff;

  send_command( pixelator, command_buffer );

  free( command_buffer );

  //Wait for the carver to be done initializing.
  wait_for_carver_response( pixelator );

  return first_pixel;
}

//This function gets pixels one at a time from the pixelator 
//and sends an instruction to the engrave to carve them.
uint8_t carve_image(PixelatorState *pixelator, Pixel *first_pixel){
  if(pixelator == NULL){
    ferr("Failed to carve image. Invalid pixelator state provided.\n");
    return -1;
  }

  if(first_pixel == NULL){
    ferr( "No information for first pixel. Is the image blank?\n");
    return -1;
  }

  Pixel *current_pixel = first_pixel;

  //While there are pixels to send keep making instructions and sending them.
  uint8_t pixel_counter    = 0x3d;
  uint8_t previous_counter = 0x3d;

  do{
    // The counter loops through a range defined for carving commands
    send_pixel_command( pixelator, pixel_counter, current_pixel, 0x00 );

    current_pixel    = get_next_pixel( pixelator );

    // Store this value for the finalization command
    previous_counter = pixel_counter;
    pixel_counter    = get_next_pixel_count( pixelator, pixel_counter );

  }while( current_pixel != NULL );

  // Return this value for the finalization command
  return previous_counter;
}

//This function sends the last command to the engraver when we are 
//done sending pixels. It tells the engraver that the run is done 
//so it will be able to receive commands besides just ones to engrave.
void finalize_carving( PixelatorState *pixelator, uint8_t final_counter_value ){
  if(pixelator == NULL){
    ferr("Failed to finalize carving. Invalid pixelator state provided.\n");
    return;
  }
  // Finalization command repeats the previous counter, but replaces
  // coordinate and aux value with 0x9
  uint8_t *command_buffer = (uint8_t *) malloc( COMMAND_SIZE * sizeof command_buffer );

  //Build the command.
  command_buffer[0] = final_counter_value;
  command_buffer[1] = 0x9;
  command_buffer[2] = 0x9;
  command_buffer[3] = 0x9;
  command_buffer[4] = 0x9;
  command_buffer[5] = 0x9;
  command_buffer[6] = 0xff;

  send_command( pixelator, command_buffer );
  free( command_buffer );
}

/* This function was modified from the Non-Canonical Input Processing example on: http://tldp.org/HOWTO/Serial-Programming-HOWTO/x115.html
*/
//This function creates the serial connection to the engraver based on 
//the given port value or the default depending on the options input.
int initialize_serial_port( HTPewPewOpts options) {
  int fd;
  struct termios oldtio,newtio;
  
  //if no port is specified open serial connection on default port
  if(options.port == NULL) {
    fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY ); 
    if (fd <0) {ferr("Unable to open serial port '%s': %s\n",MODEMDEVICE,strerror(errno)); exit(-1); }
  } else {
    fd = open(options.port, O_RDWR | O_NOCTTY ); 
    if (fd <0) {ferr("Unable to open serial port '%s': %s\n",options.port,strerror(errno)); exit(-1); }
  }

  tcgetattr(fd,&oldtio); /* save current port settings */

  bzero(&newtio, sizeof(newtio)); //zero out new termios struct
  newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD; //set baudrate, bits per character, and 2 options that should almost always be enabled
  newtio.c_iflag = IGNPAR; //ignore parity errors
  newtio.c_oflag = 0; //no output options used

  /* set input mode (non-canonical, no echo,...) */
  newtio.c_lflag = 0;

  newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
  newtio.c_cc[VMIN]     = 4;   /* blocking read until 4 chars received */

  tcflush(fd, TCIFLUSH); //flush the queue for safety
  tcsetattr(fd,TCSANOW,&newtio); //change the serial settings to what we just built

  sleep(1); //pause so settings are changed before trying to send information

  return fd; //return file descriptor to the serial connection
}

//Wrapper for the write function to send bytes over the serial connection.
void send_command( PixelatorState *pixelator, uint8_t *command_buffer){
  if(pixelator == NULL){
    ferr("Failed to send command. Invalid pixelator state provided.\n");
    return;
  }
  if(command_buffer == NULL){
    ferr("Failed to send command. Invalid command provided.\n");
    return;
  }

  if(write( pixelator->carver_handle, command_buffer, COMMAND_SIZE)<0)
    perror("Unable to send command");
  usleep(10000);
}

//Wrapper for the read function to receive bytes from the serial connection.
void wait_for_carver_response( PixelatorState *pixelator ){
  if(pixelator == NULL){
    ferr("Failed to wait for carver. Invalid pixelator state provided.\n");
    return;
  }
  if( read(pixelator->carver_handle, pixelator->readbuffer, READ_BUFFER_SIZE) == -1 ){
    ferr("Failed to read from carver\n");
    perror("");
    exit(-1);
  } else {
    fnote( "Carver sent back (this probably isn't formatted correctly): %x\n", pixelator->readbuffer );
  }
}

//This function changes the laser intensity setting of the laser. Values between 0 and a.
void change_laser_intensity(PixelatorState* pixelator, HTPewPewOpts options){
  
  uint8_t *command_buffer = (uint8_t *) malloc( COMMAND_SIZE * sizeof command_buffer );
  
  // Send command to set the laser intensity. 
  command_buffer[0] = LASER_INTENSITY_CMD;
  command_buffer[1] = options.intensity;
  command_buffer[2] = 0x00;
  command_buffer[3] = 0x00;
  command_buffer[4] = 0x00;
  command_buffer[5] = 0x00;
  command_buffer[6] = 0xff;

  send_command( pixelator, command_buffer );
  free( command_buffer );
}

//This function changes the dwell time of the laser. Values between 0 and fa.
void change_laser_dwell_time(PixelatorState* pixelator, HTPewPewOpts options){
    
  uint8_t *command_buffer = (uint8_t *) malloc( COMMAND_SIZE * sizeof command_buffer );
  
  // Send command to set the laser intensity. 
  command_buffer[0] = LASER_DWELL_TIME_CMD;
  command_buffer[1] = options.burn;
  command_buffer[2] = 0x00;
  command_buffer[3] = 0x00;
  command_buffer[4] = 0x00;
  command_buffer[5] = 0x00;
  command_buffer[6] = 0xff;

  send_command( pixelator, command_buffer );
  free( command_buffer );
}

//This function changes the fan speed of the fan. Values between 0 and 10.
void change_fan_speed(PixelatorState* pixelator, HTPewPewOpts options){
    
  uint8_t *command_buffer = (uint8_t *) malloc( COMMAND_SIZE * sizeof command_buffer );
  
  // Send command to set the laser intensity. 
  command_buffer[0] = CHANGE_FAN_SPEED_CMD;
  command_buffer[1] = options.fan_speed;
  command_buffer[2] = 0x00;
  command_buffer[3] = 0x00;
  command_buffer[4] = 0x00;
  command_buffer[5] = 0x00;
  command_buffer[6] = 0xff;

  send_command( pixelator, command_buffer );
  free( command_buffer );
}

