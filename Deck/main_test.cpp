//
// Created by Jung Woo Jeong on 13.11.22.
//

#include <iostream>
#include "Deck.h"

int main()
{
	std::cout << "Starting Deck > main_test" << std::endl;

	Deck* deck = new Deck();

	Card* culprit_cards = deck->getCulpritCards();
	Card* non_culprit_cards = deck->getNonCulpritCards();

	std::cout << "[Check culprit_deck]" << std::endl;
	for (int i = 0; i < num_culprit_cards; i++) {
		std::cout << culprit_cards[i].getName() << " " << culprit_cards[i].getType() <<  " | ";
	}

	std::cout << "\n===================" << std::endl;
	std::cout << "[Check Shuffle non_culprit_deck]" << std::endl;
	for (int i = 0; i < num_non_culprit_cards; i++) {
		std::cout << non_culprit_cards[i].getName() << " ";
	}

	return 0;
}
