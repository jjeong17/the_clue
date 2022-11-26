//
// Created by Jung Woo Jeong on 13.11.22.
//

#include "Deck.h"
#include <iostream>
#include <vector>

Deck::Deck() {
	
	std::vector<Card> culprit_cards = this->generate_culprit_card_deck();
	std::vector<Card> = this->generate_non_culprit_card_deck(culprit_cards);
	std::vector<Card> shuffled_non_culprit_cards = this->shuffle_deck(non_culprit_cards);
	set_culprit_cards(culprit_cards);
	set_non_culprit_cards(shuffled_non_culprit_cards);
}

void Deck::set_culprit_cards(Card* culprit_cards) {
	this->m_culprit_cards = culprit_cards;
}

void Deck::set_non_culprit_cards(Card* non_culprit_cards) {
	this->m_non_culprit_cards = non_culprit_cards;
}

std::vector<Card> Deck::get_culprit_cards() {
	return this->m_culprit_cards;
};

std::vector<Card> Deck::get_non_culprit_cards() {
	return this->m_non_culprit_cards;
}

std::vector<Card> Deck::generate_culprit_card_deck()
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

	culprit_room_card.set_type(card_type_room);
	culprit_suspect_card.set_type(card_type_suspect);
	culprit_weapon_card.set_type(card_type_weapon);

	culprit_room_card.set_name(room_array[random_room_idx]);
	culprit_suspect_card.set_name(suspect_array[random_suspect_idx]);
	culprit_weapon_card.set_name(weapon_array[random_weapon_idx]);

	culprit_room_card.setIsOwned(false);
	culprit_suspect_card.setIsOwned(false);
	culprit_weapon_card.setIsOwned(false);

	Card* culprit_cards = new Card[3];
	culprit_cards[0] = culprit_room_card;
	culprit_cards[1] = culprit_suspect_card;
	culprit_cards[2] = culprit_weapon_card;
	return culprit_cards;
}


std::vector<Card> Deck::generate_non_culprit_card_deck(Card* culprit_deck) {
	Card* culprit_room_card = &culprit_deck[0];
	Card* culprit_suspect_card = &culprit_deck[1];
	Card* culprit_weapon_card = &culprit_deck[2];

//	std::cout << culprit_room_card->get_name() << " " << culprit_suspect_card->get_name() << " " << culprit_weapon_card->get_name() << std::endl;

	Card* non_culprit_card = new Card[18];

	// Room
	int loop_idx = 0;
	for (int i = 0; i < num_rooms; i++) {
		if (room_array[i] != culprit_room_card->get_name()) {
			Card non_culprit_room;
			non_culprit_room.set_name(room_array[i]);
			non_culprit_room.set_type(ROOM);
			non_culprit_room.setIsOwned(true);
			non_culprit_card[loop_idx++] = non_culprit_room;
		}
	}

	// Suspect
	for (int i = 0; i < num_suspects; i++) {
		if (suspect_array[i] != culprit_suspect_card->get_name()) {
			Card non_culprit_suspect;
			non_culprit_suspect.set_name(suspect_array[i]);
			non_culprit_suspect.set_type(SUSPECT);
			non_culprit_suspect.setIsOwned(true);
			non_culprit_card[loop_idx++] = non_culprit_suspect;
		}
	}

	// Weapon
	for (int i = 0; i < num_weapons; i++) {
		if (weapon_array[i] != culprit_weapon_card->get_name()) {
			Card non_culprit_weapon;
			non_culprit_weapon.set_name(weapon_array[i]);
			non_culprit_weapon.set_type(SUSPECT);
			non_culprit_weapon.setIsOwned(true);
			non_culprit_card[loop_idx++] = non_culprit_weapon;
		}
	}

	return non_culprit_card;
}

std::vector<Card> Deck::shuffle_deck(std::vector<Card> non_culprit_cards) {
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

std::string Card::get_name() {
	return this->m_name;
}

CardType Card::get_type() {
	return this->m_type;
}

void Card::set_name(std::string name) {
	this->m_name = name;
}

void Card::set_type(CardType type) {
	this->m_type = type;
}

void Card::setIsOwned(bool is_owned) {
	this->m_is_owned = is_owned;
}