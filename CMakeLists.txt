cmake_minimum_required(VERSION 4.1)
project(curtain)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.2
)
FetchContent_MakeAvailable(Catch2)

add_subdirectory(src)

FILE(GLOB Hfiles ./src/*.h)
FILE(GLOB CPPfiles ./src/*.cpp)

add_executable(test_conductor test/main.cpp)
target_link_libraries(test_conductor toollib tinyxml2 Catch2::Catch2WithMain)

add_executable(conductor ${CPPfiles} ${Hfiles})
target_link_libraries(conductor tinyxml2 toollib)