#include <iostream>
#include <time.h>
#include "Game.h"
#include "Deck.h"
#include "Player.h"

/**
 * This method adds spaces to the terminal
 */
void Game::enterSpaces(){
	for(int i=0; i<10; i++){
		std::cout<<"\n";
	}
}

/**
 * Initialize Players as player 1 and player 2.
 */
void Game::setPlayers(){
	player1.setPlayerNumber(1);
	player2.setPlayerNumber(2);
}

//Method to begin new game.
//Includes creating a deck of 52 cards, shuffling them, setting the initial player and adding contributions to the pot.
//Set the status of both players as playing.
void Game::newGame() {
	deck.createDeck();
	deck.shuffleDeck();
	player1.setIsPlaying(1);
	player2.setIsPlaying(1);
	addToPot(&player1, 10);
	addToPot(&player2, 10);
	setInitialPlayer();
}

//Method to check which player starts.
void Game::setInitialPlayer() {
	if (player1.isFirst == 0) {
		player1.isFirst = 1;
		player2.isFirst = 0;
	}
	else {
		player1.isFirst = 0;
		player2.isFirst = 1;
	}
}

//Method to initiate rounds of betting.
//If one player hasnt been offered the chance to bet, the other bets and then moves to the next player.
void Game::roundOfBetting() {
	int noBets;
	if (player1.isFirst) {
		noBets = betsValue(&player1);
		if (noBets)
			betsValue(&player2);
	}
	else {
		noBets = betsValue(&player2);
		if (noBets)
			betsValue(&player1);
	}
}

/**
 * Adds the a players bet to the pot
 * @param The player making the bet, the amount being bet
 */
void Game::addToPot(Player* player, int amount) {
	player->makeBet(amount);
	pot += amount;
}

//Deal 5 cards to each player.
//Depending on who is the initial player, cards are distributed.
void Game::dealHands() {
	for (int i = 0; i < 5; i++) {
		if (player1.isFirst){
			player1.receiveCardFromDealer(deck.getTopCard());
			player2.receiveCardFromDealer(deck.getTopCard());
		}
		else {
			player2.receiveCardFromDealer(deck.getTopCard());
			player1.receiveCardFromDealer(deck.getTopCard());
		}
	}
}
/**
 * Ask players for the next step. Would they continue to play or exit the table.
 * @param the human player that will be asked
 * @return the persons decision
 */ 
int Game::betsValue(Player* player) {
	int userInput = -1;
	if (player1.getWallet() == 0 || player2.getWallet() == 0) {
		std::cout << "\nBetting will not be conducted this round. Insufficient funds.\n\n";
		return 0;
	}
	else {
		while (userInput == -1) {
			std::cout << "\nPlayer " << player->getPlayerNumber() << ": Would you like to bet, fold, or stay?\n\n"
				"Enter '0' for bet, '1' for fold, '2' for stay, or '3' to display hand and wallet.\n\nEnter your anwer here: ";
			std::cin >> userInput;
			std::cin.clear();
			std::cin.ignore('\n', 10);
			//Player wants to bet
			if (userInput == 0) {
				playerBet(player, 0);
				return 0;
			}
			//Player wants to fold
			else if (userInput == 1) {
				player->setIsPlaying(0);
				return 0;
			}
			//Player wants to stay
			else if (userInput == 2) {
				enterSpaces();
				return 1;
			}
			//Display hand
			else if (userInput == 3) {
				player->displayHand();
				player->displayWallet();
				userInput = -1;
			}
			else {
				std::cout << "\nInvalid Input. Enter correct input.\n\n";
				userInput = -1;
			}
		}
	}
}

/**
 * Determines how the bot bets based off of their current hand and money
 * @param player: bot making the bet
 * @param callval: the amount of money needed to match the bet
 * @return the bots decision
 */

