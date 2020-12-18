cmake_minimum_required(VERSION 3.9)
project(libmld_loader)

set (CMAKE_CXX_STANDARD 14)

include_directories(../../dependencies/include/)
include_directories(../../dependencies/include/opencv4/)

link_directories(../../dependencies/lib/)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
add_library(mld_loader SHARED ./src/Circle.cpp
                              ./src/Ellipse.cpp
                              ./src/Layer.cpp
                              ./src/Line.cpp
                              ./src/MLDFile.cpp
                              ./src/MLDLoader.cpp
                              ./src/MLDReader.cpp
                              ./src/MLDSerializer.cpp
                              ./src/Object.cpp
                              ./src/Origin.cpp
                              ./src/Point.cpp
                              ./src/Polygon.cpp
                              ./src/PolyLine.cpp
                              ./src/Rectangle.cpp
                              ./src/Shape.cpp
                              ./src/Square.cpp
                              ./src/Text.cpp)

target_include_directories(mld_loader PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(mld_loader openslide png opencv_core opencv_imgcodecs opencv_imgproc)
