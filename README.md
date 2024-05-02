using SDL2 SDL2_image for GUI and Gtest for testing
TODO: enviroment variables for link and include paths

start
```console
santi:~$ git clone https://github.com/santifiorino/chess-engine
santi:~$ cd chess-engine
```

build and run tests
```console
santi:~$ cmake -S . -B build
santi:~$ cmake --build build
santi:~$ cd build & ctest
```

play (so far human vs human)
```console
santi:~$ mkdir GUI/build & cd GUI/build
santi:~$ cmake ..
santi:~$ make
santi:~$ ./ChessEngine.exe
```
