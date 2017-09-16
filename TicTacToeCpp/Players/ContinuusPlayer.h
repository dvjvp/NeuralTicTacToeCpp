#pragma once
#include "../Game/IPlayer.h"

namespace TicTacToeGame
{
	class ContinuusPlayer : public IPlayer
	{
	public:
		virtual char MakeMove(const Board& currentBoard) override;

	};

}
