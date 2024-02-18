cmake_minimum_required(VERSION 3.14)
project(Chess)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  PositionTests
  Position/PositionTests.cpp
  Position/Position.cpp
  Position/PositionFenParser.cpp
  Position/PositionAccessing.cpp
  Utils.cpp
)

target_link_libraries(
  PositionTests
  GTest::gtest_main
)

add_executable(
  MoveGeneratorTests
  MoveGenerator/MoveGeneratorTests.cpp
  MoveGenerator/MoveGenerator.cpp
  MoveGenerator/MoveGeneratorPawns.cpp
  MoveGenerator/MoveGeneratorKnights.cpp
  MoveGenerator/MoveGeneratorMagic.cpp
  MoveGenerator/MoveGeneratorSliders.cpp
  MoveGenerator/MoveGeneratorKing.cpp
  Position/Position.cpp
  Position/PositionFenParser.cpp
  Position/PositionAccessing.cpp
  Utils.cpp
)

target_link_libraries(
  MoveGeneratorTests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(PositionTests)
gtest_discover_tests(MoveGeneratorTests)