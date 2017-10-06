#include "HumanPlayer.h"
#include <iostream>

namespace TicTacToeGame
{

	HumanPlayer::HumanPlayer()
	{
	}


	HumanPlayer::~HumanPlayer()
	{
	}

	char HumanPlayer::MakeMove(const Board& currentBoard)
	{
		std::cout << currentBoard;
		std::cout << std::endl;
		std::cout << "Select a number 0-8 to place your " << ToChar(GetColor()) << ".\n\n";

		char result = '\0';
		while (true)
		{
			while (true)
			{
				std::cout << ">>";
				std::cin >> result;
				if (result < '0' || result > '8')
				{
					std::cout << "That's not valid input.";
				}
				else
				{
					break;
				}
			}

			if (!IsMoveValid(result))
			{
				std::cout << "Move invalid.";
			}
			else
			{
				return result;
			}
		}
		
	}

}
