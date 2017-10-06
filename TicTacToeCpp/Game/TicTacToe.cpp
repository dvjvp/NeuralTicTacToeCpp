#include "TicTacToe.h"
#include <iostream>

namespace TicTacToeGame
{

	TicTacToe::TicTacToe(IPlayer* playerX, IPlayer* playerO)
	{
		this->playerX = playerX;
		this->playerO = playerO;

		playerX->currentGame = this;
		playerO->currentGame = this;
		playerX->color = Field::X;
		playerO->color = Field::O;

		turn = Field::X;
	}

	TicTacToe::~TicTacToe()
	{
		
	}


	bool TicTacToe::IsValidMove(char position)
	{
		if (position >= Board::BOARD_SIZE) return false;
		return (board.data[position] == Field::EMPTY);
	}
	void TicTacToe::MakeMove(char position)
	{
		board.MakeMove(position, turn);
		SwitchTurns();
	}
	MoveResult TicTacToe::Update()
	{
		char move = GetCurrentPlayer()->MakeMove(board);
		if (!IsValidMove(move))
		{
			//returning wrong move equals auto-losing
			std::cout << "Player tried to make an illegal move: " << (int)move << std::endl;
			return GetCurrentPlayer()->color == Field::X ? MoveResult::O_WON : MoveResult::X_WON;
		}
		MakeMove(move);
		return board.GetBoardStateResult();
	}
	void TicTacToe::SwitchTurns()
	{
		turn = turn == Field::X ? Field::O : Field::X;
	}
	inline IPlayer * TicTacToe::GetCurrentPlayer()
	{
		return turn == Field::X ? playerX : playerO;
	}
}