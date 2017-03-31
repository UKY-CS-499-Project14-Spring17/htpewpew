#include "pixelator.h"

struct pixelator_state* pixelator_init(struct htpewpew_opts opts, MagickWand* wand) {
  // allocate space for the state, return the pointer
  struct pixelator_state* state;
  state = malloc( sizeof(*state) );
  // set default values
  state->x = -1;
  state->y = 0;
  state->it = NewPixelIterator(wand);
  state->scan_dir = 0;
  // TODO change scan direction
  // TODO the fuction
  return(state);
}

struct pixel* get_next_pixel(struct pixelator_state* state) {
  // allocate space for the pixel, return the pointer
  size_t width;
  short x = state->x + 1;
  double hue, sat, light_d;
  unsigned char darkness;
  PixelWand** pwand;
  struct pixel* px;
  px = malloc( sizeof(*px) );
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
        px->x = state->x;
        px->y = state->y;
        px->intensity = darkness;
        return(px);
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
  px = NULL;
  return(px);
}
