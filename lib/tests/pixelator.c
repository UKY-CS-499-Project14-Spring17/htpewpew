#include "../pixelator.h"

// tests
int center_pixel_test_1();
int center_pixel_test_2(PixelatorState);
int center_pixel_test_3(PixelatorState);
int get_pixel_intensity_test_1(PixelatorState);
int get_pixel_intensity_test_2(PixelatorState);
int get_pixel_intensity_test_3(PixelatorState);
int get_pixel_intensity_test_4();
int pixelator_init_test_1();
int top_left_pixel_test_1();
int top_left_pixel_test_2(PixelatorState);
int top_left_pixel_test_3(HTPewPewOpts);
int top_left_pixel_test_4(PixelatorState);
int bottom_right_pixel_test_1();
int bottom_right_pixel_test_2(PixelatorState);
int bottom_right_pixel_test_3(HTPewPewOpts);
int bottom_right_pixel_test_4(PixelatorState);
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
  if( center_pixel(&state) == 0 ) { // value is good allocated
    tpass("\n");
    retval = 0;
  } else {
    tfail("\n");
  }
  free( px );
  return retval;
}

// test #1
// pixel pointer is a white pixel
// should raise an error
// TODO this fails right now because of the antialiasing
int get_pixel_intensity_test_1(PixelatorState state) {
  int retval = 1;
  size_t width = 0;
  unsigned char intensity;
  tmsg("get_pixel_intensity_test_1");
  PixelIterator* it = NewPixelIterator(state.wand);
  // collect the first row with a non-white pixel
  PixelSetFirstIteratorRow(it);
  PixelWand** pwand = PixelGetCurrentIteratorRow(it, &width);
  intensity = get_pixel_intensity( pwand[0] );
  if( intensity == 0 ) { // intensity == white
    tpass("\n");
    retval = 0;
  } else {
    tfail("actual intensity was %d, expected 0\n", intensity);
  }
  it = DestroyPixelIterator(it);
  return retval;
}

// test #2
// pixel pointer is a 50% grey pixel
// should raise an error
int get_pixel_intensity_test_2(PixelatorState state) {
  int retval = 1;
  size_t width = 0;
  unsigned char intensity;
  tmsg("get_pixel_intensity_test_2");
  PixelIterator* it = NewPixelIterator(state.wand);
  // collect the first row with a non-white pixel
  PixelSetFirstIteratorRow(it);
  PixelWand** pwand = PixelGetCurrentIteratorRow(it, &width);
  intensity = get_pixel_intensity( pwand[5] );
  if( intensity == 127 ) { // intensity == white
    tpass("\n");
    retval = 0;
  } else {
    tfail("actual intensity was %d, expected 0\n", intensity);
  }
  it = DestroyPixelIterator(it);
  return retval;
}

// test #3
// pixel pointer is a black pixel
// should raise an error
// TODO this fails right now because of the antialiasing
int get_pixel_intensity_test_3(PixelatorState state) {
  int retval = 1;
  size_t width = 0;
  unsigned char intensity;
  tmsg("get_pixel_intensity_test_3");
  PixelIterator* it = NewPixelIterator(state.wand);
  // collect the first row with a non-white pixel
  PixelSetFirstIteratorRow(it);
  PixelWand** pwand = PixelGetCurrentIteratorRow(it, &width);
  intensity = get_pixel_intensity( pwand[10] );
  if( intensity == 255 ) { // intensity == white
    tpass("\n");
    retval = 0;
  } else {
    tfail("actual intensity was %d, expected 0\n", intensity);
  }
  it = DestroyPixelIterator(it);
  return retval;
}

// test #4
// pixel pointer is a NULL
// should exit and display an error
int get_pixel_intensity_test_4() {
  // setup fork
  int fdn[2];
  pipe(fdn);

  if (fork() == 0) {

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
    PixelWand* pwand = NULL;
    get_pixel_intensity( pwand );
    close(fdn[1]);
    exit(0);

  } else {
    // parent process
    close(fdn[1]);
    tmsg("get_pixel_intensity_test_4");
    int wstat;
    wait( &wstat );
    char errmsg[89];
    char experrmsg[89] = KBLD KRED "Error: Pixel value in get_pixel_intensity is NULL\n" KNRM;
    read(fdn[0], errmsg, 89);
    close(fdn[0]);
    errmsg[88] = '\0';
    if( strcmp(errmsg,experrmsg) == 0 ) {
      tpass("\n");
      return 0;
    } else { 
      tfail("message did not match expected.\n");
      ttext("%d %s\n",strlen(errmsg), errmsg);
      ttext("%d %s\n",strlen(experrmsg), experrmsg);
      return 1;
    }
  }
}

