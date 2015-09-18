#include "locator.hpp"

int main() {
  initHull();

  while (true) {
    vector<Point> *hull = getHull();
    if (hull != NULL) {
      drawHull(*hull);
    }
    
    waitKey(30);
  }

  return 0;
}
