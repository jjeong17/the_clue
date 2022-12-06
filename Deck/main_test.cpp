//
// Created by Jung Woo Jeong on 13.11.22.
//

#include <iostream>
#include "Deck.h"

int main()
{
	std::cout << "Starting Deck > main_test" << std::endl;
	std::cout << "[Check culprit_deck]" << std::endl;
	Deck deck;
	Card* culprit_deck = deck.GenerateCulpritCardDeck();
	for (int i = 0; i < num_culprit_cards; i++) {
		std::cout << culprit_deck[i].getName() << " " << culprit_deck[i].getType() <<  " | ";
	}

	// Check non_culprit_deck
	std::cout << "\n[Check non_culprit_deck]" << std::endl;
	Card* non_culprit_deck = deck.GenerateNonCulpritCardDeck(culprit_deck);
	for (int i = 0; i < num_non_culprit_cards; i++) {
		std::cout << non_culprit_deck[i].getName() << " ";
	}
	std::cout << "\n===================" << std::endl;

	std::cout << "[Check Shuffle non_culprit_deck]" << std::endl;
	Card* shuffled_non_culprit_deck = deck.ShuffleDeck(non_culprit_deck);
	for (int i = 0; i < num_non_culprit_cards; i++) {
		std::cout << shuffled_non_culprit_deck[i].getName() << " ";
	}
	std::cout << "\n===================" << std::endl;

	return 0;
}
