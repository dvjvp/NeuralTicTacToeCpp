#include "TicTacToe.h"


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
	}

	TicTacToe::~TicTacToe()
	{
		
	}


	bool TicTacToe::IsValidMove(char position)
	{
		return board.data[position] == Field::EMPTY;
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
			return GetCurrentPlayer()->color == Field::X ? MoveResult::O_WON : MoveResult::X_WON;
		}

		return board.GetBoardStateResult();
	}
	void TicTacToe::SwitchTurns()
	{
		turn = turn == Field::X ? Field::O : Field::X;
	}
	inline IPlayer * TicTacToe::GetCurrentPlayer()
	{
		return turn == Field::X ? playerO : playerX;
	}
}