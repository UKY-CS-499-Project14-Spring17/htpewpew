#ifndef PIXELATOR_H
#define PIXELATOR_H

#include "image.h"

// This struct is used by the pixelator to decide where it is in the image
struct pixelator_state
{
  // image file location, basically
  MagickWand* wand;
  PixelIterator* it;
  // current x location for scanning
  short x;
  // scan_dir
  // 0 = horizontal
  // 1 = vertical
  unsigned char scan_dir; 
};

struct pixel
{
  // x & y should never exceed 1300 (0x0500)
  short x;
  short y;
  // value cannot exceed 255
  unsigned char intensity;
};

struct pixelator_state* pixelator_init(struct htpewpew_opts opts, MagickWand* wand);
struct pixel* get_top_left_pixel(struct pixelator_state* state);
struct pixel* get_bottom_right_pixel(struct pixelator_state* state);
struct pixel* get_next_pixel(struct pixelator_state* state);

#endif
