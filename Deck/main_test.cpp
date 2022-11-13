//
// Created by Jung Woo Jeong on 13.11.22.
//

#include <iostream>
#include "Deck.h"

int main()
{
	std::cout << "Starting main_test" << std::endl;
	std::cout << "Check culprit_deck" << std::endl;
	Deck deck;
	Card* culprit_deck = deck.GenerateCulpritCardDeck();
	for (int i = 0; i < 3; i++) {
		std::cout << culprit_deck[i].getName() << std::endl;
		std::cout << culprit_deck[i].getType() << std::endl;
	}

	// Check non_culprit_deck
	std::cout << "Check non_culprit_deck" << std::endl;
	Card* non_culprit_deck = deck.GenerateNonCulpritCardDeck(culprit_deck);
	for (int i = 0; i < 18; i++) {
		std::cout << non_culprit_deck[i].getName() << std::endl;
	}

	return 0;

}
