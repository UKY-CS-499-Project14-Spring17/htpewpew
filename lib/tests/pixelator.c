#include "../pixelator.h"

// tests
int center_pixel_test_1();
int center_pixel_test_2();
int center_pixel_test_3();
int get_pixel_intensity_test_1(); // TODO
int get_pixel_intensity_test_2(); // TODO
int get_pixel_intensity_test_3(); // TODO
int get_pixel_intensity_test_4(); // TODO
int pixelator_init_test_1(); // TODO
int top_left_test_1(); // TODO
int top_left_test_2(); // TODO
int top_left_test_3(); // TODO
int top_left_test_4(); // TODO
int bottom_right_test_1(); // TODO
int bottom_right_test_2(); // TODO
int bottom_right_test_3(); // TODO
int bottom_right_test_4(); // TODO
int get_next_test_1(); // TODO
int get_next_test_2(); // TODO
int get_next_test_3(); // TODO
int get_next_test_4(); // TODO
int get_next_test_5(); // TODO
int get_next_test_6(); // TODO

// test #1
// pixel pointer is NULL
// should raise an error
int center_pixel_test_1() {
  int retval = 1;
  ttext("If the following test issues errors, that's okay.\n");
  tmsg("center_pixel_test_1");
  PixelatorState state;
  state.px = NULL;
  if( center_pixel(&state) == -1 ) { // value not allocated
    tpass("\n");
    retval = 0;
  } else {
    tfail("\n");
  }
  return retval;
}

// test #2
// pixel pointer is NULL
// should raise an error
int center_pixel_test_2(PixelatorState state) {
  int retval = 1;
  tmsg("center_pixel_test_2");
  Pixel* px = malloc( sizeof(Pixel) );
  state.px = px;
  state.px->x = 600;
  state.px->y = 1000;
  if( center_pixel(&state) == -1 ) { // value not allocated
    tpass("\n");
    retval = 0;
  } else {
    tfail("\n");
  }
  free( px );
  return retval;
}

// test #3
// pixel pointer is NULL
// should raise an error
int center_pixel_test_3(PixelatorState state) {
  int retval = 1;
  tmsg("center_pixel_test_3");
  Pixel* px = malloc( sizeof(Pixel) );
  state.px = px;
  state.px->x = 200;
  state.px->y = 200;
  if( center_pixel(&state) == 0 ) { // value not allocated
    tpass("\n");
    retval = 0;
  } else {
    tfail("\n");
  }
  free( px );
  return retval;
}

// test #1
// pixel pointer is NULL
// should raise an error
int get_pixel_intensity_test_1() {
  int retval = 1;
  tmsg("get_pixel_intensity_test_1");
  Pixel* px = malloc( sizeof(Pixel) );
  state.px = px;
  state.px->x = 200;
  state.px->y = 200;
  if( center_pixel(&state) == 0 ) { // value not allocated
    tpass("\n");
    retval = 0;
  } else {
    tfail("\n");
  }
  free( px );
  return retval;
}


int pixelator_tests() {
  int retval = 0;

  // defaults for this group
  // use this (or any image)
  HTPewPewOpts opts;
  opts.infile    = "tests/team.jpg";
  opts.threshold = -1;
  opts.x         = 0;
  opts.y         = 0;
  opts.dry       = 1;
  opts.intensity = 5;
  opts.outfile   = NULL;
  opts.shell     = 0;

  MagickWand* wand = prepare_image(opts);
  PixelatorState state;
  state.wand = wand;
  
  retval |= center_pixel_test_1();
  retval |= center_pixel_test_2(state);
  retval |= center_pixel_test_3(state);

  // do cleanup
  wand = DestroyMagickWand(wand);
  return(retval);
}
