#include <vector>
#include <map>
#include <string>
#include "Board_Files/board.h"
#include "Deck/Deck.h"

class Game;
class Player_Manager;
class Player;
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

    std::map<int, std::string> loc_to_string = {
		{0, "STUDY"},
		{2, "HALL"},
		{4, "LOUNGE"},
		{8, "LIBRARY"},
		{10, "BILLIARD_ROOM"},
		{12, "DINING_ROOM"},
		{16, "CONSERVATORY"},
		{18, "BALLROOM"},
		{20, "KITCHEN"}
	};

    std::map<int, std::string> weapon_to_string = {
        {0, "DAGGER"},
        {1, "CANDLESTICK"},
        {2, "REVOLVER"},
        {3, "ROPE"},
        {4, "LEAD_PIPE"},
        {5, "SPANNER"}
    };

public:
    Game();
    ~Game();
    
    int add_player(int);
    int start_game();
    void see_hand(int);
    int end_game();
    int make_move(int, int);
    int make_suggestion(int, int, int, int);
    bool correctAccusation(int, int, int);
    int make_accusation(int player_id1, int player_id2, int weapon_id, int location);
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
    void start_game(Deck*);
    void suggestion_check(std::string, std::string, std::string, std::string);
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
    std::vector<Card> hand;
public:
    Player();
    Player(int, int, std::string);
    bool hasCard(std::string);
    void addCard(Card);
    void print_hand();
    std::string getCharacter();

    ~Player();
};
