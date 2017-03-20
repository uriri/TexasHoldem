/*
 * Card.cpp
 *
 *  Created on: 2017/01/27
 *      Author: haga
 */

#include "Card.h"

#include <random>

namespace game{

int makeRandom(int min, int max){
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> r(min, max);
	return r(mt);
}

std::ostream& operator<<(std::ostream& out, const Card& c){
	switch(c.getSuit()){
	case Suit::JOKER:
		out << "JO";
		return out;
	case Suit::CLUB:
		out << "C";
		break;
	case Suit::DIAMOND:
		out << "D";
		break;
	case Suit::HEART:
		out << "H";
		break;
	case Suit::SPADE:
		out << "S";
		break;
	case Suit::ALL:
		out << "error";
		break;
	}
	switch(c.getNum()){
	case 11:
		out << "J";
		break;
	case 12:
		out << "Q";
		break;
	case 13:
		out << "K";
		break;
	default:
		out << c.getNum();
		break;
	}
	return out;
}

std::ostream& operator<<(std::ostream& out, const Card *c){
	switch(c->getSuit()){
	case Suit::JOKER:
		out << "JO";
		return out;
	case Suit::CLUB:
		out << "C";
		break;
	case Suit::DIAMOND:
		out << "D";
		break;
	case Suit::HEART:
		out << "H";
		break;
	case Suit::SPADE:
		out << "S";
		break;
	case Suit::ALL:
		out << "error";
		break;
	}
	switch(c->getNum()){
	case 11:
		out << "J";
		break;
	case 12:
		out << "Q";
		break;
	case 13:
		out << "K";
		break;
	default:
		out << c->getNum();
		break;
	}
	return out;
}

}
