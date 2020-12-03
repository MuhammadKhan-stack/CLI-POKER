#pragma once
#include "Deck.h"
/** 
 * This class represents a single player in a poker game
 * It holds all the data related to said player and the actions to interact with them
 */
class Player {
private:
	card* hand[5];
	int wallet = 100;
	int cardsInHand = 0;
	int isPlaying = 0;
	int playerNumber = 0;
public:
	void setWallet(int);
	void setPlayerNumber(int number);
	int getPlayerNumber();
	int isFirst = 0;
	void setIsPlaying(int);
	int getIsPlaying();
	int getWallet();
	void winMoney(int);
	void receiveCardFromDealer(card*);
	void swapCard(card*, int);
	void makeBet(int);
	void displayHand();
	void displayWallet();
	int getCardValue(int);
	int getCardSuit(int);
};