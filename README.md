# The Clue
- Johns Hopkins University Foundation of Software Engineering Project

Compilation:

gcc -c -o server.o server.c
g++ -c -o server_to_game_interface.o server_to_game_interface.cpp
g++ -c -o game_engine.o game_engine.cpp

g++ -o final server.o server_to_game_interface.o game_engine.o -lpthread


## Deck Class
1) Generate culprit deck - 1 Room, 1 Suspect, 1 Weapon card
2) Generate shuffled non-culprit deck (8 Room, 5 Suspect, 5 Weapon Cards) : This will be distributed to players

Use cases of above are documented in `main_test.cpp` file in a form of testing.

### Running a test script
```
$ cd Deck && sh run_test.sh
```




