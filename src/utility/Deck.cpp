/*
 * Deck.cpp
 *
 *  Created on: 2017/02/02
 *      Author: haga
 */

#include "Deck.h"
#include "Card.h"

#include <iostream>
#include <random>
#include <algorithm>

namespace game{

Deck::Deck():m_index(0) {
	m_deck.reserve(52);
	for(unsigned int j=1; j<14; ++j){
		m_deck.push_back(std::make_unique<Card>(j, Suit::CLUB));
		m_deck.push_back(std::make_unique<Card>(j, Suit::DIAMOND));
		m_deck.push_back(std::make_unique<Card>(j, Suit::HEART));
		m_deck.push_back(std::make_unique<Card>(j, Suit::SPADE));
	}
	//	m_deck[i] = Card(Suit::JOKER);
	this->shuffle();
};

Deck::~Deck() = default;

void Deck::showALL() const {
	std::cout << "Deck : { ";
	for(const auto& c : m_deck){
		std::cout << *c << " ";
	}
	std::cout << "}" << std::endl;
}

void Deck::show() const {
	const int s = m_deck.size();
	std::cout << "{ ";
	for(int i=m_index; i<s; ++i ){
		std::cout << *m_deck[i] << " ";
	}
	std::cout << "}" << std::endl;
}

void Deck::shuffle(){
	std::random_device rnd;
	auto seed = rnd();
//		auto seed = 95962896;
	std::cout << "Deck seed : " << seed << std::endl;
	std::mt19937 mt(seed);
	std::shuffle(std::begin(m_deck)+m_index, std::end(m_deck), mt);
}

std::unique_ptr<Card> Deck::getTop(){ return std::move(m_deck[m_index++]); }

std::vector<std::unique_ptr<Card>> Deck::getTops(int n){
	std::vector<std::unique_ptr<Card>> tmp;
	tmp.insert(
			tmp.end(),
			std::make_move_iterator(m_deck.begin())+m_index,
			std::make_move_iterator(m_deck.begin())+m_index+n
	);
	m_index += n;
	return tmp;
}

}/* namespace game */
