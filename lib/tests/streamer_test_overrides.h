#ifdef TESTOVERRIDE
  #include "../pixelator.h"
  #ifndef STREAMER_OVERRIDES
    #define STREAMER_OVERRIDES
    Pixel *streamer__get_top_left_pixel(PixelatorState* state);
    Pixel *streamer__get_bottom_right_pixel(PixelatorState* state);
    Pixel *streamer__get_next_pixel(PixelatorState* state);
    #ifdef MODEMDEVICE
      #undef MODEMDEVICE
    #endif
    #define MODEMDEVICE "/dev/pts/0"
  #endif
  #define get_bottom_right_pixel(pixelator) streamer__get_bottom_right_pixel(pixelator)
  #define get_top_left_pixel(pixelator) streamer__get_top_left_pixel(pixelator)
  #define get_next_pixel(pixelator) streamer__get_next_pixel(pixelator)
#endif
