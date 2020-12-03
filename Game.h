#pragma once
#include "Deck.h"
#include "Player.h"

/** 
 * This class represents a single game of poker
 * It simulates through hands until the user wants to leave or the program is closed
 */
class Game {
private:
	
	void addToPot(Player*, int);
	void playerBet(Player*, int);
	void askPlayerToStay(Player*, int);
	void setInitialPlayer();
	int betsValue(Player*);
	int botBet(Player*, int previousBet);
	void askForNewCards(Player*);
	void swapCards(Player*, int);
	int getWinValue(Player*);
	int getHighValue(Player*);

public:
	Deck deck;
	Player player1;
	Player player2;
	int pot = 0;
	void enterSpaces();
	void setPlayers();
	void putInNumberOrder(int*, int*, int*, int*, int*); 
	void newGame();
	void dealHands();
	void roundOfBetting();
	int didPlayersFold();
	void round2OfDealing();
	void compareHands();
	void rewardWinnings();
	int gameOver();
};