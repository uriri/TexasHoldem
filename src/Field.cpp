/*
 * Field.cpp
 *
 *  Created on: 2017/01/30
 *      Author: haga
 */

#include "Field.h"

#include <iostream>
#include "utility/Card.h"

namespace texas {

Field::Field(std::vector< std::unique_ptr<game::Card> >&& card, unsigned int p, unsigned int mb) :
		m_openCards(0), m_playerBet(p),
		m_isPossibleBet(new bool[p]), m_isCheck(new bool[p]),
		m_pot(0), m_nowMaxBet(mb), m_isNoBet(false)
{
	m_communityCard.reserve(5);
	std::move(card.begin(), card.end(), std::back_inserter(m_communityCard));
	const size_t s = m_playerBet.size();
	for(unsigned int i=0; i<s; ++i){
		m_isPossibleBet[i] = true;
		m_isCheck[i] = false;
	}
}

Field::~Field() = default;

//最初だけBBにもベット権がある
void Field::initBet(unsigned int id, int bet){
	const size_t s = m_playerBet.size();
	//最初は全員あり
	for(unsigned int i=0; i<s; ++i){
		m_isPossibleBet[i] = true;
	}
	m_playerBet[id] = bet;
	//-1はフォールドした人なので足さないようにしてる
	m_pot = std::accumulate(std::begin(m_playerBet), std::end(m_playerBet), 0,
			[](int sum, auto val){ return sum+(val>0?val:0); }
	);
}

void Field::setBet(unsigned int id, int bet) {
	if (bet >= 0) {
		//かけ金上がった時
		if (bet > m_nowMaxBet) {
			m_nowMaxBet = bet;
		}
		const size_t s = m_playerBet.size();
		//最大かけ金以外の人はベット権あり
		//フォールドした人はなし（あたりまえだよなぁ？）
		for(unsigned int i=0; i<s; ++i){
			m_isPossibleBet[i] = isFold(i)?false:
					(i!=id&&!isMaxBetPlayer(i));
		}
		m_playerBet[id] = bet;
		//-1はフォールドした人なので足さないようにしてる
		m_pot = std::accumulate(std::begin(m_playerBet), std::end(m_playerBet), 0,
				[](int sum, auto val){ return sum+(val>0?val:0); }
		);
		m_isNoBet = false;
	} else {
		//Foldした時は bet=-1 にしてる
		m_playerBet[id] = bet;
		m_isPossibleBet[id] = false;
	}
}

//全員が同額ベットしたか
bool Field::isSameBet() {
	const size_t s = m_playerBet.size();

	//-1ではないベット額のインデックス
	auto betIndex = std::distance(std::begin(m_playerBet),
			std::find_if(std::begin(m_playerBet), std::end(m_playerBet),
					[](auto b){ return (b!=-1); })
	);
	int tmp = m_playerBet[betIndex];

	++betIndex;
	for (unsigned int i=betIndex; i<(betIndex+s); ++i) {
		if ( !isFold(i%s) && tmp != m_playerBet[i%s] ) {
			return false;
		}
	}
	return true;
}

//全員チェックしてても次に進む
bool Field::isAllCheck(){
	return std::all_of(std::begin(m_playerBet), std::end(m_playerBet),
			[&](int i){ return (m_isCheck[i] || isFold(i)); }
	);
}

//ベット可能なプレイヤにフラグを立てる
void Field::setPossibleBet(){
	//最大かけ金以外の人はベット権あり
	const size_t s = m_playerBet.size();
	for(unsigned int i=0; i<s; ++i){
		m_isPossibleBet[i] = !isFold(i)?(!isMaxBetPlayer(i)):false;
	}
}

//ベットラウンド終了時
//フォールド以外はベット可能にする，チェックフラグリセット
void Field::turnEnd(){
	const size_t s = m_playerBet.size();
	for(unsigned int i=0; i<s; ++i){
		m_isPossibleBet[i] = !isFold(i);
		m_isCheck[i] = false;
	}
	m_isNoBet = true;
}

void Field::show() {
	std::cout << "-------------------------" << std::endl;

	std::cout << "pot : " << m_pot;
	std::cout << " { ";
	for (const auto pb : m_playerBet) {
		std::cout << pb << " ";
	} std::cout << "} " << std::endl;

	std::cout << "common : ";
	for (unsigned int i = 0; i < m_openCards; ++i) {
		std::cout << m_communityCard[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "now bet : " << m_nowMaxBet << "(" << (m_isNoBet?"No":"Bet") << ")" << std::endl;

	std::cout << "-------------------------" << std::endl;
}

void Field::showALL() {
	auto space = [](int p){
		std::string o("");
		int s = static_cast<int>(log10(p))+1;
		for(int i=0; i<s; ++i){ o += " "; }
		return o;
	};
	const size_t s = m_playerBet.size();

	std::cout << "-------------------------" << std::endl;

	std::cout << "pot : " << m_pot;
	std::cout << " { ";
	for (const auto pb : m_playerBet) {
		std::cout << pb << " ";
	} std::cout << "} " << std::endl;

	std::cout << "canBet" << space(m_pot);
	std::cout << " { ";
	for (unsigned int i=0; i<s; ++i) {
		std::cout << m_isPossibleBet[i] << " ";
	} std::cout << "} " << std::endl;

	std::cout << "check " << space(m_pot);
	std::cout << " { ";
	for (unsigned int i=0; i<s; ++i) {
		std::cout << m_isCheck[i] << " ";
	} std::cout << "} " << std::endl;

	std::cout << "common : ";
	for(unsigned int i=0; i<5; ++i){
		std::cout << m_communityCard[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "now bet : " << m_nowMaxBet << "(" << (m_isNoBet?"No":"Bet") << ")" << std::endl;

	std::cout << "-------------------------" << std::endl;
}

}/* namespace texas */
