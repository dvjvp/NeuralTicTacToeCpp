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
	Field Board::GetSquare(char fieldIndex) const
	{
		return data[fieldIndex];
	}
	MoveResult Board::GetBoardStateResult()
	{
		//check vertically
		for (size_t i = 0; i < 3; i++)
		{
			if (data[i] == data[i + 3] && data[i] == data[i + 6])
			{
				return (MoveResult)data[i];
			}
		}

		//check horizontally
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
	std::ostream & operator<<(std::ostream & stream, const Board & board)
	{
		stream << ToChar(board.data[0]) << '|' << ToChar(board.data[1]) << '|' << ToChar(board.data[2]) << std::endl;
		stream << "-----\n";
		stream << ToChar(board.data[3]) << '|' << ToChar(board.data[4]) << '|' << ToChar(board.data[5]) << std::endl;
		stream << "-----\n";
		stream << ToChar(board.data[6]) << '|' << ToChar(board.data[7]) << '|' << ToChar(board.data[8]) << std::endl;

		return stream;
	}
	char ToChar(const Field & field)
	{
		switch (field)
		{
		case Field::EMPTY:
			return ' ';
		case Field::X:
			return 'X';
		case Field::O:
			return 'O';
		default:
			return '*';
		}
	}
}