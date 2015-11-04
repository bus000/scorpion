#include "../src/camera.hpp"
#include <iostream>

int main() {
   // MACBOOK PRO WEBCAM:
    //camera fx: 1063.255997
    //camera fy: 1071.611217
    //camera cx: 680.881655
    //camera cy: 325.583094

  Camera camera(1063.256, 1071.611, 680.882);

  while(true) {
     camera.measure(true);
  }

  return 0;
}
