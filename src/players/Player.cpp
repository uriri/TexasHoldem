/*
 * Player.cpp
 *
 *  Created on: 2017/01/26
 *      Author: haga
 */

#include "Player.h"
#include "../Field.h"

#include <iostream>
#include <algorithm>

namespace texas {

Player::Player() noexcept:
	m_ID(0), m_money(MONEY), m_isDEALER(false), m_isSB(false), m_isBB(false), m_hand(texas::HAND_NUM){};
Player::Player(unsigned int id, int m, bool deal, bool sb, bool bb) noexcept:
	m_ID(id), m_money(MONEY-m), m_isDEALER(deal), m_isSB(sb), m_isBB(bb), m_hand(texas::HAND_NUM){};
Player::~Player() = default;

//各種フラグリセット
void Player::reset(bool deal, bool sb, bool bb) {
	m_isDEALER = deal;
	m_isSB = sb;
	m_isBB = bb;
}

void Player::reset() {
	m_isSB = m_isBB = false;
}

void Player::show() {
	std::cout << m_ID << "(" << (m_isDEALER?"D":(m_isSB?"SB":(m_isBB?"BB":" ")))
			<< "):" << m_money <<  " { ";
	for (const auto& h : m_hand) {
		std::cout << h << " ";
	} std::cout << "}" << std::endl;
}

//手持ちと共有カードを統合
void Player::combineHand(const std::vector<std::shared_ptr<game::Card>>& commonCards){
	m_hand.insert(std::end(m_hand), std::begin(commonCards), std::end(commonCards));
}

//手持ちと共有カードを分離
void Player::divideHand(){
	m_hand.erase(std::begin(m_hand)+HAND_NUM, std::end(m_hand));
	m_hand.shrink_to_fit();
}

/*     ポーカーでプレイヤができる行動群     */

//勝負を降りる
void Player::Fold(const std::shared_ptr<texas::Field>& field) {
	field->setBet(m_ID, -1);
	std::cout << m_ID << " is Fold" << std::endl;
}

//CheckとCallを合わせたやつ
void Player::sameBet(const std::shared_ptr<texas::Field>& field){
	if(field->isNoBet()){
		field->check(m_ID);
		std::cout << m_ID << " is Check" << std::endl;
	} else {
		const int bet = field->getNowMaxBet();
		m_money -= (bet-field->getplayerBet(m_ID));	//引くのは差額分（SB，BB対策）
		field->setBet(m_ID, bet);
		std::cout << m_ID << " is Call " << bet << std::endl;
	}
}

//RiseとBetを合わせたやつ
void Player::moreBet(const std::shared_ptr<Field>& field, const int betMax){
	int bet = std::min(field->getNowMaxBet()+1, betMax);
	m_money -= bet;
	field->setBet(m_ID, bet);
	std::cout << m_ID << (field->isNoBet()?" is Bet ":" is Rise ") << bet << std::endl;
}

void Player::play(const std::shared_ptr<texas::Field>& field, Turn turn){
	this->combineHand(field->getCommunityCards());
	this->policy(field, turn);
	this->divideHand();
}

#if 0
//場の最大ベットと自分の掛け金の差額をベットする
//CallとCheck
void Player::Call(const std::shared_ptr<texas::Field>& field){
	const int bet = field->getNowMaxBet();
	m_money -= (bet-field->getplayerBet(m_ID));	//引くのは差額分（SB，BB対策）
	field->setBet(m_ID, bet);
	std::cout << m_ID << " is Call " << bet << std::endl;
}
void Player::Check(const std::shared_ptr<texas::Field>& field){
	field->setBet(m_ID, 0);
	std::cout << m_ID << " is Check" << std::endl;
}
#endif
#if 0
//場の最大ベット以上をベットする
void Player::Rise(const std::shared_ptr<Field>& field, const int betMax){
	int bet = std::min(field->getNowMaxBet()+1, betMax);
	m_money -= bet;
	field->setBet(m_ID, bet);
	std::cout << m_ID << " is Rise " << bet << std::endl;
}

void Player::Bet(const std::shared_ptr<Field>& field, const int betMax){
	int bet = std::min(field->getNowMaxBet()+1, betMax);
	m_money -= bet;
	field->setBet(m_ID, bet);
	std::cout << m_ID << " is Bet ";
}
#endif
} /* namespace texas */

