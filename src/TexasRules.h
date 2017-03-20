/*
 * TexasRules.h
 *
 *  Created on: 2016/12/01
 *      Author: haga
 */

#ifndef TEXASRULES_H_
#define TEXASRULES_H_

#include <ostream>

namespace texas{

//プレイ人数
constexpr unsigned int PLAYERS_DEFAULT = 6;

//所持金
constexpr int MONEY = 100;

//スモールブラインドとビッグブラインド
//なんか最初にかけされられるやつ
constexpr unsigned int SB = 1;
constexpr unsigned int BB = SB*2;

//上限
constexpr unsigned int LIMIT_FIRST = BB*2;
constexpr unsigned int LIMIT_SECOND = LIMIT_FIRST*2;

//手札枚数
constexpr int HAND_NUM = 2;

//ベットラウンド
enum class Turn{
	PREFLOP = 0,
	FLOP,
	TURN,
	REVER,
	END
};

Turn begin(Turn);
Turn end(Turn);
Turn operator*(Turn t);
Turn operator++(Turn& t);

std::ostream& operator<<(std::ostream& o, Turn t);

//ベットラウンドでオープンするカード枚数
constexpr unsigned int openNum(Turn t){
	switch(t){
	case Turn::PREFLOP: return 0;
	case Turn::FLOP: return 3;
	case Turn::TURN:
	case Turn::REVER: return 1;
	case Turn::END:
	default: return 0;
	}
}

//ベットラウンドでのかけ金上限
constexpr unsigned int limitBet(Turn t){
	switch(t){
	case Turn::PREFLOP:
	case Turn::FLOP: return LIMIT_FIRST;
	case Turn::TURN:
	case Turn::REVER: return LIMIT_SECOND;
	case Turn::END:
	default: return 0;
	}
}

} /* namespace texas */


#endif /* TEXASRULES_H_ */
