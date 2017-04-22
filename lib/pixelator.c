/*
University of Kentucky 
CS 499 Spring 2017
A Linux based program to run the HTPOW brand laser engravers.
Authors: Lucian Hymer, Grant Sparks, Patrick Thompson

pixelator.c handles scanning through the image and finding the 
pixels that need to be carved to hand to the streamer. It scans 
through an image and finds the pixels position and brightness, 
represented here as intensity, and stores it in a "pixel" struct 
that is passed back and forth between the pixelator and the 
streamer.
*/

#include "pixelator.h"

//This function adds padding to pixels if the image size is 
//smaller than the max size so that the image is printed in 
//the middle of the canvas or workspace instead of from the 
//upper left corner.
void center_pixel(PixelatorState* state) {
  size_t width, height, padx, pady;
  //Get current image height and width.
  width  = MagickGetImageWidth( state->wand );
  height = MagickGetImageHeight( state->wand );

  //Calculate padding that needs to be added.
  padx   = (CANVAS_SIZE - width )/2;
  pady   = (CANVAS_SIZE - height)/2;

  //Add padding to the pixel.
  state->px->x += padx;
  state->px->y += pady;
  
  //Add offset to the pixel.
  state->px->x += state->x_offset;
  state->px->y += state->y_offset;
}

//This function finds the brightness of the input pixel 
//and converts it to a darkness measurement to be 
//compatible with the engraver's protocol.
unsigned char get_pixel_intensity(PixelWand* pixel) {
  double hue, sat, light_d;
  unsigned char darkness;
  // for each x value, move through each pixel
  PixelGetHSL(pixel, &hue, &sat, &light_d);
  // 255 is the max burn intensity
  // 0 is no burn
  darkness = (char) (255 - (255 * light_d));
  return(darkness);
}

//This function initializes the pixelator by creating the PixelatorState 
//struct and setting the starting values inside it.
PixelatorState* pixelator_init(HTPewPewOpts opts, MagickWand* wand) {
  // allocate space for the state, return the pointer
  PixelatorState* state;
  state = malloc( sizeof(*state) );
  state->it = NewPixelIterator(wand);
  // set default values
  state->wand = wand;
  state->x = -1;
  state->y = 0;
  state->px = NULL;
  state->scan_dir = 0;

  state->x_offset = opts.x;
  state->y_offset = opts.y;
  // TODO change scan direction
  // TODO the fuction
  return(state);
}

//TODO describe
Pixel scan_until_dark_pixel(PixelatorState* state) {
  unsigned char darkness;
  size_t width;
  Pixel first_dark;
  first_dark.x = 0;
  first_dark.y = 0;
  PixelIterator* it = NewPixelIterator(state->wand);
  // collect the first row with a non-white pixel
  PixelSetFirstIteratorRow(it);
  PixelWand** pwand = PixelGetCurrentIteratorRow(it, &width);
  while( pwand != NULL ) {
    for( first_dark.x = 0; first_dark.x < width; first_dark.x++ ) {
      // for each x value, move through each pixel
      darkness = get_pixel_intensity(pwand[first_dark.x]);
      if( darkness != 0 )
        return( first_dark );
    }
    // move to the next row
    pwand = PixelGetNextIteratorRow(state->it, &width);
    PixelSyncIterator(state->it);
    // drop to the next row
    first_dark.y += 1;
  }
  it = DestroyPixelIterator(it);
  return first_dark;
}

//This function scans through the image to find the top left most 
//pixel. The engraver needs this information in its protocol.
Pixel* get_top_left_pixel(PixelatorState* state) {
  PixelWand* pwhite;
  if( state->px == NULL )
    state->px = malloc( sizeof(*(state->px)) );
  state->px->intensity = 0;
  Pixel top = scan_until_dark_pixel(state);
  // make a white pixel to serve as the background
  pwhite = NewPixelWand();
	PixelSetColor(pwhite,"white");
  // rotate the image 90 degrees (clockwise?)
  MagickRotateImage(state->wand, pwhite, 90);
  Pixel left = scan_until_dark_pixel(state);
  // rotate the image back 90 degrees (ccwise?)
  MagickRotateImage(state->wand, pwhite, -90);
  state->px->y = top.y;
  state->px->x = left.y; // remember, this is rotated
  center_pixel(state);
  return(state->px);
}

//This function scans through the image to find the bottom right most 
//pixel. The engraver needs this information in its protocol.
Pixel* get_bottom_right_pixel(PixelatorState* state) {
  size_t width, height;
  //Get current image height and width.
  width  = MagickGetImageWidth( state->wand );
  height = MagickGetImageHeight( state->wand );
  PixelWand* pwhite;
  if( state->px == NULL )
    state->px = malloc( sizeof(*(state->px)) );
  state->px->intensity = 0;
  // make a white pixel to serve as the background
  pwhite = NewPixelWand();
	PixelSetColor(pwhite,"white");
  // rotate the image 90 degrees (clockwise?)
  MagickRotateImage(state->wand, pwhite, -90);
  Pixel right = scan_until_dark_pixel(state);
  // rotate the image back 90 degrees (ccwise?)
  MagickRotateImage(state->wand, pwhite, -90);
  Pixel bottom = scan_until_dark_pixel(state);
  MagickRotateImage(state->wand, pwhite, 180);
  state->px->y = height - bottom.y;
  state->px->x = width  - right.y; // remember, this is rotated
  center_pixel(state);
  return(state->px);
}

//This function starts at the position of the given pixel and 
//finds the next one to engrave by scanning to the right and 
//down from the given pixel until it finds one that is dark 
//enough to be engraved.
Pixel* get_next_pixel(PixelatorState* state) {
  // allocate space for the pixel, return the pointer
  size_t width, height;
  short x = state->x + 1;
  unsigned char darkness;
  PixelWand** pwand;
  height = MagickGetImageHeight( state->wand );
  if( state->px == NULL )
    state->px = malloc( sizeof(*(state->px)) );
  // check that PixelIterator is valid
  if (state->it == NULL)
    throw_wand_exception(state->wand);
  pwand = PixelGetCurrentIteratorRow(state->it, &width);
  while( pwand != NULL && state->y < height) {
    for( /* do not initialize x */ ; x < width; x++ ) {
      // for each x value, move through each pixel
      darkness = get_pixel_intensity(pwand[x]);
      // 255 is the max burn intensity
      // 0 is no burn
      if( darkness != 0 ) {
        state->x = x;
        fnote("y=%d x=%d i=%d\n",state->y, state->x, darkness);
        state->px->x = state->x;
        state->px->y = state->y;
        state->px->intensity = darkness;
        center_pixel(state);
        fnote("y=%d x=%d\n",state->px->y, state->px->x);
        return(state->px);
      }
    }
    x = 0;
    // move to the next row
    pwand = PixelGetNextIteratorRow(state->it, &width);
    PixelSyncIterator(state->it);
    // drop to the next row
    state->y += 1;
  }
  // if you get here, there are no pixels left
  // the image is complete
  state->it = DestroyPixelIterator(state->it);
  free(state->px);
  state->px = NULL;
  return(state->px);
}
