/*
 * TexasRules.cpp
 *
 *  Created on: 2017/01/27
 *      Author: haga
 */

#include "TexasRules.h"

namespace texas{

Turn begin(Turn){ return Turn::PREFLOP; }
Turn end(Turn){ return Turn::END; }
Turn operator*(Turn t){ return t; }
Turn operator++(Turn& t){
	return t = Turn(std::underlying_type<Turn>::type(t) + 1);
}
std::ostream& operator<<(std::ostream& o, Turn t){
	switch(t){
	case Turn::PREFLOP: return o<<"PREFLOP";
	case Turn::FLOP: return o<<"FLOP";
	case Turn::TURN: return o<<"TURN";
	case Turn::REVER: return o<<"REVER";
	case Turn::END: return o<<"END";
	default: return o;
	}
}

}
