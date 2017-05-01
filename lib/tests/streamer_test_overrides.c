#include "../pixelator.h"
Pixel *streamer__get_top_left_pixel(PixelatorState* state){
  Pixel *pixel = (Pixel *) malloc( sizeof pixel);
  pixel->x = 100;
  pixel->y = 132;
  pixel->intensity = 100;
  return pixel;
}