// test #1
// wand pointer is a NULL
// should exit and display an error
int pixelator_init_test_1(HTPewPewOpts opts) {
  // setup fork
  int fdn[2];
  pipe(fdn);

  if (fork() == 0) {

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
    MagickWand* wand = NULL;
    pixelator_init(opts, wand);
    close(fdn[1]);
    exit(0);
  } else {
    // parent process
    close(fdn[1]);
    tmsg("pixelator_init_test_1");
    int wstat;
    wait( &wstat );
    char errmsg[99];
    char experrmsg[99] = KBLD KRED "Error: Wand in pixelator_init is NULL. Use the image.c/prepare_image function first.\n" KNRM;
    read(fdn[0], errmsg, 98);
    close(fdn[0]);
    errmsg[98] = 0;
    if( strcmp(errmsg,experrmsg) == 0 ) {
      tpass("\n");
      return 0;
    } else { 
      tfail("message did not match expected.\n");
      ttext("%d %s\n",strlen(errmsg), errmsg);
      ttext("%d %s\n",strlen(experrmsg), experrmsg);
      return 1;
    }
  }
}

// test #1
// state pointer is a NULL
// should exit and display an error
int top_left_pixel_test_1() {
  // setup fork
  int fdn[2];
  pipe(fdn);

  if (fork() == 0) {

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
    PixelatorState* state = NULL;
    get_top_left_pixel(state);
    close(fdn[1]);
    exit(0);
  } else {
    // parent process
    close(fdn[1]);
    tmsg("get_top_left_pixel_test_1");
    int wstat;
    wait( &wstat );
    char errmsg[105];
    char experrmsg[105] = KBLD KRED "Error: State in pixelator_init is NULL. Use the pixelator.c/pixelator_init function first.\n" KNRM;
    read(fdn[0], errmsg, 104);
    close(fdn[0]);
    errmsg[104] = 0;
    if( strcmp(errmsg,experrmsg) == 0 ) {
      tpass("\n");
      return 0;
    } else { 
      tfail("message did not match expected.\n");
      ttext("%d %s\n",strlen(errmsg), errmsg);
      ttext("%d %s\n",strlen(experrmsg), experrmsg);
      return 1;
    }
  }
}

// test #2
// state pointer is a NULL
// should exit and display an error
int top_left_pixel_test_2(PixelatorState state) {
  // setup fork
  int fdn[2];
  pipe(fdn);

  if (fork() == 0) {

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
    state.wand = NULL;
    get_top_left_pixel(&state);
    close(fdn[1]);
    exit(0);
  } else {
    // parent process
    close(fdn[1]);
    tmsg("get_top_left_pixel_test_2");
    int wstat;
    wait( &wstat );
    char errmsg[99];
    char experrmsg[99] = KBLD KRED "Error: Wand in pixelator_init is NULL. Use the image.c/prepare_image function first.\n" KNRM;
    read(fdn[0], errmsg, 98);
    close(fdn[0]);
    errmsg[98] = 0;
    if( strcmp(errmsg,experrmsg) == 0 ) {
      tpass("\n");
      return 0;
    } else { 
      tfail("message did not match expected.\n");
      ttext("%d %s\n",strlen(errmsg), errmsg);
      ttext("%d %s\n",strlen(experrmsg), experrmsg);
      return 1;
    }
  }
}

// test #3
// image with no dark pixels
int top_left_pixel_test_3(HTPewPewOpts opts) {
  int retval = 1;
  opts.infile = "tests/white.jpg";
  MagickWand* wand = prepare_image(opts);
  PixelatorState* state = pixelator_init(opts,wand);
  Pixel* px = get_top_left_pixel(state);
  tmsg("get_top_left_pixel_test_3");
  // check pixel value, remember these are centered
  if( px->x == 245 && px->y == 245 ) {
    tpass("\n");
    retval = 0;
  } else {
    tfail("Actual Pixel value = (%d,%d)\n", px->x, px->y);
  }
  return retval;
}

// test #4
// normal image
int top_left_pixel_test_4(PixelatorState state) {
  int retval = 1;
  Pixel* px = get_top_left_pixel(&state);
  tmsg("get_top_left_pixel_test_4");
  // check pixel value
  if( px->x == 250 && px->y == 245 ) {
    tpass("\n");
    retval = 0;
  } else {
    tfail("Actual Pixel value = (%d,%d)\n", px->x, px->y);
  }
  return retval;
}

