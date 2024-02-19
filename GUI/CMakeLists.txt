cmake_minimum_required(VERSION 3.14)
project(ChessEngine)

# Set the C++ standard you want to use (change this if you want a different standard)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the directory where SDL2 headers are located
include_directories(D:/msys64/mingw64/include)

# Add the directory where SDL2 libraries are located
link_directories(D:/msys64/mingw64/lib)

# Add an executable target called "MyExecutable" to be built from the source file "main.cpp"
add_executable(
    ChessEngine
    main.cpp
    ../MoveGenerator/MoveGenerator.cpp
    ../MoveGenerator/MoveGeneratorKing.cpp
    ../MoveGenerator/MoveGeneratorKnights.cpp
    ../MoveGenerator/MoveGeneratorMagic.cpp
    ../MoveGenerator/MoveGeneratorPawns.cpp
    ../MoveGenerator/MoveGeneratorSliders.cpp
    ../Position/Position.cpp
    ../Position/PositionAccessing.cpp
    ../Position/PositionFenParser.cpp
    ../Position/PositionMove.cpp
    ../ChessGame.cpp
    ../Utils.cpp
    )

# Link the SDL2 libraries to the executable
target_link_libraries(ChessEngine mingw32 SDL2main SDL2 SDL2_image LIBPNG16)