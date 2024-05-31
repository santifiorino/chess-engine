using SDL2 SDL2_image for GUI

start
```console
santi:~$ git clone https://github.com/santifiorino/chess-engine
santi:~$ cd chess-engine
```

play (so far human vs random moves)
```console
santi:~$ mkdir GUI/build & cd GUI/build
santi:~$ cmake ..
santi:~$ make
santi:~$ ./ChessEngine.exe
```

TODO:
- promoting pice picker GUI
- draw by insufficient material
- fun begins! abstract class for bot and start with the basic ones
    - easy bot vs bot setup so they can play multiple matches and i can monitor which bot is better!