#include "../pixelator.h"
Pixel *streamer__get_top_left_pixel(PixelatorState* state){
  Pixel *pixel = (Pixel *) malloc( sizeof pixel);
  pixel->x = 100;
  pixel->y = 132;
  pixel->intensity = 100;
  return pixel;
}

Pixel *streamer__get_bottom_right_pixel(PixelatorState* state){
  Pixel *pixel = (Pixel *) malloc( sizeof pixel);
  pixel->x = 101;
  pixel->y = 133;
  pixel->intensity = 101;
  return pixel;
}

Pixel *streamer__get_next_pixel(PixelatorState* state){
  Pixel *pixel = streamer__get_top_left_pixel(state);
  return pixel;
}
