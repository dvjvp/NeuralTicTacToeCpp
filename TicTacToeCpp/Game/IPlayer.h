#pragma once
#include "Board.h"

namespace TicTacToeGame
{

	class IPlayer
	{
	public:
		IPlayer();
		~IPlayer();

		virtual char MakeMove(const Board& currentBoard) = 0;

	protected:
		bool IsMoveValid(char move);
		Field GetColor();

	private:
		friend class TicTacToe;
		class TicTacToe* currentGame;
		Field color;
	};

}