// blackjackWithClasses.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <array>
#include <ctime>
#include <cstdlib>

class Card {
public:
	enum Ranks {
		RANK_2, //0
		RANK_3, //1
		RANK_4, //2
		RANK_5, //3
		RANK_6, //4
		RANK_7, //5
		RANK_8, //6
		RANK_9, //7
		RANK_10, //8
		RANK_JACK, //9
		RANK_QUEEN, //10
		RANK_KING, //11
		RANK_ACE, //12
		MAX_RANKS //13
	};

	enum Suits {
		SUIT_HEARTS,
		SUIT_CLUBS,
		SUIT_DIAMONDS,
		SUIT_SPADES,
		MAX_SUITS
	};

private:
	Ranks m_rank;
	Suits m_suit;

public:
	Card(Ranks rank=RANK_2, Suits suit=SUIT_HEARTS) : m_rank(rank), m_suit(suit) {
		//default constructor
	}

	Ranks getRank() {
		return m_rank;
	}

	const int getCardValue() {
		if (m_rank < 8) {
			return m_rank + 2;
		}
		else if (m_rank > 7 && m_rank < 12) {
			return 10;
		}
		else {
			return 11;
		}
	}

	const void printCard() {
		switch (m_rank) {
		case RANK_2:
			std::cout << 2;
			break;
		case RANK_3:
			std::cout << 3;
			break;
		case RANK_4:
			std::cout << 4;
			break;
		case RANK_5:
			std::cout << 5;
			break;
		case RANK_6:
			std::cout << 6;
			break;
		case RANK_7:
			std::cout << 7;
			break;
		case RANK_8:
			std::cout << 8;
			break;
		case RANK_9:
			std::cout << 9;
			break;
		case RANK_10:
			std::cout << 10;
			break;
		case RANK_JACK:
			std::cout << "J";
			break;
		case RANK_QUEEN:
			std::cout << "Q";
			break;
		case RANK_KING:
			std::cout << "K";
			break;
		case RANK_ACE:
			std::cout << "A";
			break;
		}
		switch (m_suit) {
		case SUIT_HEARTS:
			std::cout << "H";
			break;
		case SUIT_CLUBS:
			std::cout << "C";
			break;
		case SUIT_DIAMONDS:
			std::cout << "D";
			break;
		case SUIT_SPADES:
			std::cout << "S";
			break;
		}
	}
};

class Deck {
	std::array<Card, 52> m_deck;
	int m_cardIndex = 0;

public:
	Deck() {
		int rank = 0;
		int suit = 0;
		for (int index = 0; index < 52; index++) {
			m_deck[index] = Card(static_cast<Card::Ranks>(rank), static_cast<Card::Suits>(suit));
			rank++;
			if (rank == Card::MAX_RANKS) {
				rank = 0;
				suit++;
			}
		}
	}

	void printDeck() {
		for (Card card : m_deck) {
			card.printCard();
			std::cout << "\t";
			if (card.getRank() == 12) std::cout << "\n";
		}
	}

private:
	static void cardSwap(Card &cardA, Card &cardB) {
		Card temp = cardA;
		cardA = cardB;
		cardB = temp;
	}

public:
	void shuffleDeck() {
		for (int index = 0; index < 52; index++) {
			m_cardIndex = 0;
			static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
			int randCard = static_cast<int>(rand() * fraction * (52));
			cardSwap(m_deck[index], m_deck[randCard]);
		}
	}

	Card dealCard() {
		return m_deck[m_cardIndex++];
	}
};

void aceCounter(int cardValue, int &playerScore, int &aceCount) {
	playerScore += cardValue;
	if (cardValue == 11) {
		aceCount++;
	}
}

//the following two lines are forward declarations with prototypes of the next two functions that call each other
void replay(Deck deck);
void blackJack(Deck deck);

void replay(Deck deck) {
	std::cout << "Would you like to play again? (y/n) ";
	char again;
	std::cin >> again;
	if (again == 'y') {
		deck.shuffleDeck();
		blackJack(deck);
	}
}

void blackJack(Deck deck) {
	//auto *cardPtr = &deck[0];
	int dealer = 0;
	int dealerAces = 0;
	int player = 0;
	int playerAces = 0;
	aceCounter(deck.dealCard().getCardValue(), dealer, dealerAces);
	//dealer += deck.dealCard().getCardValue();
	std::cout << "The dealer's score is: " << dealer << std::endl;
	aceCounter(deck.dealCard().getCardValue(), player, playerAces);
	aceCounter(deck.dealCard().getCardValue(), player, playerAces);
	if (player > 21 && playerAces > 0) {
		player = player - 10 * playerAces--;
		std::cout << "One ace converted to \"1\" to keep the score within limits." << std::endl;
	}
	//player += deck.dealCard().getCardValue();
	//player += deck.dealCard().getCardValue();
	while (player < 21) {
		std::cout << "Your score is: " << player << " Would you like to draw another card? (y/n)" << std::endl;
		char hit;
		std::cin >> hit;
		if (hit == 'y') {
			aceCounter(deck.dealCard().getCardValue(), player, playerAces);
			if (player > 21 && playerAces > 0) {
				player = player - 10 * playerAces--;
				std::cout << "One ace converted to \"1\" to keep the score within limits." << std::endl;
			}
		}
		else {
			break;
		}
	}
	if (player > 21) {
		std::cout << "You went bust with " << player << "!" << std::endl;
		replay(deck);
		return;
	}
	else {
		while (dealer < 17) {
			aceCounter(deck.dealCard().getCardValue(), dealer, dealerAces);
			if (dealer > 17 && dealerAces > 0) {
				dealer = dealer - 10 * dealerAces--;
				std::cout << "Dealer converts an ace to \"1\" to keep playing." << std::endl;
			}
			/*if (dealer > 21 && dealerAces > 0) {
				dealer = dealer - 10 * dealerAces--;
				std::cout << "Dealer converts an ace to \"1\" to keep the score within limits." << std::endl;
			}*/
		}
		if (dealer > 21) {
			std::cout << "The dealer went bust with " << dealer << "! You win!" << std::endl;
			replay(deck);
			return;
		}
	}
	std::cout << "The dealer's score is " << dealer << std::endl;
	std::cout << "Your score is " << player << std::endl;
	if (player > dealer) {
		std::cout << "You win!" << std::endl;
	}
	else {
		std::cout << "Sorry, you lose. Better luck next time." << std::endl;
	}
	replay(deck);
	return;
}

int main()
{
	srand(static_cast<unsigned int>(time(0))); // set initial seed value to system clock
	rand(); // If using Visual Studio, discard first random value

	Deck deck;
	deck.shuffleDeck();
	blackJack(deck);

	return 0;
}

