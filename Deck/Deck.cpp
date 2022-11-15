//
// Created by Jung Woo Jeong on 13.11.22.
//

#include "Deck.h"
#include <iostream>

Deck::Deck() {
	Card* culprit_cards = this->generateCulpritCardDeck();
	Card* non_culprit_cards = this->generateNonCulpritCardDeck(culprit_cards);
	Card* shuffled_non_culprit_cards = this->shuffleDeck(non_culprit_cards);
	setCulpritCards(culprit_cards);
	setNonCulpritCards(shuffled_non_culprit_cards);
}

void Deck::setCulpritCards(Card* culprit_cards) {
	this->culprit_cards = culprit_cards;
}

void Deck::setNonCulpritCards(Card* non_culprit_cards) {
	this->non_culprit_cards = non_culprit_cards;
}

Card* Deck::getCulpritCards() {
	return this->culprit_cards;
};

Card* Deck::getNonCulpritCards() {
	return this->non_culprit_cards;
}

Card* Deck::generateCulpritCardDeck()
{
	int random_room_idx = (rand()%9);
	int random_suspect_idx = (rand()%6);
	int random_weapon_idx = (rand()%6);

	CardType card_type_room = ROOM;
	CardType card_type_suspect = SUSPECT;
	CardType card_type_weapon = WEAPON;

	Card culprit_room_card;
	Card culprit_suspect_card;
	Card culprit_weapon_card;

	culprit_room_card.setType(card_type_room);
	culprit_suspect_card.setType(card_type_suspect);
	culprit_weapon_card.setType(card_type_weapon);

	culprit_room_card.setName(room_array[random_room_idx]);
	culprit_suspect_card.setName(suspect_array[random_suspect_idx]);
	culprit_weapon_card.setName(weapon_array[random_weapon_idx]);

	culprit_room_card.setIsOwned(false);
	culprit_suspect_card.setIsOwned(false);
	culprit_weapon_card.setIsOwned(false);

	Card* culprit_cards = new Card[3];
	culprit_cards[0] = culprit_room_card;
	culprit_cards[1] = culprit_suspect_card;
	culprit_cards[2] = culprit_weapon_card;
	return culprit_cards;
}


Card* Deck::generateNonCulpritCardDeck(Card* culprit_deck) {
	Card* culprit_room_card = &culprit_deck[0];
	Card* culprit_suspect_card = &culprit_deck[1];
	Card* culprit_weapon_card = &culprit_deck[2];

//	std::cout << culprit_room_card->getName() << " " << culprit_suspect_card->getName() << " " << culprit_weapon_card->getName() << std::endl;

	Card* non_culprit_card = new Card[18];

	// Room
	int loop_idx = 0;
	for (int i = 0; i < num_rooms; i++) {
		if (room_array[i] != culprit_room_card->getName()) {
			Card non_culprit_room;
			non_culprit_room.setName(room_array[i]);
			non_culprit_room.setType(ROOM);
			non_culprit_room.setIsOwned(true);
			non_culprit_card[loop_idx++] = non_culprit_room;
		}
	}

	// Suspect
	for (int i = 0; i < num_suspects; i++) {
		if (suspect_array[i] != culprit_suspect_card->getName()) {
			Card non_culprit_suspect;
			non_culprit_suspect.setName(suspect_array[i]);
			non_culprit_suspect.setType(SUSPECT);
			non_culprit_suspect.setIsOwned(true);
			non_culprit_card[loop_idx++] = non_culprit_suspect;
		}
	}

	// Weapon
	for (int i = 0; i < num_weapons; i++) {
		if (weapon_array[i] != culprit_weapon_card->getName()) {
			Card non_culprit_weapon;
			non_culprit_weapon.setName(weapon_array[i]);
			non_culprit_weapon.setType(SUSPECT);
			non_culprit_weapon.setIsOwned(true);
			non_culprit_card[loop_idx++] = non_culprit_weapon;
		}
	}

	return non_culprit_card;
}

Card* Deck::shuffleDeck(Card* non_culprit_cards) {
	Card temp_card = non_culprit_cards[0];
	int random_idx = 0;

	for (int i = 0; i < num_non_culprit_cards; i++) {
		random_idx = rand() % num_non_culprit_cards;
		temp_card = non_culprit_cards[i];
		non_culprit_cards[i] = non_culprit_cards[random_idx];
		non_culprit_cards[random_idx] = temp_card;
	}

	return non_culprit_cards;
}

Card::Card() {}

std::string Card::getName() {
	return this->name;
}

CardType Card::getType() {
	return this->type;
}

void Card::setName(std::string name) {
	this->name = name;
}

void Card::setType(CardType type) {
	this->type = type;
}

void Card::setIsOwned(bool is_owned) {
	this->Is_Owned = is_owned;
}
