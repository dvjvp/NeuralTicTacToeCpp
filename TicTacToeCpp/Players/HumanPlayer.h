#pragma once
#include "../Game/IPlayer.h"

namespace TicTacToeGame
{
	class HumanPlayer : public IPlayer
	{
	public:
		HumanPlayer();
		~HumanPlayer();

		virtual char MakeMove(const Board& currentBoard) override;
	};

}

