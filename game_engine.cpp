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

	Deck deck;
	Card* culprit_deck = deck.GenerateCulpritCardDeck();
	Card* non_culprit_deck = deck.GenerateNonCulpritCardDeck(culprit_deck);
	Card* shuffled_non_culprit_deck = deck.ShuffleDeck(non_culprit_deck);

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
    printf("Move requested player %d to location %d\n", player_id, move_option);
    Player* p = player_manager->getPlayer(player_id);
    return board->movePiece(p->getCharacter(), move_option);
    
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
    player_list[player_id] = player;

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