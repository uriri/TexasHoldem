/*
 * PlayerRandom.h
 *
 *  Created on: 2017/01/26
 *      Author: haga
 */

#ifndef RANDOMPLAYER_H_
#define RANDOMPLAYER_H_

#include "Player.h"

namespace texas {

class RandomPlayer: public Player {
private:
public:
	RandomPlayer(int id, int m, bool deal, bool sb, bool bb);
	~RandomPlayer();

	//方策，インターフェイスにしてプレイヤごとで変えれるように
	void policy(const std::shared_ptr<texas::Field>& field, Turn turn);
};

} /* namespace texas */

#endif /* RANDOMPLAYER_H_ */