// test #1
// state pointer is a NULL
// should exit and display an error
int bottom_right_pixel_test_1() {
  // setup fork
  int fdn[2];
  pipe(fdn);

  if (fork() == 0) {

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
    PixelatorState* state = NULL;
    get_bottom_right_pixel(state);
    close(fdn[1]);
    exit(0);
  } else {
    // parent process
    close(fdn[1]);
    tmsg("get_bottom_right_pixel_test_1");
    int wstat;
    wait( &wstat );
    char errmsg[105];
    char experrmsg[105] = KBLD KRED "Error: State in pixelator_init is NULL. Use the pixelator.c/pixelator_init function first.\n" KNRM;
    read(fdn[0], errmsg, 104);
    close(fdn[0]);
    errmsg[104] = 0;
    if( strcmp(errmsg,experrmsg) == 0 ) {
      tpass("\n");
      return 0;
    } else { 
      tfail("message did not match expected.\n");
      ttext("%d %s\n",strlen(errmsg), errmsg);
      ttext("%d %s\n",strlen(experrmsg), experrmsg);
      return 1;
    }
  }
}

// test #2
// state pointer is a NULL
// should exit and display an error
int bottom_right_pixel_test_2(PixelatorState state) {
  // setup fork
  int fdn[2];
  pipe(fdn);

  if (fork() == 0) {

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
    state.wand = NULL;
    get_bottom_right_pixel(&state);
    close(fdn[1]);
    exit(0);
  } else {
    // parent process
    close(fdn[1]);
    tmsg("get_bottom_right_pixel_test_2");
    int wstat;
    wait( &wstat );
    char errmsg[99];
    char experrmsg[99] = KBLD KRED "Error: Wand in pixelator_init is NULL. Use the image.c/prepare_image function first.\n" KNRM;
    read(fdn[0], errmsg, 98);
    close(fdn[0]);
    errmsg[98] = 0;
    if( strcmp(errmsg,experrmsg) == 0 ) {
      tpass("\n");
      return 0;
    } else { 
      tfail("message did not match expected.\n");
      ttext("%d %s\n",strlen(errmsg), errmsg);
      ttext("%d %s\n",strlen(experrmsg), experrmsg);
      return 1;
    }
  }
}

// test #3
// image with no dark pixels
int bottom_right_pixel_test_3(HTPewPewOpts opts) {
  int retval = 1;
  opts.infile = "tests/white.jpg";
  MagickWand* wand = prepare_image(opts);
  PixelatorState* state = pixelator_init(opts,wand);
  Pixel* px = get_bottom_right_pixel(state);
  tmsg("get_bottom_right_pixel_test_3");
  // check pixel value, remember these are centered
  if( px->x == 244 && px->y == 244 ) {
    tpass("\n");
    retval = 0;
  } else {
    tfail("Actual Pixel value = (%d,%d)\n", px->x, px->y);
  }
  return retval;
}

// test #4
// normal image
int bottom_right_pixel_test_4(PixelatorState state) {
  int retval = 1;
  Pixel* px = get_bottom_right_pixel(&state);
  tmsg("get_bottom_right_pixel_test_4");
  // check pixel value
  if( px->x == 239 && px->y == 244 ) {
    tpass("\n");
    retval = 0;
  } else {
    tfail("Actual Pixel value = (%d,%d)\n", px->x, px->y);
  }
  return retval;
}

// test #1
// state pointer is a NULL
// should exit and display an error
int get_next_pixel_test_1() {
  // setup fork
  int fdn[2];
  pipe(fdn);

  if (fork() == 0) {

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
    PixelatorState* state = NULL;
    get_next_pixel(state);
    close(fdn[1]);
    exit(0);
  } else {
    // parent process
    close(fdn[1]);
    tmsg("get_next_pixel_test_1");
    int wstat;
    wait( &wstat );
    char errmsg[105];
    char experrmsg[105] = KBLD KRED "Error: State in pixelator_init is NULL. Use the pixelator.c/pixelator_init function first.\n" KNRM;
    read(fdn[0], errmsg, 104);
    close(fdn[0]);
    errmsg[104] = 0;
    if( strcmp(errmsg,experrmsg) == 0 ) {
      tpass("\n");
      return 0;
    } else { 
      tfail("message did not match expected.\n");
      ttext("%d %s\n",strlen(errmsg), errmsg);
      ttext("%d %s\n",strlen(experrmsg), experrmsg);
      return 1;
    }
  }
}

