#include "IPlayer.h"
#include "TicTacToe.h"


namespace TicTacToeGame
{

	IPlayer::IPlayer()
	{
	}


	IPlayer::~IPlayer()
	{
	}

	bool IPlayer::IsMoveValid(char move)
	{
		return currentGame->IsValidMove(move);
	}

	inline Field IPlayer::GetColor()
	{
		return color;
	}

}