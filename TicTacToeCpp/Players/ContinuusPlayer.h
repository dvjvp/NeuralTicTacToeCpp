#pragma once
#include "../Game/IPlayer.h"

namespace TicTacToeGame
{	
	/*
	* AI Player for TicTacToe that goes through all the fields in the same order every time:

	* 1|2|3
	* -+-+-
	* 4|5|6
	* -+-+-
	* 7|8|9
	*
	* When a field is already taken, he takes the next one from the list
	*/
	class ContinuusPlayer : public IPlayer
	{
	public:
		virtual char MakeMove(const Board& currentBoard) override;

	};

}
