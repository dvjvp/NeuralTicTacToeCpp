#pragma once
#include "Board.h"

namespace TicTacToeGame
{

	/**
	* Interface for TicTacToe players to derive from.
	* Allows easily switching players for a game and creating new
	* player types (based on different algorithms or something)
	*/
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