#pragma once
#include "../Game/IPlayer.h"

namespace TicTacToeGame
{
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


