#include <vector>
#include <string>

class Game;
class Player_Manager;
class Player;
class Board;
class Location;
class Deck;
class Card;

class Game 
{
private:
    static int num_games; // Counter to store the number of active games
    int id; // The unique id of this game
    Player_Manager* player_manager; // The player manager for this game instance
    Board* board; // The board for this game instance
    Deck* deck; // The deck for this game instance

public:
    Game();
    ~Game();

    int add_player(Player* player)
    {
        // TODO: Implement this function
        int ret = player_manager.add_player(player);
        return ret;
    }
    int start_game()
    {
        // TODO: Implement this function
        return 0;
    }
    int end_game()
    {
        // TODO: Implement this function
        return 0;
    }
    int make_move(int move_option)
    {
        // TODO: Implement this function
        // This will be called by the server code
        // Functionality will be to interface with the board object to check validity of move,
        // And then make the move (And do all of the subsequent things this entails)
        // (This will be a very big function)
        // return is 1 if move was made 0 if requested move is invalid
    }
}
Game::Game()
{
    this->id = num_games;
    num_games += 1;
    // TODO: Implement construction of new Player_Manager instance
    // TODO: Implement construction of new Board instance
    // TODO: Implement construction of new Deck instance
}
Game::~Game()
{
    num_games -= 1;
}
Game::num_games = 0;


class Player_Manager
{
private:
    int num_players;
    std::vector<Player*> player_list;
    std::string available_characters[6] = {"Colonel Mustard", "Mrs White", "Professor Plum", "Mrs. Peacock", "Mr. Green", "Ms. Scarlett"};
public:
    Player_Manager();
    ~Player_Manager();
    int add_player(int player_id)
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
    // TODO: Implement several other functions in here
}
Player_Manager::Player_Manager()
{
    num_players = 0;
}

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
    ~Player();
}
Player::Player(int player_id_, int position_)
{
    // hand should be initialized at game start, not here
    
    this->player_id = player_id_;
    this->position = position_; // Will be provided from the Player_Manager object
}

class Board
{
private:
    Location* locations[26];
public:
    Board();
    ~Board();
    // TODO: Lots of functions here
}
Board::Board()
{
    // TODO: This should initialize all 21 locations
}

class Location
{
private:
    int index;
public:
    Location();
    ~Location();
}

class Start_Square : Location
{

}

class Room : Location
{

}

class Deck
{

}

class Card
{

}