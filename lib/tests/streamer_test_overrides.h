#ifdef TESTOVERRIDE
  Pixel *streamer__get_top_left_pixel(PixelatorState* state);
  #define get_top_left_pixel(x) streamer__get_top_left_pixel(x)
#endif
