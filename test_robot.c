#include "inc/controls2.h"

void main() {
  while (true) {
    int controls_finished = Controls2_update();
    if (controls_finished) {
      dir = UP;
      Controls2_setDir(dir);
      }
    Controls2_update();
  }
}
