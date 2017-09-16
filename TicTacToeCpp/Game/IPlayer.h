#pragma once

namespace TicTacToeGame
{
	enum class Field;	//just a predeclaration to avoid having unnecessary .h files in headers
	class Board;		//same as above

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