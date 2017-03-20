#include "TexasHoldem.h"

int main () {
	constexpr unsigned int playerNum = texas::PLAYERS_DEFAULT;
	constexpr unsigned int human = 0;

	//(人，プレイ人数)の順番
	//プレイ人数と人の差分は完全ランダムプレイヤ
	texas::TexasHold_em Game(playerNum, human);

	Game.playGame(2);

	return 0;
}
