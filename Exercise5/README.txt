This directory contains source code that might be helpful when solving
exercise 5. We provide both C++ and Python versions of this code.

The only file that needs editing to create a working system
is exercise5.cc or exercise5.py, but it is recommended to have a brief look at the rest
of the source code. In particular it is recommended to look at particle.[cc|h|py]
to see how particles are represented.

The files exercise5.cc and exercise5.py was created by simply removing parts of a 
functioning program and replacing these parts by lines saying:
  //XXX: You do this
So look for 'XXX' to see where you should concentrate your efforts.

The folder also contain two auxillary C++ programs:
checkerboard_generator - Generates a camera calibration pattern image
camera_calibrator - Based on images of a checkerboard test pattern the intrinsic camera parameters are calculated.

The main C++ project for exercise5 can be build using the make command. Just type
make

Alternatively, CMake can be used like this:
mkdir build
cd build/
cmake ..
make

This auto-generates makefile's or you can use the generator facility of cmake to generate project files for your favourite IDE. Using CMake is the only way to build the auxillary programs.

If you're having problems understanding the code, don't hesitate to contact me.

Kim Steenstrup Pedersen, september 9, 2015.
Søren Hauberg, august 21st, 2006.
