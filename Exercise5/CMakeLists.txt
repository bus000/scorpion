CMAKE_MINIMUM_REQUIRED (VERSION 2.8.9)
PROJECT(exercise5 CXX)


#FIND_PACKAGE(OpenCV REQUIRED core highgui imgcodecs)
FIND_PACKAGE(OpenCV REQUIRED)

INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

MESSAGE( STATUS "OpenCV_VERSION = ${OpenCV_VERSION}")
MESSAGE( STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")
MESSAGE( STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")

add_executable(checkerboard_generator checkerboard_generator.cc)
target_link_libraries(checkerboard_generator ${OpenCV_LIBS})

add_executable(camera_calibrator camera_calibrator.cc)
target_link_libraries(camera_calibrator ${OpenCV_LIBS})



SET(SRCS
  camera.cc
  particles.cc
  random_numbers.cc
)

SET(HEADERS
  camera.h
  particles.h
  random_numbers.h
)

SET(DUMMY_MAIN
  exercise5.cc
)

add_executable(exercise5 ${DUMMY_MAIN} ${SRCS} ${HEADERS})
target_link_libraries(exercise5 ${OpenCV_LIBS})

SET(SOLUTION_MAIN
  solutions/solution.cc
)

add_executable(solution ${SOLUTION_MAIN} ${SRCS} ${HEADERS})
target_link_libraries(solution ${OpenCV_LIBS})
SET_PROPERTY(TARGET solution APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}")

# Only works with CMake 3.3 and onwards
#target_include_directories(solution PUBLIC ${PROJECT_SOURCE_DIR})

