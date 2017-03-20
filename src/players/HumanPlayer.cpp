/*
 * HumanPlayer.cpp
 *
 *  Created on: 2017/01/27
 *      Author: haga
 */

#include "HumanPlayer.h"

#include <iostream>

namespace texas{

HumanPlayer::HumanPlayer(int id, int m, bool deal, bool sb, bool bb):Player(id, m, deal, sb, bb) {};
HumanPlayer::~HumanPlayer() = default;

void HumanPlayer::policy(const std::shared_ptr<texas::Field>& field, Turn turn){
	const auto betMax = limitBet(turn);
	int flag = -1;
	do {
		std::cout << "input action (0:Fold 1:Call(Check) 2:Rise(Bet) )" << std::endl;
		std::cin >> flag;
	}while (flag>2);

	switch (flag) {
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
		std::cerr << "input error" << std::endl;
		break;
	}
}

}/* namespace texas */
