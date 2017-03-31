#include "pixelator.h"

PixelatorState* pixelator_init(HTPewPewOpts opts, MagickWand* wand) {
  // allocate space for the state, return the pointer
  PixelatorState* state;
  state = malloc( sizeof(*state) );
  // set default values
  state->x = -1;
  state->y = 0;
  state->px = NULL;
  state->it = NewPixelIterator(wand);
  state->scan_dir = 0;
  // TODO change scan direction
  // TODO the fuction
  return(state);
}

Pixel* get_top_left_pixel(PixelatorState* state) {
  if( state->px == NULL )
    state->px = malloc( sizeof(*(state->px)) );
  state->px->x = 0;
  state->px->y = 0;
  state->px->intensity = 0;
  return(state->px);
}

Pixel* get_bottom_right_pixel(PixelatorState* state) {
  if( state->px == NULL )
    state->px = malloc( sizeof(*(state->px)) );
  state->px->x = 0x459;
  state->px->y = 0x459;
  state->px->intensity = 0;
  return(state->px);
}

Pixel* get_next_pixel(PixelatorState* state) {
  // allocate space for the pixel, return the pointer
  size_t width;
  short x = state->x + 1;
  double hue, sat, light_d;
  unsigned char darkness;
  PixelWand** pwand;
  if( state->px == NULL )
    state->px = malloc( sizeof(*(state->px)) );
  // check that PixelIterator is valid
  if (state->it == NULL)
    throw_wand_exception(state->wand);
  pwand = PixelGetCurrentIteratorRow(state->it, &width);
  while( pwand != NULL ) {
    for( /* do not initialize x */ ; x < width; x++ ) {
      // for each x value, move through each pixel
      PixelGetHSL(pwand[x], &hue, &sat, &light_d);
      // 255 is the max burn intensity
      // 0 is no burn
      darkness = (char) (255 - (255 * light_d));
      if( darkness != 0 ) {
        state->x = x;
        fnote("y=%d x=%d i=%d\n",state->y, state->x, darkness);
        state->px->x = state->x;
        state->px->y = state->y;
        state->px->intensity = darkness;
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
