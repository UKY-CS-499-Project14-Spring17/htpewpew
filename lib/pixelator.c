#include "pixelator.h"

struct pixelator_state* pixelator_init(struct htpewpew_opts opts, MagickWand* wand) {
  // allocate space for the state, return the pointer
  struct pixelator_state* state;
  state = malloc( sizeof(*state) );
  // set default values
  state->x = 0;
  state->it = NewPixelIterator(wand);
  state->scan_dir = 0;
  // TODO change scan direction
  // TODO the fuction
  return(state);
}

struct pixel* get_next_pixel(struct pixelator_state* state) {
  // allocate space for the pixel, return the pointer
  size_t width;
  Quantum darkness;
  PixelWand** pwand;
  struct pixel* px;
  px = malloc( sizeof(*px) );
  // check that PixelIterator is valid
  if (state->it == NULL)
    throw_wand_exception(state->wand);
  pwand = PixelGetCurrentIteratorRow(state->it, &width);
  while( pwand != NULL ) {
    for( short x = state->x; x < width; x++ ) {
      // for each x value, move through each pixel
      darkness = PixelGetBlackQuantum(pwand[x]);
      fnote("%d\n",darkness);
    }
    // move to the next row
    pwand = PixelGetNextIteratorRow(state->it, &width);
    PixelSyncIterator(state->it);
  }
  // if you get here, there are no pixels left
  // the image is complete
  state->it = DestroyPixelIterator(state->it);
  px = NULL;
  return(px);
}
