#include "game_engine.h"
#include "Board_Files/board.h"
#include "Deck/Deck.h"
/*
* Game class function definitions
*/
Game::Game()
{
    this->id = num_games;
    num_games += 1;
    // TODO: Implement construction of new Player_Manager instance
    this->player_manager = new Player_Manager();
    board = new Board();
    deck = new Deck();
}
Game::~Game()
{
    num_games -= 1;
}
int Game::add_player(int player_id)
{
    // TODO: Implement this function
    int ret = this->player_manager->add_player(player_id);
    return ret;
}
int Game::start_game()
{
    // TODO: Implement this function
    return 0;
}
int Game::end_game()
{
    // TODO: Implement this function
    return 0;
}
int Game::make_move(int player_id, int move_option)
{
    //I am assuming the move_option will map exactly to the 
    //number system the board uses for indexing locations
    Player* p = player_manager->getPlayer(player_id);
    board->movePiece(p->getCharacter(), move_option);
    
    return 0;
}
int Game::get_game_id()
{
    return this->id;
}
int Game::num_games = 0;


/*
*   Player Manager function definitions
*/

Player_Manager::Player_Manager()
{
    num_players = 0;
}
int Player_Manager::add_player(int player_id)
{
    if (num_players >= 6)
    {
        // Too many players in game
        return 0;
    }
    num_players += 1;

    Player* player = new Player(player_id, num_players, characters[num_players]);
    player_list.push_back(player);

    // TODO: Implement the rest of this function

    return 1;
}

Player* Player_Manager::getPlayer(int id){
    return player_list[id];
}


/*
*   Player function definitions
*/

Player::Player(int player_id_, int position_, std::string character_)
{
    // hand should be initialized at game start, not here
    
    this->player_id = player_id_;
    this->position = position_; // Will be provided from the Player_Manager object
    this->selected_character = character_;
}

std::string Player::getCharacter()
{
    return selected_character;
}