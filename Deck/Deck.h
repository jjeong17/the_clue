//
// Created by Jung Woo Jeong on 13.11.22.
//

#ifndef THE_CLUE_DECK_H
#define THE_CLUE_DECK_H

#include <iostream>
#include <vector>

const int num_rooms = 9;
const int num_suspects = 6;
const int num_weapons = 6;

const int num_culprit_cards = 3;
const int num_non_culprit_cards = 18;

const std::string room_array[num_rooms] = {
		"HALL",
		"LOUNGE",
		"DINING_ROOM",
		"KITCHEN",
		"BALLROOM",
		"CONSERVATORY",
		"BILLIARD_ROOM",
		"LIBRARY",
		"STUDY"
};

const std::string suspect_array[num_suspects] = {
		"MISS_SCARLETT",
		"PROFESSOR_PLUM",
		"COLONEL_MUSTARD",
		"MRS_PEACOCK",
		"MR_GREEN",
		"MRS_WHITE"
};

const std::string weapon_array[num_weapons] = {
		"DAGGER",
		"CANDLESTICK",
		"REVOLVER",
		"ROPE",
		"LEAD_PIPE",
		"SPANNER",
};

enum CardType
{
	ROOM = 0,
	SUSPECT = 1,
	WEAPON = 2
};

class Card {
private:
	int m_id;
	CardType m_type;
	std::string m_name;
	bool m_is_owned;

public:
	Card();

	// Getter
	std::string get_name();
	CardType get_type();

	// Setter
	void set_name(std::string name);
	void set_type(CardType type);
	void setIsOwned(bool is_owned);
};

class Deck {
private:
	std::vector<Card> m_culprit_cards;
	std::vector<Card> m_non_culprit_cards;

	std::vector<Card> generate_culprit_card_deck();
	std::vector<Card> generate_non_culprit_card_deck(Card* culprit_deck);
	std::vector<Card> shuffle_deck(Card* non_culprit_cards);
	void set_culprit_cards(Card* cards);
	void set_non_culprit_cards(Card* cards);
public:
	Deck();

	std::vector<Card> get_culprit_cards();
	std::vector<Card> get_non_culprit_cards();
};

#endif //THE_CLUE_DECK_H