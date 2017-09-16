#include "ContinuusPlayer.h"

namespace TicTacToeGame
{
	char ContinuusPlayer::MakeMove(const Board& currentBoard)
	{
		for (char i = 0; i < Board::BOARD_SIZE; i++)
		{
			if (IsMoveValid(i)) return i;
		}
		return 0;
	}

}
