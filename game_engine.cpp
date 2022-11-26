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
    this->player_manager = new Player_Manager();
    board = new Board();
	deck = new Deck();

	// Logging Deck Status
//	std::cout << "Starting Deck Setup" << std::endl;
//	std::cout << "[Culprit_deck]" << std::endl;
//	for (int i = 0; i < num_culprit_cards; i++) {
//		std::cout << culprit_deck[i].getName() << " " << culprit_deck[i].getType() <<  " | ";
//	}
//	std::cout << "\n===================" << std::endl;
//	std::cout << "[Non-Culprit_deck]" << std::endl;
//	for (int i = 0; i < num_non_culprit_cards; i++) {
//		std::cout << shuffled_non_culprit_deck[i].getName() << " ";
//	}
//	std::cout << "\n===================" << std::endl;
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

bool Game::validPlayer(int player_id){
    for(int i = 0; i < invalid_players.size(); ++i){
        if(invalid_players[i] == player_id){
            return false;
        }
    }
    return true;
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
    int ret_code;
    if(!validPlayer(player_id)){
        std::cout << "Player is eliminated" << std::endl;
        return false;
    }
    
    printf("Move requested player %d to location %d\n", player_id, move_option);
    Player* p = player_manager->getPlayer(player_id);
    ret_code = board->movePiece(p->getCharacter(), move_option);
    board->printBoard();
    return ret_code;
}

int Game::make_suggestion(int player_id1, int player_id2, int weapon_id, int location){
    int ret_code;
    if(!validPlayer(player_id1)){
        std::cout << "Player is eliminated" << std::endl;
        return false;
    }
    Player* p1 = player_manager->getPlayer(player_id1);
    Player* p2 = player_manager->getPlayer(player_id2);
    ret_code = board->suggestionMove(p1->getCharacter(), p2->getCharacter(), location);
    board->printBoard();
    return ret_code;
}

int Game::make_accusation(int player_id1, int player_id2, int weapon_id, int location){
    if(!validPlayer(player_id1)){
        std::cout << player_manager->getPlayer(player_id1)->getCharacter() << " eliminated" << std::endl;
        return false;
    }
    std::string accused_name = player_manager->getPlayer(player_id2)->getCharacter();
    std::string accused_weapon = weapon_to_string[weapon_id];
    std::string accused_location = loc_to_string[location];

    std::cout << "Our accusation" << std::endl;
    std::cout << "name = " << accused_name << std::endl;
    std::cout << "weapon = " << accused_weapon << std::endl;
    std::cout << "location = " << accused_location << std::endl;
    
    if(deck->accusationMatch(accused_name, accused_weapon, accused_location)){
        std::cout << player_manager->getPlayer(player_id1)->getCharacter() << " wins!" << std::endl;
    }else{
        std::cout << "Accusation Incorrect" << std::endl;
        std::cout << player_manager->getPlayer(player_id1)->getCharacter() << " eliminated" << std::endl;
        return false;
    }
    return true;
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

    Player* player = new Player(player_id, num_players, characters[num_players]);
    player_list[player_id] = player;

    num_players += 1;
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