#include <iostream>
#include "Player.h"
#include "Deck.h"

/**
 * Sets the amount of money in the players wallet
 * @param value the new value
 */
void Player::setWallet(int value) {
	wallet = value;
}

/**
 * Adds a new card to the players hand if they have enough space
 * @param newCard the card being added
 */
void Player::receiveCardFromDealer(card* newCard) {
	if (cardsInHand < 5) {
		hand[cardsInHand] = newCard;
		cardsInHand++;
	}
}

/**
 * Swaps cards with the deck by removing a card and getting a new one
 * @param newCard the card being added
 * @param discardedCardIndex the index of the card to be deleted
 */
void Player::swapCard(card* newCard, int discardedCardIndex) {
	delete hand[discardedCardIndex - 1];
	hand[discardedCardIndex - 1] = newCard;
}

//Prints out the players hand so they can see it
void Player::displayHand() {
	if (isPlaying) {
		std::cout << "Player " << playerNumber << "'s hand:\n";
		for (int i = 0; i < cardsInHand; i++) {
			std::cout << "Card " << i + 1 << ": ";
			//Deck is a Heart
			if (hand[i]->suit == 0) {
				if (hand[i]->number == 0)
					std::cout << "This card is an Ace of Hearts" << std::endl;
				else if (hand[i]->number == 10)
					std::cout << "This card is a Jack of Hearts" << std::endl;
				else if (hand[i]->number == 11)
					std::cout << "This card is a Queen of Hearts" << std::endl;
				else if (hand[i]->number == 12)
					std::cout << "This card is a King of Hearts" << std::endl;
				else
					std::cout << "This card is a " << hand[i]->number + 1 << " of Hearts" << std::endl;
			}
			//Deck is a Diamond
			if (hand[i]->suit == 1) {
				if (hand[i]->number == 0)
					std::cout << "This card is an Ace of Diamonds" << std::endl;
				else if (hand[i]->number == 10)
					std::cout << "This card is a Jack of Diamonds" << std::endl;
				else if (hand[i]->number == 11)
					std::cout << "This card is a Queen of Diamonds" << std::endl;
				else if (hand[i]->number == 12)
					std::cout << "This card is a King of Diamonds" << std::endl;
				else
					std::cout << "This card is a " << hand[i]->number + 1 << " of Diamonds" << std::endl;
			}
			//Deck is a Club
			if (hand[i]->suit == 2) {
				if (hand[i]->number == 0)
					std::cout << "This card is an Ace of Clubs" << std::endl;
				else if (hand[i]->number == 10)
					std::cout << "This card is a Jack of Clubs" << std::endl;
				else if (hand[i]->number == 11)
					std::cout << "This card is a Queen of Clubs" << std::endl;
				else if (hand[i]->number == 12)
					std::cout << "This card is a King of Clubs" << std::endl;
				else
					std::cout << "This card is a " << hand[i]->number + 1 << " of Clubs" << std::endl;
			}
			//Deck is a Spade
			if (hand[i]->suit == 3) {
				if (hand[i]->number == 0)
					std::cout << "This card is an Ace of Spades" << std::endl;
				else if (hand[i]->number == 10)
					std::cout << "This card is a Jack of Spades" << std::endl;
				else if (hand[i]->number == 11)
					std::cout << "This card is a Queen of Spades" << std::endl;
				else if (hand[i]->number == 12)
					std::cout << "This card is a King of Spades" << std::endl;
				else
					std::cout << "This card is a " << hand[i]->number + 1 << " of Spades" << std::endl;
			}
		}
		std::cout << std::endl;
	}
}
/**
 * Simulates the money loss from making a bet
 * @param betAmount the amount of money being bet with
 */
void Player::makeBet(int betAmount) {
	wallet -= betAmount;
}
/**
 * Returns the amount of money left in the wallet
 * @return the amount of money left
 */
int Player::getWallet() {
	return wallet;
}

//Displays the wallet of the player
void Player::displayWallet() {
	std::cout << "Player " << playerNumber << " has $" << wallet << std::endl << std::endl;
}

/**
 * Sets the players number in the turn order
 * @param number their turn number
 */
void Player::setPlayerNumber(int number) {
	playerNumber = number;
}
/**
 * Gets the players number in the turn order
 * @return their turn number
 */
int Player::getPlayerNumber() {
	return playerNumber;
}
/**
 * Sets if the player has folded
 * @param number a binary value representing state
 */
void Player::setIsPlaying(int number) {
	isPlaying = number;
}
/**
 * Gets if the player has folded
 * @return a binary value representing state
 */
int Player::getIsPlaying() {
	return isPlaying;
}
/**
 * adds the players winnings
 * @param winnings the winnings won
 */
void Player::winMoney(int winnings) {
	wallet += winnings;
}
/**
 * gets a card number from the players hand
 * @param cardNumber the index of the card 
 * @return the cards value
 */
int Player::getCardValue(int cardNumber) {
	return hand[cardNumber-1]->number;
}
/**
 * Gets the suit of the card
 * @param cardNumber the index of the card
 * @return the cards suit
 */
int Player::getCardSuit(int cardNumber) {
	return hand[cardNumber-1]->suit;
}