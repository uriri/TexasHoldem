/*
 * TexasHoldem.cpp
 *
 *  Created on: 2017/01/26
 *      Author: haga
 */

#include "TexasHoldem.h"
#include "utility/Deck.h"
#include "Field.h"
#include "players/RandomPlayer.h"
#include "players/HumanPlayer.h"
#include "HandChecker.h"

#include <algorithm>
#include <iostream>
#include <map>

namespace texas {

TexasHold_em::TexasHold_em(unsigned int playerNum, unsigned int human=0):m_dealNum(0), m_playNum(playerNum){
	//山札生成
	std::unique_ptr<game::Deck> d = std::make_unique<game::Deck>();

	m_field = std::make_shared<Field>(std::move(d->getTops(5)), m_playNum, BB);
	m_dealNum = game::makeRandom(0, m_playNum-1);
	m_players.reserve(m_playNum);

	//プレイヤオブジェクトの生成とフィールドの初期化
	for(unsigned int p=0; p<m_playNum; ++p){
		if(p < human){
			m_players.push_back(
					std::make_unique<HumanPlayer>(p, initBet(p), isDEALER(p), isSB(p), isBB(p))
			);
		} else {
			m_players.push_back(
					std::make_unique<RandomPlayer>(p, initBet(p), isDEALER(p), isSB(p), isBB(p))
			);
		}
		m_field->initBet(p, initBet(p));
	}

	//カード配る
	//プログラム的には関係ないかもしれないが
	//一応ディーラーの左（SB）から
	for(unsigned int c=0; c<HAND_NUM; ++c){
		for(unsigned int p=(m_dealNum+1); p<(m_dealNum+1+m_playNum); ++p){
			m_players[mod(p)]->setCard(c, d->getTop());
		}
	}
};

TexasHold_em::~TexasHold_em() = default;

void TexasHold_em::initGame(){
	std::unique_ptr<game::Deck> d = std::make_unique<game::Deck>();

	//フィールドのリセット -> 新しくする
	m_field.reset();
	auto newField = std::make_shared<Field>(d->getTops(5), m_playNum, BB);
	m_field = std::move(newField);

	m_dealNum = game::makeRandom(0, m_playNum-1);

	//ディーラー，SBとかを初期化
	for(unsigned int p=0; p<m_playNum; ++p){
		m_players[p]->reset( isDEALER(p), isSB(p), isBB(p) );
		m_field->initBet(p, this->initBet(p));
	}

	//カード配る
	//プログラム的には関係ないかもしれないが
	//一応ディーラーの左（SB）から
	for(unsigned int c=0; c<HAND_NUM; ++c){
		for(unsigned int p=(m_dealNum+1); p<(m_dealNum+1+m_playNum); ++p){
			m_players[mod(p)]->setCard(c, d->getTop());
		}
	}
};

//最後の1人か否か
bool TexasHold_em::lastOne(){
	return ( m_playNum - std::count_if(std::begin(m_players), std::end(m_players),
			[&](const std::unique_ptr<Player>& p){
				return m_field->isFold(p->getID());
	}) )==1;
}

unsigned int TexasHold_em::showDown(){
	using rArray = std::array<int, game::RANKSIZE>;

	//残ってるプレイヤだけまとめる
	std::vector<std::pair<int, rArray>> remainPlayer;
	remainPlayer.reserve(m_playNum);
	for(auto& p : m_players){
		if( !m_field->isFold(p->getID()) ){
			p->combineHand(m_field->getCommunityCards());
			p->show();
			rArray rank = game::Hand2Rank(p->getHand());
			remainPlayer.emplace_back(p->getID(), rank);
			std::cout << game::Rank2Str(rank) << std::endl;
			p->divideHand();
		}
	}
	remainPlayer.shrink_to_fit();

	//抽出した残りプレイヤの手を全部比較
	int maxID = -1;
	rArray maxRank = {0};
	for(auto i=std::begin(remainPlayer); i!=std::end(remainPlayer); ++i){
		rArray other = i->second;
		if(game::compRank(maxRank, other) == game::RESULT::LOSE){
			maxRank = other;
			maxID = i->first;
		}
	}
	std::cout << "win " << maxID << " " << game::Rank2Str(maxRank) << std::endl;
	return static_cast<unsigned int>(maxID);
}

void TexasHold_em::playGame(unsigned int gameNumber){
	for(unsigned int gameNum=0; gameNum<gameNumber; ++gameNum){
		auto playingNum = m_dealNum+3;
		for(auto turn : Turn()){
			//ラウンドでの共有カードの公開
			m_field->openCard(openNum(turn));

			//残り一人だったら出る
			if(lastOne()) break;
			std::cout << turn << std::endl;

			//参加者全員が同じ額ベットするまでループ
			//当然一人になったら出る
			//全員チェックしても出る
			do{
				for(auto i=playingNum; i<(playingNum+m_playNum); ++i){
					auto& player = m_players[mod(i)];
					if(lastOne()) break;
					if( m_field->isPossibleBet(player->getID()) || player->isBB()){	//降りてない人かBB
						m_field->show();
						player->show();
						player->play(m_field, turn);
					}
				}
			}while(!m_field->isSameBet() && !lastOne() && !m_field->isAllCheck());

			m_field->turnEnd();

			if(turn == Turn::PREFLOP){
				//BBとかは最初だけなので削除
				for(auto& p : m_players)
					p->reset();
			}

		}// turn loop

		/* ショーダウン */
		unsigned int maxID = this->showDown();

		m_players[maxID]->addMoney(m_field->getPot());
		m_players[maxID]->show();

		//ディーラーが1つずれる
		++m_dealNum;
		m_dealNum = mod(m_dealNum);

		//所持金などは引き継ぎつつ試合のリセット
		if(gameNum != gameNumber)
			initGame();

	}// 1 game loop

	std::cout << "ALL GAME END" << std::endl;
}

} /* namespace texas */
