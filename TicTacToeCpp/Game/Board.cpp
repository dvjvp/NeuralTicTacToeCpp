#include "Board.h"
#include <string>
#include <exception>


namespace TicTacToeGame
{

	Board::Board()
	{
		memset(data, 0, sizeof(Field)*BOARD_SIZE);
	}


	Board::~Board()
	{
	}
	void Board::MakeMove(char fieldIndex, Field move)
	{
		if (data[fieldIndex] == Field::EMPTY)
		{
			data[fieldIndex] = move;
		}
		else
		{
			throw std::exception("Invalid argument in MakeMove");
		}
	}
	Field Board::GetSquare(char fieldIndex)
	{
		return data[fieldIndex];
	}
	MoveResult Board::GetBoardStateResult()
	{
		//check horizontally
		for (size_t i = 0; i < 3; i++)
		{
			if (data[i] == data[i + 3] && data[i] == data[i + 6])
			{
				return (MoveResult)data[i];
			}
		}

		//check vertically
		for (size_t i = 0; i < BOARD_SIZE; i += 3)
		{
			if (data[i] == data[i + 1] && data[i] == data[i + 2])
			{
				return (MoveResult)data[i];
			}
		}
		
		//check diagonally
		if (data[0] == data[4] && data[0] == data[8])
		{
			return (MoveResult)data[4];
		}
		if (data[2] == data[4] && data[2] == data[6])
		{
			return (MoveResult)data[4];
		}

		//check for tie
		for (size_t i = 0; i < BOARD_SIZE; i++)
		{
			if (data[i] == Field::EMPTY)
			{
				return MoveResult::GAME_CONTINUES;
			}
		}

		return MoveResult::TIE;
	}
}