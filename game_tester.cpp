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
    my_game.start_game();
    for(int i = 1; i < 7; i++){
        std::cout << "printing hand of player " << i << std::endl;
        my_game.see_hand(i);
    }
    //my_game.make_accusation(1, 6, 4, 16);

    return 0;
}