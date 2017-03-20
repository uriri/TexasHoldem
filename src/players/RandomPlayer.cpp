/*
 * RandomPlayer.cpp
 *
 *  Created on: 2017/01/27
 *      Author: haga
 */

#include "RandomPlayer.h"
#include "../Field.h"

#include <iostream>

namespace texas{

RandomPlayer::RandomPlayer(int id, int m, bool deal, bool sb, bool bb):Player(id, m, deal, sb, bb){};
RandomPlayer::~RandomPlayer() = default;

void RandomPlayer::policy(const std::shared_ptr<texas::Field>& field, Turn turn){
	const auto betMax = limitBet(turn);
	int flag = -1;
	do{
		flag = game::makeRandom(0, 2);
	}while(flag==1 && field->isCheck(m_ID));

	switch(flag){
	case 0:
		Fold(field);
		break;
	case 1:
		//Call(field);
		sameBet(field);
		break;
	case 2:
		moreBet(field, betMax);
		break;
	default:
		std::cerr << "random error" << std::endl;
		break;
	}
}

}/* namespace texas */
