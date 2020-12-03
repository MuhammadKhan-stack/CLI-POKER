#pragma once
/** 
 * This class represents a deck object which stores and distributes card structs
 */
struct card {
	int suit;
	int number;
};
class Deck {
private:
	card* deck[52];
	int cardsInDeck = 52;
public:
	void createDeck();
	void shuffleDeck();
	void displayDeck();
	card* getTopCard();
};
