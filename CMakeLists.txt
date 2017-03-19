
cmake_minimum_required(VERSION 3.6)
project(simpleBlobDetection)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(OpenCV_DIR "C:\\OpenCV\\build_MinGW\\install")
find_package(OpenCV REQUIRED COMPONENTS features2d imgproc highgui core )

include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(opencv main.cpp)
set(OpenCV_LIBS opencv_features2d opencv_imgproc opencv_highgui opencv_imgcodecs opencv_videoio opencv_core)
target_link_libraries(opencv ${OpenCV_LIBS})



