//
// Created by Jung Woo Jeong on 13.11.22.
//

#ifndef THE_CLUE_DECK_H
#define THE_CLUE_DECK_H

#include <iostream>

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
		"COLONEL_MUSTARD",
		"PROFESSOR_PLUM",
		"MR_GREEN",
		"MRS_PEACOCK",
		"MISS_SCARLETT",
		"MRS_WHITE",
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
	int id;
	CardType type;
	std::string name;
	bool Is_Owned;

public:
	Card();

	// Getter
	std::string getName();
	CardType getType();

	// Setter
	void setName(std::string name);
	void setType(CardType type);
	void setIsOwned(bool is_owned);
};

class Deck {
	Card* culprit_cards;
	Card* non_culprit_cards;
private:
	Card* generateCulpritCardDeck();
	Card* generateNonCulpritCardDeck(Card* culprit_deck);
	Card* shuffleDeck(Card* non_culprit_cards);
	void setCulpritCards(Card* cards);
	void setNonCulpritCards(Card* cards);
public:
	Deck();

	Card* getCulpritCards();
	Card* getNonCulpritCards();
};

#endif //THE_CLUE_DECK_H
