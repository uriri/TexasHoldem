/*
 * Deck.h
 *
 *  Created on: 2016/11/22
 *      Author: haga
 */

#ifndef DECK_H_
#define DECK_H_

#include <vector>
#include <memory>

namespace game {

class Card;

class Deck {
private:
	std::vector< std::unique_ptr<Card> > m_deck;
	int m_index;
public:
	Deck();
	virtual ~Deck();

	void showALL() const;
	void show() const;

	void shuffle();

	std::unique_ptr<Card> getTop();
	std::vector< std::unique_ptr<Card> > getTops(int n);
};

#if 0
	const vector<Card> NoPair = {
			Card(10, Suit::CLUB), Card(8, Suit::DIAMOND), Card(6, Suit::DIAMOND), Card(4, Suit::HEART), Card(2, Suit::SPADE)
	};
	const vector<Card> OnePair = {
			Card(5, Suit::CLUB), Card(5, Suit::DIAMOND), Card(9, Suit::HEART), Card(7, Suit::CLUB), Card(8, Suit::DIAMOND)
	};
	const vector<Card> TwoPair = {
			Card(10, Suit::CLUB), Card(10, Suit::DIAMOND), Card(6, Suit::HEART), Card(7, Suit::CLUB), Card(7, Suit::DIAMOND)
	};
	const vector<Card> ThreeOfAKind = {
			Card(10, Suit::CLUB), Card(10, Suit::DIAMOND), Card(10, Suit::HEART), Card(1, Suit::CLUB), Card(7, Suit::DIAMOND)
	};
	const vector<Card> Straight = {
			Card(2, Suit::SPADE), Card(3, Suit::CLUB), Card(4, Suit::HEART), Card(5, Suit::SPADE), Card(6, Suit::HEART)
	};
	const vector<Card> Flush = {
			Card(2, Suit::DIAMOND), Card(4, Suit::DIAMOND), Card(6, Suit::DIAMOND), Card(9, Suit::DIAMOND), Card(10, Suit::DIAMOND)
	};
	const vector<Card> FullHouse = {
			Card(10, Suit::CLUB), Card(10, Suit::DIAMOND), Card(10, Suit::HEART), Card(7, Suit::CLUB), Card(7, Suit::DIAMOND)
	};
	const vector<Card> FourOfAKind = {
			Card(9, Suit::CLUB), Card(9, Suit::DIAMOND), Card(9, Suit::HEART), Card(9, Suit::SPADE), Card(7, Suit::CLUB)
	};
	const vector<Card> StraightFlush = {
			Card(6, Suit::CLUB), Card(7, Suit::CLUB), Card(8, Suit::CLUB), Card(9, Suit::CLUB), Card(10, Suit::CLUB)
	};
	const vector<Card> full1 = {
			Card(5, Suit::SPADE), Card(5, Suit::HEART), Card(5, Suit::DIAMOND), Card(12, Suit::HEART), Card(12, Suit::CLUB)
	};
	const vector<Card> full2 = {
			Card(5, Suit::SPADE), Card(5, Suit::HEART), Card(5, Suit::DIAMOND), Card(11, Suit::HEART), Card(11, Suit::CLUB)
	};
	const vector<Card> Straight3 = {
			Card(2, Suit::SPADE), Card(11, Suit::CLUB), Card(12, Suit::HEART), Card(13, Suit::SPADE), Card(1, Suit::HEART)
	};
//	"Royal Straight Flush"
#endif

} /* namespace game */

#endif /* DECK_H_ */
