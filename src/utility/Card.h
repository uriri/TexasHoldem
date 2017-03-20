/*
 * Card.h
 *
 *  Created on: 2016/11/22
 *      Author: haga
 */

#ifndef CARD_H_
#define CARD_H_

#include <ostream>

namespace game {

enum class Suit{
	JOKER = 0,
	CLUB,
	DIAMOND,
	HEART,
	SPADE,
	ALL,
};

//scoped enumerationは暗黙の型変換なし
//intがいる場合はこれで変換
template<typename T>
constexpr auto toIntegral(T arg) -> typename std::underlying_type<T>::type {
	return static_cast<typename std::underlying_type<T>::type>(arg);
}

extern int makeRandom(int min, int max);

class Card {
private:
	unsigned int m_num;
	Suit m_suit;
public:
	constexpr Card() noexcept:m_num(0U), m_suit(Suit::ALL){};
	constexpr Card(unsigned int n, Suit s) noexcept:m_num(n), m_suit(s){};
	constexpr Card(Suit s) noexcept:m_num(0U), m_suit(s){};
	virtual ~Card() = default;

	unsigned int getNum() const { return m_num; }
	Suit getSuit() const { return m_suit; }

	bool operator<(const Card& right) const {
		return m_num == right.getNum()?m_suit<right.getSuit():m_num<right.getNum();
	};
};

std::ostream& operator<<(std::ostream& out, const Card& c);
std::ostream& operator<<(std::ostream& out, const Card *c);

} /* namespace game */

#endif /* CARD_H_ */
