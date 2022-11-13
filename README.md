# The Clue
- Johns Hopkins University Foundation of Software Engineering Project

Compilation:

gcc -c -o server.o server.c
g++ -c -o server_to_game_interface.o server_to_game_interface.cpp
g++ -c -o game_engine.o game_engine.cpp

g++ -o final server.o server_to_game_interface.o game_engine.o




## Deck Class

Init function will trigger followings in order:
1) Randomly pick one from each category of cards and create an "Envelop/Cabinet" out of it. (Location, Tool, Player)
2) Distribute remaining cards randomly to each players. NOTE. No need to be distributed equally



## Initialization

1) Each cards get initiated:
    a. 6 Suspects
    b. 6 Weapons
    c. 9 Rooms
