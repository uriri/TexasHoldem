/*
 * PlayerHuman.h
 *
 *  Created on: 2017/01/26
 *      Author: haga
 */

#ifndef HUMANPLAYER_H_
#define HUMANPLAYER_H_

#include "Player.h"

namespace texas {

class HumanPlayer : public Player {
public:
	HumanPlayer(int id, int m, bool deal, bool sb, bool bb);
	virtual ~HumanPlayer();

	void policy(const std::shared_ptr<texas::Field>& field, Turn turn);
};

} /* namespace texas */
#endif /* HUMANPLAYER_H_ */