int Game::botBet(Player* player, int callVal) {
	if (player1.getWallet() == 0 || player2.getWallet() == 0) {
		std::cout << "\nBetting will not be conducted this round. Insufficient funds.\n\n";
		return 0;
	}
	int betAmount = 0;
	int pricerisk = (callVal / player->getWallet()) * 10;
	int handq = getWinValue(player);
	int handquality = handq + (handq - 10) * 3;
	//All in cases
	if(handquality > 20 || callVal >= player->getWallet()) {
		betAmount = player->getWallet();
	}
	//Fold Cases
	else if(handquality < 0 && pricerisk > 5) {
		player->setIsPlaying(0);
	}
	else if(handquality > 12 ) {
		if(player->getWallet() - callVal > 20) {
			betAmount = callVal + std::min((player->getWallet() - callVal) / 2, 20);
		}
		else {
			betAmount = player->getWallet() / 2;
		} 
	}
	else if(handquality > 6 && pricerisk < 5) {
		if(player->getWallet() + callVal > 20) {
			betAmount = std::min(player->getWallet() / 4, 20);
		}
		else {
			betAmount = player->getWallet();
		} 
	}
	//Check case is just to add 0
	addToPot(player, betAmount);
	askPlayerToStay(&player1, betAmount);
	return 0;
}

//Check if a player bet is legal.
void Game::playerBet(Player* player, int previousBet) {
	int betAmount = 0;
	do {
		std::cout << "\nEnter bet amount here: ";
		std::cin >> betAmount;
		std::cin.clear();
		std::cin.ignore('\n', 10);
		//Not all players can afford the bet
		if ((betAmount - previousBet) > player1.getWallet() || (betAmount - previousBet) > player2.getWallet() || betAmount > player->getWallet()) {
				std::cout << "\nPlayers cannot bet this amount.\n\n";
				player1.displayWallet();
				player2.displayWallet();
				betAmount = 0;
		}
		//User does not enter value equivalent to raising the bet
		else if (betAmount <= previousBet) {
			std::cout << "\nEBet must be more than yout previous bet. Previous bet was $" << previousBet << "\n\n";
			betAmount = 0;
		}
		//User enters negative number
		else if (betAmount < 0)
			std::cout << "\nBet must be greater than 0.\n\n";
		
	} while (betAmount < 1);
	addToPot(player, betAmount);
	enterSpaces();
	if (player->getPlayerNumber() == 1)
		botBet(&player2, betAmount - previousBet);
	else
		askPlayerToStay(&player1, betAmount - previousBet);
}

/**
 * Allows the player to respond to an action by the bot and checks if its valid
 * @param The player being asked, the current bet they have to match
 */
void Game::askPlayerToStay(Player* player, int bet) {
	int userInput = -1;
	while (userInput == -1) {
		std::cout << "\nPlayer " << player->getPlayerNumber() << ": Enter '0' to fold, '1' to match the bet, '2' to raise the bet, or '3' to display hand and wallet."
			"\nPlease enter your userInput here: ";
		std::cin >> userInput;
		std::cin.clear();
		std::cin.ignore('\n', 10);
		//Player folds
		if (userInput == 0) {
			player->setIsPlaying(0);
		}
		//Player matches bet
		else if (userInput == 1) {
			if (player->getWallet() < bet) {
				std::cout << "\nCannot match bet as the required funds are not enough.\n\n";
				player->displayWallet();
				userInput = -1;
			}
			else {
				addToPot(player, bet);
				enterSpaces();
			}
		}
		//Player raises bet
		else if (userInput == 2) {
			if (player->getWallet() <= bet) {
				std::cout << "\nERROR : INSUFFICIENT FUNDS\n\n";
				player->displayWallet();
				userInput = -1;
			}
			else {
				playerBet(player, bet);
			}
		}
		else if (userInput == 3) {
			player->displayHand();
			player->displayWallet();
			userInput = -1;
		}
		//Invalid input
		else {
			std::cout << "\nERROR : INVALID ANSWER\n\n";
			userInput = -1;
		}
	}
}

/**
 * Checks to see if theres still a valid player in the game
 * @return whether there is or not
 */
int Game::didPlayersFold() {
	if (player1.getIsPlaying() == 0 || player2.getIsPlaying() == 0){
		return 1;
	}
	return 0;
}

//If both players are active, deal more cards.
void Game::round2OfDealing() {
	if (player1.isFirst) {
		askForNewCards(&player1);
		askForNewCards(&player2);
	}
	else {
		askForNewCards(&player2);
		askForNewCards(&player1);
	}
}

/**
 * Allows the player to swap cards from the deck
 * @param the player swapping
 */
