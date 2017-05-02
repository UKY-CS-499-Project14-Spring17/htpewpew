
// include the test helpers and the cli
#include "cli.h"
#include "tests/helpers.h"

// include each file from test/ dir
#include "tests/image.c"
#include "tests/streamer.c"

int main() {
  set_silent();
  int results = 0;
  results |= image_tests();
  results |= streamer_tests();
  return results;
}
