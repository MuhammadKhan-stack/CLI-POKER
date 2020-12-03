#include <iostream>
#include <time.h>
#include "Deck.h"
#include "Player.h"
#include "Game.h"

using namespace std;

/**
 * Runs a game of poker for the player using the Game class
 */
int main() {
	int userInput = -1;
	Game game;
	game.setPlayers();
	while (userInput == -1) {
		cout << "\nWelcome to Poker!\n\nIf you would like to go to the table, enter '0'.\n"
			"\nIf you would like to quit, enter '1'.\n\nPlease enter your answer here: ";
		cin >> userInput;
		cin.clear();
		cin.ignore('\n', 10);
		switch (userInput) {
		case(0):
			game.enterSpaces();
			userInput = -1;
			while (userInput == -1) {
				cout << "\nWelcome to the Table!\n\n";
				game.player1.displayWallet();
				game.player2.displayWallet();
				cout << "If you would like to leave the table, enter '0'.\n"
					"If you would like to play a hand, enter '1'.\n\nPlease enter your answer here: ";
				cin >> userInput;
				cin.clear();
				cin.ignore('\n', 10);
				switch (userInput) {
				case(0):
					game.enterSpaces();
					break;
				case(1):
					if (!game.gameOver()) {
						game.enterSpaces();
						game.newGame();
						game.dealHands();
						game.roundOfBetting();
						if (!game.didPlayersFold()) {
							game.round2OfDealing();
							game.roundOfBetting();
							if (!game.didPlayersFold()) {
								cout << "\nComparing hands!\n";
								game.compareHands();
								cout << "\nDone comparing hands!\n";
							}
						}
						game.rewardWinnings();
					}
					else {
						game.enterSpaces();
						cout << "Insufficient funds for both players. Game cannot continue.\n\n";
					}
					break;
				default:
					game.enterSpaces();
					cout << "\nInvalid Input.\n\n";
					break;
				}
				if (userInput != 0)
					userInput = -1;
			}
			break;
		case(1):
			game.enterSpaces();
			cout << "\nExiting Poker.\n\n";
			break;
		
		default:
			game.enterSpaces();
			cout << "\nInvalid Input.\n\n";
			break;
		}
		if (userInput != 1)
			userInput = -1;
	}
	return 0;
}