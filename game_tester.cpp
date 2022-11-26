#include "Deck/Deck.h"
#include "Board_Files/board.h"
#include <iostream>
#include "game_engine.h"
int main(){

    Game my_game = Game();
    my_game.add_player(1);
    my_game.add_player(2);
    my_game.add_player(3);
    my_game.add_player(4);
    my_game.add_player(5);
    my_game.add_player(6);
    
    my_game.make_accusation(1, 6, 4, 16);

    return 0;
}