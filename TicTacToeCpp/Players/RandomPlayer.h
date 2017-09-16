#pragma once
#include "../Game/IPlayer.h"

namespace TicTacToeGame
{
	/*
	* AI Player for TicTacToe that uses random moves throughout the whole game
	*
	*/
	class RandomPlayer : public IPlayer
	{
	public:
		RandomPlayer();
		~RandomPlayer();

		virtual char MakeMove(const Board& currentBoard) override;

	private:
		char Random();
	};
}


