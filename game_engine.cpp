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

}
Game::~Game()
{
    num_games -= 1;
}
int Game::add_player(int player_id)
{
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
    player_manager->start_game(deck);
    board->printBoard();
    
    return 0;
}
int Game::end_game()
{
    board->printBoard();
    return 0;
}

int* Game::get_player_locations()
{
    int* character_locations = board->getPlayerLocations();
    return character_locations;
}

void Game::see_hand(int player_id){
    player_manager->getPlayer(player_id)->print_hand();
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
    if(!board->suggestionMove(p1->getCharacter(), p2->getCharacter(), location)){
        std::cout << "INVALID SUGGESTION" << std::endl;
        return 0;
    }
    std::string accused_weapon = weapon_to_string[weapon_id];
    std::string accused_location = loc_to_string[location];
    player_manager->suggestion_check(p1->getCharacter(), p2->getCharacter(), accused_weapon, accused_location);

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
    
    if(deck->accusation_match(accused_name, accused_weapon, accused_location)){
        std::cout << player_manager->getPlayer(player_id1)->getCharacter() << " wins!" << std::endl;
    }else{
        std::cout << "Accusation Incorrect" << std::endl;
        invalid_players.push_back(player_id1);
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

void Player_Manager::start_game(Deck* game_deck){
    int cards_per_player = 18 / num_players;
    std::map<int, Player*>::iterator it;
    int index = 0;
    for(it = player_list.begin(); it != player_list.end(); it++){
        for(int j = 0; j < cards_per_player; j++){
            it->second->addCard(game_deck->getCard());
        }
    }
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

    return 1;
}

void Player_Manager::suggestion_check(std::string p1, std::string p2, std::string weapon, std::string loc){
    std::map<int, Player*>::iterator it;
    
    for(it = player_list.begin(); it != player_list.end(); it++){
        if(it->second->getCharacter() == p1){
            break;
        }
    }
    it++;
    for(int i = 0; i < num_players-1; i++){
        if(it == player_list.end()){
            it = player_list.begin();
        }
        if(it->second->hasCard(p2)){
            std::cout << "from player " << it->second->getCharacter() << "\n";
            std::cout << "\t" << p2 << std::endl;
            return;
        }else if(it->second->hasCard(weapon)){
            std::cout << "from player " << it->second->getCharacter() << "\n";
            std::cout << "\t" << weapon << std::endl;
            return;
        }else if(it->second->hasCard(loc)){
            std::cout << "from player " << it->second->getCharacter() << "\n";
            std::cout << "\t" << loc << std::endl;
            return;
        }
        it++;
    }
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

void Player::addCard(Card a){
    hand.push_back(a);
}

bool Player::hasCard(std::string a){
    for(int i = 0; i < hand.size(); i++){
        if(hand[i].get_name() == a){
            return true;
        }
    }
    return false;
}

void Player::print_hand(){
    for(int i = 0; i < hand.size(); i++){

        std::cout << "\t" << hand[i].get_name() << "\n";
    }
}