# The Clue
- Johns Hopkins University Foundation of Software Engineering Project

Compilation:

gcc -c -o server.o server.c
g++ -c -o server_to_game_interface.o server_to_game_interface.cpp
g++ -c -o game_engine.o game_engine.cpp

g++ -o final server.o server_to_game_interface.o game_engine.o
