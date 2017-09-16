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

	Field IPlayer::GetColor()
	{
		return color;
	}

}