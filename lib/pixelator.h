#ifndef PIXELATOR_H
#define PIXELATOR_H

#include "image.h"
#include <stdint.h>

typedef struct pixel
{
  // x & y should never exceed 1300 (0x0500)
  short x;
  short y;
  // value cannot exceed 255
  unsigned char intensity;
} Pixel;

// This struct is used by the pixelator to decide where it is in the image
typedef struct pixelator_state
{
  // image file location, basically
  MagickWand* wand;
  PixelIterator* it;
  Pixel* px;
  // current x & y location for scanning
  short x;
  short y;
  // scan_dir
  // 0 = horizontal
  // 1 = vertical
  unsigned char scan_dir; 
  int carver_handle; 
  uint8_t *readbuffer; 
} PixelatorState;

void center_pixel(PixelatorState* state);
unsigned char get_pixel_intensity(PixelWand* pixel);

PixelatorState* pixelator_init(HTPewPewOpts opts, MagickWand* wand);
Pixel scan_until_dark_pixel(PixelatorState* state);
Pixel* get_top_left_pixel(PixelatorState* state);
Pixel* get_bottom_right_pixel(PixelatorState* state);
Pixel* get_next_pixel(PixelatorState* state);

#endif
