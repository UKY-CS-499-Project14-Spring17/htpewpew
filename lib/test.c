
// include the test helpers and the cli
#include "cli.h"
#include "tests/helpers.h"

// include each file from test/ dir
#include "tests/image.c"
#include "tests/streamer.c"

int main() {
  set_silent();
  int results;
  //results = image_tests() || streamer_tests();
  results = streamer_tests();
  return results;
}
