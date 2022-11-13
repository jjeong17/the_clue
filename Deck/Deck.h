//
// Created by Jung Woo Jeong on 13.11.22.
//

#ifndef THE_CLUE_DECK_H
#define THE_CLUE_DECK_H

#include <iostream>

const int num_rooms = 9;
const int num_suspects = 6;
const int num_weapons = 6;

//enum ROOM
//{
//	HALL = "HALL",
//	LOUNGE = "LOUNGE",
//	DINING_ROOM = "DINING",
//	KITCHEN = "KITCHEN",
//	BALLROOM = "BALLROOM",
//	CONSERVATORY = "CONSERVATORY",
//	BILLIARD_ROOM = "BILLIARD_ROOM",
//	LIBRARY = "LIBRARY",
//	STUDY = "STUDY"
//};
//
//enum SUSPECT
//{
//	COLONEL_MUSTARD = "COLONEL_MUSTARD",
//	PROFESSOR_PLUM = "PROFESSOR_PLUM",
//	MR_GREEN = "MR_GREEN",
//	MRS_PEACOCK = "MRS_PEACOCK",
//	MISS_SCARLETT = "MISS_SCARLETT",
//	MRS_WHITE = "MRS_WHITE",
//};
//
//enum WEAPON
//{
//	DAGGER = "DAGGER",
//	CANDLESTICK = "CANDLESTICK",
//	REVOLVER = "REVOLVER",
//	ROPE = "ROPE",
//	LEAD_PIPE = "LEAD_PIPE",
//	SPANNER = "SPANNER",
//};

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
public:
	Deck();
	Card* GenerateCulpritCardDeck();
	Card* GenerateNonCulpritCardDeck(Card* culprit_deck);
	void ShuffleDeck(Card non_culprit_cards);
};

#endif //THE_CLUE_DECK_H
