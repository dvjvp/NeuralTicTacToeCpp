#pragma once
#include "Board.h"
#include "IPlayer.h"



namespace TicTacToeGame
{

	class TicTacToe
	{
	public:
		TicTacToe(IPlayer* playerX, IPlayer* playerO);
		~TicTacToe();

		bool IsValidMove(char position);
		void MakeMove(char position);

		MoveResult Update();

	protected:
		void SwitchTurns();
		inline IPlayer* GetCurrentPlayer();

	public:
		Board board;
		Field turn = Field::X;
		IPlayer* playerX, * playerO;
	};

}