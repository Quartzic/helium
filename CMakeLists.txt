cmake_minimum_required(VERSION 3.17)
project(helium)

find_package( OpenCV REQUIRED )
find_package( Boost REQUIRED COMPONENTS program_options )
set(CMAKE_CXX_STANDARD 17)

add_executable(helium main.cpp)

include_directories( ${Boost_INCLUDE_DIRS} )
target_link_libraries( helium ${Boost_PROGRAM_OPTIONS_LIBRARY} )
target_link_libraries( helium ${OpenCV_LIBS} )