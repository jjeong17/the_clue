#!/bin/sh
clear
gcc -c -o server.o server.c
g++ -c -o server_to_game_interface.o server_to_game_interface.cpp
g++ -c -o game_engine.o game_engine.cpp
g++ -c -o BoardObj.o Board_Files/BoardObj.cpp
g++ -c -o board.o Board_Files/board.cpp
g++ -c -o Deck.o Deck/Deck.cpp
g++ -o final board.o BoardObj.o Deck.o server.o server_to_game_interface.o game_engine.o -lpthread
./final