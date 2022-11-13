#include "game_engine.h"


/*
* Game class function definitions
*/
Game::Game()
{
    this->id = num_games;
    num_games += 1;
    // TODO: Implement construction of new Player_Manager instance
    this->player_manager = new Player_Manager();
    // TODO: Implement construction of new Board instance
    // TODO: Implement construction of new Deck instance

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
int Game::make_move(int move_option)
{
    // TODO: Implement this function
    // This will be called by the server code
    // Functionality will be to interface with the board object to check validity of move,
    // And then make the move (And do all of the subsequent things this entails)
    // (This will be a very big function)
    // return is 1 if move was made 0 if requested move is invalid
    return 0;
}
int Game::get_game_id()
{
    return this->id;
}
int Game::num_games = 0;

Game::cardSetup() {
	Card suspect_cards[6]
	for (SUSPECT s = SUSPECT::Begin; s != SUSPECT::END; ++s) {
		suspect_cards[s] =

	}
	Card weapon_cards[6]
	Card room_cards[9]


}


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

    Player* player = new Player(player_id, num_players);
    player_list.push_back(player);

    // TODO: Implement the rest of this function

    return 1;
}


/*
*   Player function definitions
*/

Player::Player(int player_id_, int position_)
{
    // hand should be initialized at game start, not here
    
    this->player_id = player_id_;
    this->position = position_; // Will be provided from the Player_Manager object
}

/*
*   Board function definitions
*/

Board::Board()
{
    // TODO: This should initialize all 21 locations
}

/*
*   Location function definitions
*/

/*
*   Deck function definitions
*/