void Game::askForNewCards(Player* player) {
	int userInput = -1;
	enterSpaces();
	while (userInput == -1) {
		std::cout << "\nPlayer " << player->getPlayerNumber() << ": Enter '0' to swap 0 cards, '1' to swap 1 card, "
			"'2' to swap 2 cards,\n'3' to swap 3 cards, or '4' to display hand and wallet\n\nPlease enter your userInput here: ";
		std::cin >> userInput;
		std::cin.clear();
		std::cin.ignore('\n', 10);
		if (userInput == 1)
			swapCards(player, 1);
		else if (userInput == 2)
			swapCards(player, 2);
		else if (userInput == 3)
			swapCards(player, 3);
		else if (userInput == 4) {
			player->displayHand();
			player->displayWallet();
			userInput = -1;
		}
		else if (userInput != 0) {
			std::cout << "\nPlease enter a valid input.\n\n";
			userInput = -1;
		}
	}
}
/**
 * The legword for the ask for new cards method. Swaps the cards specified with the deck
 * @param player: the player swapping
 * @param numberOfCards: the number of cards
 */
void Game::swapCards(Player* player, int numberOfCards) {
	int timesRan = 0;
	int card1 = 0;
	int card2 = 0;
	int card3 = 0;
	int userInput = -1;
	while (timesRan < numberOfCards) {
		if (card1 == 0) {
			while (userInput == -1) {
				player->displayHand();
				std::cout << "\nEnter the card number you would like to discard here, or enter 0 to display hand and wallet: ";
				std::cin >> userInput;
				std::cin.clear();
				std::cin.ignore('\n', 10);
				if (userInput >= 1 && userInput <= 5)
					card1 = userInput;
				else if (userInput == 0) {
					player->displayHand();
					player->displayWallet();
					userInput = -1;
				}
				else {
					std::cout << "\nInvalid Input.\n\n";
					userInput = -1;
				}
			}
			++timesRan;
			userInput = -1;
		}
		else {
			while (userInput == -1) {
				player->displayHand();
				std::cout << "Enter another card number you would like to discard here: ";
				std::cin >> userInput;
				std::cin.clear();
				std::cin.ignore('\n', 10);
				if (userInput >= 1 && userInput <= 5 && card1 != userInput && card2 != userInput) {
					if (card2 == 0)
						card2 = userInput;
					else
						card3 = userInput;
				}
				else {
					std::cout << "\nERROR : INVALID ANSWER\n\n";
					userInput = -1;
				}
			}
			++timesRan;
			userInput = -1;
		}
	}
	player->swapCard(deck.getTopCard(), card1);
	if (card2 != 0)
		player->swapCard(deck.getTopCard(), card2);
	if (card3 != 0)
		player->swapCard(deck.getTopCard(), card3);
}

//If both players are still playing after this runs, it is a tie.
void Game::compareHands() {
	int p1win = getWinValue(&player1);
	int p2win = getWinValue(&player2);
	if (p1win > p2win)
		player2.setIsPlaying(0);
	else if (p2win > p1win) 
		player1.setIsPlaying(0);
	else {
		p1win = getHighValue(&player1);
		p2win = getHighValue(&player2);
		if (p1win > p2win)
			player2.setIsPlaying(0);
		else if (p2win > p1win)
			player1.setIsPlaying(0);
	}
}

/**
 * Rates the value of the players hand
 * @param player, the player whos hand is being rated
 * @return the integer score of their hand
 */
