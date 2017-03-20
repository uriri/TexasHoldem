/*
 * HandChecker.h
 *
 *  Created on: 2016/11/24
 *      Author: haga
 */

#ifndef HANDCHECKER_H_
#define HANDCHECKER_H_

#include <array>
#include <algorithm>
#include <iostream>

namespace game {

constexpr int NUM = 13;
constexpr unsigned int RANKSIZE = 3;

enum class RESULT{
	WIN = 0,
	LOSE,
	DRAW
};

std::array<int, RANKSIZE> Hand2Rank(const std::vector<std::shared_ptr<game::Card>>& hand){
	std::array<int, 5> suit{0};
	std::array<int, NUM> nums{0};

	//ポーカーではjokerはないらしい
	//joker対応はなしで
	for(const auto& h : hand){
		++suit[toIntegral(h->getSuit())];
		++nums[h->getNum()-1];
	}

	//所持しているかどうか（0でないかどうか）
	auto isHave = [](int c){return c!=0;};

	// suit : { JOKER, CLUB, DIAMOND, HEART, SPADE }
	// nums : [ 数字-1 ]の要素

	//一番大きな数字 index+1
	//後ろから逆順探索，Aが一番強い
	int highest = (nums[0]!=0)?
			1:
			nums.rend() - std::find_if(nums.rbegin(), nums.rend(),isHave);

	//フラッシュ（全部のスートが同じ）
	//5枚同じスートならフラッシュ
	bool isFlush = std::count(std::begin(suit), std::end(suit), 5);

	//ストレート（連番）
	// A-2-3-4-5 10-J-Q-K-Aはおｋ
	// Q-K-A-2-3などのK-A-2を含むものはNG
	bool isStraight = true;
	auto pos = std::distance(std::begin(nums),
			std::find_if(std::begin(nums), std::end(nums), isHave)
	);
	auto pEnd = pos+5;
	for(int p=pos; p<pEnd; ++p){
		if(nums[p%14] == 0){
			isStraight = false;
			break;
		}
	}
	//10-J-Q-K-Aのパターン
	if(pos == 0 && !isStraight){
		//std::all_of 全部tureかどうか
		isStraight = std::all_of(std::begin(nums)+9, std::end(nums), isHave);
//		isStraight = true;
//		for(int i=9; i<NUM; ++i){
//			if(nums[i] == 0){
//				isStraight = false;
//				break;
//			}
//		}
	}

	//ペア系用の数字カウント
	auto count = [&](auto n){
		return std::count(std::begin(nums), std::end(nums), n);
	};

	//数字の強さ
	auto calRank = [&](int n){
		return (nums[0]==n)?
				1:nums.rend() - std::find(nums.rbegin(), nums.rend(), n);
	};

	//ペア
	const bool isFour = count(4);
	const bool isTree = count(3);
	const int pairNum = count(2);
	const bool isOnePair = (pairNum==1);
	const bool isTwoPair = (pairNum==2);
	const bool isFull = isTree&isOnePair;

	//役の中で一番大きい数字
	const int fourRank = calRank(4);
	const int treeRank = calRank(3);
	const int pairRank = calRank(2);

	if(isFlush){
		if(isStraight){
			//Straight Flush
			return {8, highest, highest};
		}
		return {5, highest, highest};
	} else if(isStraight){
		return {4, highest, highest};
	}

	if(isOnePair){
		if(isFull){
			return {6, treeRank, pairRank};
		}
		return {1, pairRank, highest};
	} else if(isTwoPair){
		return {2, pairRank, highest};
	} else if(isTree){
		return {3, treeRank, highest};
	} else if(isFour){
		return {7, fourRank, highest};
	}
	return {0, highest, highest};
}

template<typename rank>
constexpr std::string Rank2Str(const rank& r){
	int hand = r[0];
	std::string high = std::to_string(r[1]);
	std::string sub = std::to_string(r[2]);
	switch(hand){
	case 0: return "High Card(" + std::to_string(hand) + ") : " + high + ", " + sub;
	case 1: return "One Pair(" + std::to_string(hand) + ") : " + high + ", " + sub;
	case 2: return "Two Pair(" + std::to_string(hand) + ") : " + high + ", " + sub;
	case 3: return "Three of a Kind(" + std::to_string(hand) + ") : " + high + ", " + sub;
	case 4: return "Straight(" + std::to_string(hand) + ") : " + high + ", " + sub;
	case 5: return "Flush(" + std::to_string(hand) + ") : " + high + ", " + sub;
	case 6: return "Full House(" + std::to_string(hand) + ") : " + high + ", " + sub;
	case 7: return "Four of a Kind(" + std::to_string(hand) + ") : " + high + ", " + sub;
	case 8: return "Straight Flush(" + std::to_string(hand) + ") : " + high + ", " + sub;
	default:return "error";
	}
}

//srcの勝敗が返ってくる
//handRank -> handHighest -> subRankの順で比較
//2017/02/14 index_tupleイディオムを使って実装するのもありかも
template<typename rArray>
RESULT compRank(const rArray& src, const rArray& other, std::size_t pos=0){
	//1 -> 13の順番を 2 -> 13, 1の順番に
	auto rank = [](int n){ return n==1?NUM:n-1; };

	auto sRank = pos==0?src[pos]:rank(src[pos]);
	auto oRank = pos==0?other[pos]:rank(other[pos]);
	if(sRank < oRank){
		return RESULT::LOSE;
	} else if(sRank == oRank){
		++pos;
		return pos==RANKSIZE?RESULT::DRAW:compRank(src, other, pos);
	} else {
		return RESULT::WIN;
	}
}

#if 0
//srcの勝敗が返ってくる
template<int pos = 0>
RESULT comp(const std::array<int, RANKSIZE>& src, const std::array<int, RANKSIZE>& other){
	//1 -> 13の順番を 2 -> 13, 1の順番に
	auto rank = [](int n){ return n==1?13:n-1; };

	auto sRank = pos==0?src[pos]:rank(src[pos]);
	auto oRank = pos==0?other[pos]:rank(other[pos]);
	if(sRank < oRank){
		return RESULT::LOSE;
	} else if(sRank == oRank){
		return comp<pos+1>(src, other);
	} else {
		return RESULT::WIN;
	}
}

template<>
RESULT comp<RANKSIZE>(const std::array<int, RANKSIZE>& src, const std::array<int, RANKSIZE>& other){
	return RESULT::DRAW;
}

#endif

template<typename rArray>
void showResult(const rArray& p1Rank, const rArray& p2Rank){
	switch(comp(p1Rank, p2Rank)){
	case RESULT::WIN:
		std::cout << "P1 win" << std::endl;
		break;
	case RESULT::LOSE:
		std::cout << "P2 win" << std::endl;
		break;
	case RESULT::DRAW:
		std::cout << "draw" << std::endl;
		break;
	}
}

} /* namespace game */

#endif /* HANDCHECKER_H_ */
