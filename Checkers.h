#pragma once
#include <iostream>
#include "cursor.h"
#include "colors.h"
#include "Pieces.h"
#include <conio.h>


class Board {
	private:
		static int size_x;
		static int size_y;
		static int cell_size_x;
		static int cell_size_y;
		static int offset_y;
		static int offset_x;

		Piece*** arr;
		int player;

		int start_cursor_x;
		int start_cursor_y;
		int cursor_x;
		int cursor_y;

	public:
		Board()
		{
			arr = new Piece * *[size_y];
			for (int row = 0; row < size_y; row++)
			{
				arr[row] = new Piece * [size_x];
			}
			player = 1;
			SetBoard();
			cursor_x = 0;
			cursor_y = size_y - 1;
			start_cursor_x = -1;
			start_cursor_y = -1;
		}
		~Board()
		{
			for (int row = 0; row < size_y; row++)
			{
				for (int col = 0; col < size_x; col++)
				{
					delete arr[row][col];
				}
				delete[] arr[row];
			}
			delete[] arr;
		}
		void SetBoard()
		{
			for (int row = 0; row < size_y; row++)
			{
				for (int col = 0; col < size_x; col++)
				{
					if (row % 2 != col % 2) //black cell
					{
						if (row <= 2) arr[row][col] = new Piece(Piece::Types::man, Piece::Sides::black);
						else if (row >= size_y - 3) arr[row][col] = new Piece(Piece::Types::man, Piece::Sides::white);
						else arr[row][col] = nullptr;
					}
					else {
						arr[row][col] = nullptr;
					}
				}
			}
		}
		void GameEnd()
		{

		}
		bool GameEndCheck()
		{
			return false;
		}
		void Press(char key)
		{
			switch (key)
			{
				//RESHOW
				case 'r':
				case 'R':
					Show();
					break;
				//CURSOR-LEFT
				case 'a':
				case 'A':
					break;
				//CURSOR-RIGHT
				case 'd':
				case 'D':
					break;
				//CURSOR-UP
				case 'w':
				case 'W':
					break;
				//CURSOR-DOWN
				case 's':
				case 'S':
					break;
				//MAKE-A-MOVE
				case 'f':
				case 'F':
					break;
			}
		}
		void Start()
		{
			Show();
			char key;
			do {
				key = _getch();
				Press(key);
			} while (GameEndCheck() == false);
			GameEnd();
		}
		void ShowControls()
		{
			Cursor::set(size_x * cell_size_x + 2, 0+offset_y);
			std::cout << "WASD - to move";

			Cursor::set(size_x * cell_size_x + 2, 1 + offset_y);
			std::cout << "F - to choose a cell";

			Cursor::set(size_x * cell_size_x + 2, 2 + offset_y);
			std::cout << "X - to cancel a choice";

			Cursor::set(size_x * cell_size_x + 2, 3 + offset_y);
			std::cout << "R - to reshow";

			Cursor::set(0, size_y * cell_size_y + offset_y - 1);
		}
		void ShowPlayer()
		{
			Cursor::set(offset_x, 0);
			std::cout << "Player " << player << ":";
			Cursor::set(0, size_y*cell_size_y+offset_y-1);
		}
		void ShowCell(int row, int col)
		{
			Colors fg, bg;
			if (row % 2 == col % 2) bg = Colors::white;
			else bg = Colors::black;

			if (arr[row][col] && arr[row][col]->side == Piece::Sides::white) fg = Colors::lwhite;
			else fg = Colors::lblack;


			SetColor(fg, bg);

			for (int i = 0; i < cell_size_y; i++)
			{
				Cursor::set(col * cell_size_x + offset_x, row*cell_size_y + i + offset_y);
				for (int j = 0; j < cell_size_x; j++)
				{
					std::cout << ' ';
				}
			}
			Cursor::set(col * cell_size_x + cell_size_x / 2 + offset_x, row * cell_size_y + cell_size_y / 2 + offset_y);
			if (arr[row][col]) std::cout << *arr[row][col];
			SetColor();
			Cursor::set(0, size_y * cell_size_y + offset_y - 1);
		}
		void Show()
		{
			system("cls");
			for (int row = 0; row < size_y; row++) {
				for (int col = 0; col < size_x; col++) {
					ShowCell(row, col);
				}
				std::cout << std::endl;
			}
			ShowPlayer();
			ShowControls();
		}
};
int Board::size_x = 8;
int Board::size_y = 8;
int Board::cell_size_x = 5;
int Board::cell_size_y = 3;
int Board::offset_x = 0;
int Board::offset_y = 2;