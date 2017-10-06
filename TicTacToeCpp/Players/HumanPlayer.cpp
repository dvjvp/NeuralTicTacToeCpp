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
		std::cout << "Select a number 1-9 to place your " << ToChar(GetColor()) << ".\n\n";

		char result = '\0';
		while (true)
		{
			while (true)
			{
				std::cout << ">>";
				std::cin >> result;
				if (result < '1' || result > '9')
				{
					std::cout << "That's not valid input." << std::endl;
				}
				else
				{
					break;
				}
			}
			result -= '1';	//convert from ASCII number to actual number

			if (!IsMoveValid(result))
			{
				std::cout << "Move invalid." << std::endl;
			}
			else
			{
				return result;
			}
		}
		
	}

}
