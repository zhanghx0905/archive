#include <stdlib.h>

int main() {
  srand(time(NULL));
  int t = 0;
  for (int i = 0; i < 100; i++) {
    if (rand() % 2 == 0) {
      t = 0;
    }
    if (rand() % 2 == 0) {
      t = 1;
    }
    if (rand() % 2 == 0) {
      t = 2;
    }
    if (rand() % 2 == 0) {
      t = 3;
    }
    if (rand() % 2 == 0) {
      t = 4;
    }
  }
}