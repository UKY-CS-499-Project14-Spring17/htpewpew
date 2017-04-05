
// include each file from test/ dir
#include "tests/image.c"

int main() {
  int results = 0;
  results = results | image_tests();
  return results;
}