// test #2
// wand` pointer is a NULL
// should exit and display an error
int get_next_pixel_test_2(PixelatorState state) {
  // setup fork
  int fdn[2];
  pipe(fdn);

  if (fork() == 0) {

    // stdout >> pipe >> strcmp
    dup2(fdn[1], STDERR_FILENO);
    state.wand = NULL;
    get_next_pixel(&state);
    close(fdn[1]);
    exit(0);
  } else {
    // parent process
    close(fdn[1]);
    tmsg("get_next_pixel_test_2");
    int wstat;
    wait( &wstat );
    char errmsg[99];
    char experrmsg[99] = KBLD KRED "Error: Wand in pixelator_init is NULL. Use the image.c/prepare_image function first.\n" KNRM;
    read(fdn[0], errmsg, 98);
    close(fdn[0]);
    errmsg[98] = 0;
    if( strcmp(errmsg,experrmsg) == 0 ) {
      tpass("\n");
      return 0;
    } else { 
      tfail("message did not match expected.\n");
      ttext("%d %s\n",strlen(errmsg), errmsg);
      ttext("%d %s\n",strlen(experrmsg), experrmsg);
      return 1;
    }
  }
}

// test #3
// pixel pointer is NULL
// should allocate a pixel
int get_next_pixel_test_3(PixelatorState state) {
  int retval = 1;
  state.px = NULL;
  Pixel* px = get_next_pixel(&state);
  tmsg("get_next_pixel_test_3");
  // check pixel value, remember these are centered
  if( px == NULL ) {
    tpass("\n");
    retval = 0;
  } else {
    tfail("Actual Pixel value = (%d,%d)\n", px->x, px->y);
  }
  return retval;
}

// test #4
// pixel iterator pointer is NULL
// should allocate an iterator
int get_next_pixel_test_4(PixelatorState state) {
  int retval = 1;
  state.it = NULL;
  Pixel* px = get_next_pixel(&state);
  tmsg("get_next_pixel_test_4");
  // check pixel value, remember these are centered
  if( px == NULL ) {
    tpass("\n");
    retval = 0;
  } else {
    tfail("Actual Pixel value = (%d,%d)\n", px->x, px->y);
  }
  return retval;
}

// test #5
// normal image
int get_next_pixel_test_5(PixelatorState state) {
  int retval = 1;
  Pixel* px = get_next_pixel(&state);
  tmsg("get_next_pixel_test_5");
  // check pixel value
  if( px->x == 239 && px->y == 244 ) {
    tpass("\n");
    retval = 0;
  } else {
    tfail("Actual Pixel value = (%d,%d)\n", px->x, px->y);
  }
  return retval;
}

// test #6
// image with no dark pixels
int get_next_pixel_test_6(HTPewPewOpts opts) {
  int retval = 1;
  opts.infile = "tests/white.jpg";
  MagickWand* wand = prepare_image(opts);
  PixelatorState* state = pixelator_init(opts,wand);
  Pixel* px = get_next_pixel(state);
  tmsg("get_next_pixel_test_6");
  // check pixel value, remember these are centered
  if( px == NULL ) {
    tpass("\n");
    retval = 0;
  } else {
    tfail("Actual Pixel value = (%d,%d)\n", px->x, px->y);
  }
  return retval;
}


int pixelator_tests() {
  int retval = 0;

  // defaults for this group
  // use this (or any image)
  HTPewPewOpts opts;
  opts.infile    = "tests/greyscale.png";
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
  retval |= get_pixel_intensity_test_1(state);
  retval |= get_pixel_intensity_test_2(state);
  retval |= get_pixel_intensity_test_3(state);
  retval |= get_pixel_intensity_test_4();
  retval |= pixelator_init_test_1(opts);
  retval |= top_left_pixel_test_1();
  retval |= top_left_pixel_test_2(state);
  retval |= top_left_pixel_test_3(opts);
  retval |= top_left_pixel_test_4(state);
  retval |= bottom_right_pixel_test_1();
  retval |= bottom_right_pixel_test_2(state);
  retval |= bottom_right_pixel_test_3(opts);
  retval |= bottom_right_pixel_test_4(state);
  retval |= get_next_pixel_test_1();
  retval |= get_next_pixel_test_2(state);
  //retval |= get_next_pixel_test_3(state);
  //retval |= get_next_pixel_test_4(state);
  //retval |= get_next_pixel_test_5(state);
  retval |= get_next_pixel_test_6(opts);

  // do cleanup
  wand = DestroyMagickWand(wand);
  return(retval);
}
