#include "controls.h"

void main() {
  while (true) {
    int controls_finished = Control_update();
    if (controls_finished) {
      dir = UP;
      Control_setDir(dir);
      }
    Control_update();
  }
}
