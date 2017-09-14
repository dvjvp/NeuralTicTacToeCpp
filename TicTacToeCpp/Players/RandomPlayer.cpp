#include "RandomPlayer.h"
#include <cstdlib>
#include <time.h>

namespace TicTacToeGame
{

	RandomPlayer::RandomPlayer()
	{
		srand((size_t)time(0));
	}


	RandomPlayer::~RandomPlayer()
	{
	}

	char RandomPlayer::MakeMove(const Board& currentBoard)
	{
		char c;

		do 
		{
			c = Random();
		} 
		while (!IsMoveValid(c));

		return c;
	}

	char RandomPlayer::Random()
	{
		return (char)(rand() % Board::BOARD_SIZE);
	}

}
