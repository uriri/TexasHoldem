/*
 * FieldInfo.h
 *
 *  Created on: 2016/12/05
 *      Author: haga
 */

#ifndef FIELD_H_
#define FIELD_H_

#include <vector>
#include <algorithm>
#include <memory>

namespace game{
class Card;
}

namespace texas {

class Field {
private:
	std::vector<std::shared_ptr<game::Card>> m_communityCard;	//共有されるコミュニティカード 5枚
	unsigned int m_openCards;					//公開されてるカード

	std::vector<int> m_playerBet;				//各プレイヤがベットしている金額（負の値はフォールド）
	std::unique_ptr<bool[]> m_isPossibleBet;	//各プレイヤのベット権利
	std::unique_ptr<bool[]> m_isCheck;			//各プレイヤがチェックしたか
	unsigned int m_pot;							//フィールドにポットされた金額
	int m_nowMaxBet;							//現在の最大ベット金額
	bool m_isNoBet;								//すでにベットが行われたか（チェックとコール判別用）
public:
	Field(std::vector<std::unique_ptr<game::Card>>&& card, unsigned int p, unsigned int mb);
	virtual ~Field();

	//最初だけBBにもベット権がある
	void initBet(unsigned int id, int bet);		//ゲーム開始時のsetBet
	void setBet(unsigned int id, int bet);		//ゲーム中のsetBet

	//共有カードオープン
	void openCard(unsigned int n) { m_openCards += n; }

	//共有カード取得
	std::vector<std::shared_ptr<game::Card>> getCommunityCards() {
		std::vector<std::shared_ptr<game::Card>> tmp(m_openCards);
		std::copy(std::begin(m_communityCard),
				std::begin(m_communityCard) + m_openCards, std::begin(tmp));
		return tmp;
	}

	void check(int id){ m_isCheck[id] = true; }

	unsigned int getPot() const { return m_pot; }
	int getNowMaxBet() const { return m_nowMaxBet; }
	int getplayerBet(int p) const { return m_playerBet[p]; }

	bool isNoBet() const { return m_isNoBet; }

	bool isMaxBetPlayer(int p) const { return m_playerBet[p] == m_nowMaxBet; }
	bool isFold(int p) const { return m_playerBet[p] == -1; }
	bool isCheck(int id){ return m_isCheck[id]; }
	bool isPossibleBet(int id){ return m_isPossibleBet[id]; }

	//全員が同額ベットしたか
	bool isSameBet();

	//全員チェックしてるか
	bool isAllCheck();

	//ベット可能なプレイヤにフラグを立てる
	void setPossibleBet();

	//ベットラウンド終了時
	//フォールド以外はベット可能にする，チェックフラグリセット
	void turnEnd();

	void show();
	void showALL();
};

}

#endif /* FIELD_H_ */
