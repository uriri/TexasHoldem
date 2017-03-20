/*
 * PlayerInfo.h
 *
 *  Created on: 2016/11/24
 *      Author: haga
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "../utility/Card.h"
#include "../TexasRules.h"

#include <vector>
#include <memory>

namespace texas {

class Field;

class Player {
protected:
	unsigned int m_ID;					//プレイヤID
	int m_money;						//所持金

	bool m_isDEALER;					//ディーラーかどうか
	bool m_isSB;						//SBかどうか
	bool m_isBB;						//BBかどうか

	std::vector<std::shared_ptr<game::Card>> m_hand;	//手札
public:
	Player() noexcept;
	Player(unsigned int id, int m, bool deal, bool sb, bool bb) noexcept;
	virtual ~Player();

	void addMoney(int m){ m_money += m; }

	void setCard(int i, std::unique_ptr<game::Card> card){ m_hand[i] = std::move(card); }

	int getID() const { return m_ID; }
	std::vector<std::shared_ptr<game::Card>> getHand() const { return m_hand; }

	bool isDEALER() const { return m_isDEALER; }
	bool isBB() const {return m_isBB; }

	//各種フラグリセット
	void reset(bool deal, bool sb, bool bb);
	void reset();

	void show();

	//手持ちと共有カードを統合
	void combineHand(const std::vector<std::shared_ptr<game::Card>>& commonCards);

	//手持ちと共有カードを分離
	void divideHand();

	/*     ポーカーでプレイヤができる行動群     */

	//勝負を降りる
	void Fold(const std::shared_ptr<texas::Field>& field);

	//場の最大ベットと自分の掛け金の差額をベットする
	//CallとCheck
	void sameBet(const std::shared_ptr<texas::Field>& field);

	//場の最大ベット以上をベットする
	void moreBet(const std::shared_ptr<Field>& field, const int betMax);

	//プレイ関数
	//共有カードと統合～分割まで全部やる
	void play(const std::shared_ptr<texas::Field>& field, Turn turn);

	//方策，インターフェイスにしてプレイヤごとで変えれるように
	virtual void policy(const std::shared_ptr<texas::Field>& field, Turn turn) = 0;

#if 0
	void Call(const std::shared_ptr<texas::Field>& field);
	void Check(const std::shared_ptr<texas::Field>& field);
	void Rise(const std::shared_ptr<Field>& field, const int betMax);
	void Bet(const std::shared_ptr<Field>& field, const int betMax);
#endif
};

} /* namespace texas */

#if 0
namespace japanPoker {

class Player {
private:
	std::vector<Card> m_hand;
public:
	Player(const std::vector<Card>& hand) {
		m_hand.resize(hand.size());
		std::copy(hand.begin(), hand.end(), m_hand.begin());
		std::sort(m_hand.begin(), m_hand.end());
	}
	virtual ~Player() {
	}

	int getHandNum() const {
		return m_hand.size();
	}
	std::vector<Card> getHand() const {
		return m_hand;
	}

	void showHand() {
		const int s = m_hand.size();
		std::cout << "{ ";
		for (int i = 0; i < s; ++i) {
			std::cout << i << ":" << m_hand[i] << " ";
		}
		std::cout << "}" << std::endl;
	}

	void addCard(const Card& c) {
		m_hand.push_back(c);
		std::sort(std::begin(m_hand), std::end(m_hand));
	}
	Card disCard(int n) {
		auto c = m_hand[n];
		m_hand.erase(std::begin(m_hand) + n);
		return c;
	}
};

} /* namespace japanPoker */
#endif


#endif /* PLAYER_H_ */

