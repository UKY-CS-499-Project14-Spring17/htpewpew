#include "options.h"
#include "image.h"

void stream(PixelatorState *pixelator){
  initialize_carver(pixelator);

}

void send_pixel_command( uint8_t command, Pixel *pixel, uint8_t aux_code ){
  // TODO carve command counter
  // Do something about grayscale here
  char *pixel_command_buffer = (char *) malloc( COMMAND_SIZE * sizeof pixel_command_buffer );
  pixel_command_buffer = { command, pixel->x/MAX_LOWER_BYTE, pixel->x%MAX_LOWER_BYTE, pixel->y/MAX_LOWER_BYTE, pixel->y%MAX_LOWER_BYTE, aux_code, 0xff }; 
  send_command(pixel_command_buffer);
  free( pixel_command_buffer );
}

uint8_t get_next_pixel_count( uint8_t previous_pixel_count ){
  uint8_t next_pixel_count = 0;
  if( previous_pixel_count == MAX_PIXEL_COUNTER_BW ){
    next_pixel_count = MIN_PIXEL_COUNTER_BW;
  } else if( previous_pixel_count > MAX_PIXEL_COUNTER_BW || previous_pixel_count < MIN_PIXEL_COUNTER_BW ){
    ferr( "Pixel count out of bounds with value %02x. Something went wrong.", previous_pixel_count );
  } else {
    next_pixel_count = previous_pixel_count + 1;
  }

  return next_pixel_count;
}

void initialize_carver(PixelatorState *pixelator){
  Pixel *top_left     = get_top_left_pixel(pixelator);
  Pixel *bottom_right = get_bottom_right_pixel(pixelator);

  if( top_left == NULL || bottom_right == NULL ){
    ferr( "Error setting image size. Top left: %sfound, bottom right: %sfound\n", top_left == NULL ? "not " : "", bottom_right == NULL ? "not " : "" );
  }

  char *command_buffer = (char *) malloc( COMMAND_SIZE * sizeof command_buffer );

  // Send top left border command
  send_pixel_command( SET_BORDER_CMD, top_left, 0x00 );

  // Send bottom right border command
  send_pixel_command( SET_BORDER_CMD, top_left, 0x01 );

  // Send draw border box command
  command_buffer = { DRAW_BOX_CMD, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff }; 
  send_command( command_buffer );

  // Get the first pixel
  Pixel *current_pixel = get_next_pixel( pixelator );
  if( current_pixel == NULL ){
    ferr( "Unable to retrieve first pixel information" );
  }

  send_pixel_command( GOTO_CMD, current_pixel, 0x00 );

  // Initialize carving
  command_buffer = { INIT_CMD, 0x01, 0x01, 0x00, 0x00, 0x00, 0xff }; 
  send_command( command_buffer );

  uint8_t pixel_counter    = 0x3d;
  uint8_t previous_counter = 0x3d;

  do{
    // The counter loops through a range defined for carving commands
    send_pixel_command( pixel_counter, current_pixel, 0x00 );

    current_pixel    = get_next_pixel( pixelator );

    // Store this value for the finalization command
    previous_counter = pixel_counter;
    pixel_counter    = get_next_pixel_count( pixel_counter );

  }while( current_pixel != NULL );

  // Finalization command repeats the previous counter, but replaces
  // coordinate and aux value with 0x9
  command_buffer = { previous_counter, 0x9, 0x9, 0x9, 0x9, 0x9, 0xff };
  send_command( command_buffer );
}