int Game::getWinValue(Player* player) {
	int winValue = 0;
	int card1Val = player->getCardValue(1);
	int card1Suit = player->getCardSuit(1);
	int card2Val = player->getCardValue(2);
	int card2Suit = player->getCardSuit(2);
	int card3Val = player->getCardValue(3);
	int card3Suit = player->getCardSuit(3);
	int card4Val = player->getCardValue(4);
	int card4Suit = player->getCardSuit(4);
	int card5Val = player->getCardValue(5);
	int card5Suit = player->getCardSuit(5);

	putInNumberOrder(&card1Val, &card2Val, &card3Val, &card4Val, &card5Val);
	
	//Check for Flush(18), then for Straight Flush (21)
	if (card1Suit == card2Suit && card2Suit == card3Suit && card3Suit == card4Suit && card4Suit == card5Suit) {
		winValue = 18;
		if ((card1Val + 1) == card2Val && (card2Val + 1) == card3Val && (card3Val + 1) == card4Val && (card4Val + 1) == card5Val)
			winValue = 21;
	}

	//Check for Two of a kind(14), then Two pairs(15), then Three of a kind(16), then Full house(19), then Four of a kind(20)
	//Find pair
	else if (card1Val == card2Val) {
		if (card2Val == card3Val) {
			if (card4Val == card5Val)
				winValue = 19;
			else if (card3Val == card4Val)
				winValue = 20;
			else
				winValue = 16;
		}
		else if (card3Val == card4Val) {
			if (card4Val == card5Val)
				winValue = 19;
			else
				winValue = 15;
		}
		else if (card4Val == card5Val)
			winValue = 15;
		else
			winValue = 14;
	}
	else if (card2Val == card3Val) {
		if (card3Val == card4Val) {
			if (card4Val == card5Val)
				winValue = 20;
			else
				winValue = 16;
		}
		else if (card4Val == card5Val)
			winValue = 15;
		else
			winValue = 14;
	}
	else if (card3Val == card4Val) {
		if (card4Val == card5Val)
			winValue = 16;
		else
			winValue = 14;
	}
	else if (card4Val == card5Val)
		winValue = 14;
	
	//Check for Straight (17)
	else if ((card1Val + 1) == card2Val && (card2Val + 1) == card3Val && (card3Val + 1) == card4Val && (card4Val + 1) == card5Val)
		winValue = 17;

	//Check for High Card (0 = Ace, King = 12)
	else
		winValue = card5Val;

	return winValue;
}

/**
 * Sorts the values into ascending order
 * @param val1 various integer values to be sorted
 * @param val2 various integer values to be sorted
 * @param val3 various integer values to be sorted
 * @param val4 various integer values to be sorted
 * @param val5 various integer values to be sorted
 */
void Game::putInNumberOrder(int* val1, int* val2, int* val3, int* val4, int* val5) {
	//While numbers are not sorted
	int temp;
	while (!(val1[0] <= val2[0]) || !(val2[0] <= val3[0]) || !(val3[0] <= val4[0]) || !(val4[0] <= val5[0])) {
		if (!(val1[0] <= val2[0])) {
			temp = val1[0];
			val1[0] = val2[0];
			val2[0] = temp;
		}
		if (!(val2[0] <= val3[0])) {
			temp = val2[0];
			val2[0] = val3[0];
			val3[0] = temp;
		}
		if (!(val3[0] <= val4[0])) {
			temp = val3[0];
			val3[0] = val4[0];
			val4[0] = temp;
		}
		if (!(val4[0] <= val5[0])) {
			temp = val4[0];
			val4[0] = val5[0];
			val5[0] = temp;
		}
	}
}

/**
 * Gets the highest value among the players cards
 * @param person: the player
 * @return the highest value
 */
int Game::getHighValue(Player* person) {
	int highValue = person->getCardValue(1);
	if (highValue < person->getCardValue(2))
		highValue = person->getCardValue(2);
	if (highValue < person->getCardValue(3))
		highValue = person->getCardValue(3);
	if (highValue < person->getCardValue(4))
		highValue = person->getCardValue(4);
	if (highValue < person->getCardValue(5))
		highValue = person->getCardValue(5);
	return highValue;
}

/**
 * Shows the winnings of the ahdn
 */
void Game::rewardWinnings() {
	if (player1.getIsPlaying() == 0) {
		player2.winMoney(pot);
		std::cout << "Player 2 wins!\n\n";
		player1.displayWallet();
		player2.displayWallet();
		pot = 0;
	}
	else if (player2.getIsPlaying() == 0) {
		player1.winMoney(pot);
		std::cout << "Player 1 wins!\n\n";
		player1.displayWallet();
		player2.displayWallet();
		pot = 0;
	}
	else {
		std::cout << "Its was a draw!\n\n";
		player1.winMoney(pot / 2);
		player2.winMoney(pot / 2);
		pot = 0;
	}
}
/**
 * Returns an integer value if one of the players lost
 * @return lost flag
 */
int Game::gameOver() {
	if (player1.getWallet() < 10 || player2.getWallet() < 10)
		return 1;
	return 0;
}