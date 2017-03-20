/*
 * TexasHoldem.h
 *
 *  Created on: 2017/01/26
 *      Author: haga
 */

#ifndef TEXASHOLDEM_H_
#define TEXASHOLDEM_H_

#include "TexasRules.h"

#include <vector>
#include <memory>

namespace texas {

class Player;
class Field;

class TexasHold_em {
private:

	//各プレイヤの情報
	std::vector< std::unique_ptr<Player> > m_players;

	//フィールドの情報
	std::shared_ptr<Field> m_field;

	//ディーラーのID
	unsigned int m_dealNum;

	//プレイ人数
	unsigned int m_playNum;

	//mod PLAYERS
	unsigned int mod(unsigned int n){
		return n%m_playNum;
	}

	//ディーラーの左の人はSB，その左はBB
	//ベットする額
	unsigned int initBet(unsigned int p){
		if(isSB(p)){
			return SB;
		} else if(isBB(p)){
			return BB;
		} else {
			return 0;
		}
	}

	bool isDEALER(unsigned int p){ return p==m_dealNum; }
	bool isSB(unsigned int p){ return p==mod(m_dealNum+1); }
	bool isBB(unsigned int p){ return p==mod(m_dealNum+2); }

	//ゲームの初期化
	void initGame();

	//最後の1人
	bool lastOne();

	//ショーダウン
	//勝利プレイヤのIDを返す
	unsigned int showDown();

public:
	TexasHold_em(unsigned int playerNum, unsigned int human);
	virtual ~TexasHold_em();
	void playGame(unsigned int gameNumber);
};

} /* namespace texas */

#endif /* TEXASHOLDEM_H_ */
