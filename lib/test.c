
// include the test helpers and the cli
#include "cli.h"
#include "tests/helpers.h"

// include each file from test/ dir
// overrides in tests/streamer_test_overrides.h break these pixelator tests
#include "tests/image.c"
#include "tests/pixelator.c"
#include "tests/streamer.c"
#include "tests/main.c"
#include "tests/cli.c"

int main() {
  //set_silent();
  int results = 0;
  results |= image_tests();
  results |= main_tests();
  results |= streamer_tests();
  results |= cli_tests();
  results |= pixelator_tests();
  return results;
}
