#include "CFork.h"



CFork::CFork() {
  isInUse = false;
}

void CFork::pickUp() {
  isInUse = true;
}

void CFork::putDown() {
  isInUse = false;
}
