#pragma once
#include <iostream>

namespace TicTacToeGame
{

	enum class Field : char
	{
		EMPTY = 0,
		X = 1,
		O = 2
	};

	enum class MoveResult : char
	{
		GAME_CONTINUES = 0,
		X_WON = 1,
		O_WON = 2,
		TIE
	};

	class Board
	{
	public:
		Board();
		~Board();

		void MakeMove(char fieldIndex, Field move);
		Field GetSquare(char fieldIndex) const;

		MoveResult GetBoardStateResult();

	public:
		static const size_t BOARD_SIZE = 9;


		Field data[BOARD_SIZE];
		/*
			Board looks like this:

			0 1 2
			3 4 5
			6 7 8
		*/

	};

	std::ostream& operator<< (std::ostream& stream, const Board& board);
	char ToChar(const Field& field);
}


