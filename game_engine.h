#include <vector>
#include <map>
#include <string>
#include "Board_Files/board.h"
#include "Deck/Deck.h"

class Game;
class Player_Manager;
class Player;
class Location;
class Card;

class Game
{
private:

    bool validPlayer(int player_id);

    std::vector<int> invalid_players;
    static int num_games; // Counter to store the number of active games
    int id; // The unique id of this game
    Player_Manager* player_manager; // The player manager for this game instance
    Board* board; // The board for this game instance
    Deck* deck; // The deck for this game instance
public:
    Game();
    ~Game();
    
    int add_player(int);
    int start_game();
    int end_game();
    int make_move(int, int);
    int make_suggestion(int player_id1, int player_id2, int location);
    int make_accusation(int player_id1, int player_id2, int weapon_id, int location)
    int get_game_id();
};

class Player_Manager
{
private:
    int num_players;
    std::map<int, Player*> player_list;
    std::map<int, std::string> player_to_character;
    std::string characters[6] = {"MISS_SCARLETT", "PROFESSOR_PLUM",
    "COLONEL_MUSTARD", "MRS_PEACOCK", "MR_GREEN", "MRS_WHITE"};
    
public:
    Player_Manager();
    ~Player_Manager();
    int add_player(int);
    Player* getPlayer(int);
};

class Player
{
private:
    int player_id; // This will be assigned by the server
    int position; // Position at imaginary "table" -- to determine who goes in what order
    std::string selected_character;
    std::string username;
    std::vector<Card*> hand;
public:
    Player();
    Player(int, int, std::string);
    std::string getCharacter();
    ~Player();
};

//I think this location object may be useful to map the UI 
//indexing to the board indexing if they differ
class Location
{
private:
    int index;
public:
    Location();
    ~Location();
};