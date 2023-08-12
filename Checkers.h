#pragma once
#include <iostream>
#include "cursor.h"
#include "colors.h"
#include "Pieces.h"
#include <conio.h>
#include <set>


class Board {
	private:
		static int size_x;
		static int size_y;
		static int cell_size_x;
		static int cell_size_y;
		static int offset_y;
		static int offset_x;
		static char cursor_symbol;

		Piece*** arr;
		int player;

		int start_cursor_x;
		int start_cursor_y;
		int cursor_x;
		int cursor_y;

		std::set<std::pair<int, int>> move_variants;

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
		void HideCursor()
		{
			ShowCell(cursor_y, cursor_x, false);
		}
		void ShowCursor()
		{
			ShowCell(cursor_y, cursor_x);
		}
		void CancelMove()
		{
			//CELL WAS NOT CHOOSED
			if (start_cursor_x == -1 && start_cursor_y == -1) return;
			int x = start_cursor_x;
			int y = start_cursor_y;
			start_cursor_x = -1;
			start_cursor_y = -1;
			ShowCell(y, x);
			DelMoveVariants();
		}
		void MakeAMove()
		{
			//START CELL
			if (start_cursor_x == -1 && start_cursor_y == -1)
			{
				//STARTING WITH AN EMPTY CELL
				if (arr[cursor_y][cursor_x] == nullptr) return;

				//STARTING WITH AN ENEMY PIECE
				if ((arr[cursor_y][cursor_x]->side == Piece::Sides::black && player == 1)
					||
					(arr[cursor_y][cursor_x]->side == Piece::Sides::white && player == 2)) return;

				start_cursor_x = cursor_x;
				start_cursor_y = cursor_y;
				SetMoveVariants(cursor_x, cursor_y);
				ShowMoveVariants();
			}

			//END CELL
			else
			{
				//CORRECT MOVE
				if (move_variants.find(std::pair<int, int>(cursor_x, cursor_y)) != move_variants.end())
				{
					//jump over enemy piece
					if (abs(cursor_x - start_cursor_x) == 2 && abs(cursor_y - start_cursor_y) == 2)
					{
						arr[(cursor_y + start_cursor_y) / 2][(cursor_x + start_cursor_x) / 2] = nullptr;
						ShowCell((cursor_y + start_cursor_y) / 2, (cursor_x + start_cursor_x) / 2);
					}
					arr[cursor_y][cursor_x] = arr[start_cursor_y][start_cursor_x];
					arr[start_cursor_y][start_cursor_x] = nullptr;
					ShowCell(cursor_y, cursor_x);
					CancelMove();
					player = (player == 1 ? 2 : 1);
					ShowPlayer();
				}
			}
		}
		void ShowMoveVariants()
		{
			for (std::pair<int, int> coords : move_variants)
			{
				ShowCell(coords.second, coords.first);
			}
		}
		void DelMoveVariants()
		{
			std::set<std::pair<int, int>> move_variants_copy = move_variants;
			move_variants.clear();
			for (std::pair<int, int> coords : move_variants_copy)
			{
				ShowCell(coords.second, coords.first);
			}
		}
		void SetMoveVariants(int x, int y)
		{
			move_variants.clear();
			if ((x >= 0 || x <= size_x - 1) == false) return;
			if ((y >= 0 || y <= size_y - 1) == false) return;
			if (arr[y][x] == nullptr) return;
			
			//white (or king)
			if (arr[y][x]->side == Piece::Sides::white || arr[y][x]->type == Piece::Types::king)
			{
				//up 1 . left 1 (empty)
				if (x - 1 >= 0 && y - 1 >= 0 && arr[y-1][x-1] == nullptr) move_variants.insert(std::pair<int, int>(x - 1, y - 1));
				//up 1 . right 1 (empty)
				if (x + 1 <= size_x-1 && y - 1 >= 0 && arr[y-1][x+1] == nullptr) move_variants.insert(std::pair<int, int>(x + 1, y - 1));
				
				//up 2 . left 2 (take enemy piece)
				if (x - 2 >= 0 && y - 2 >= 0 && arr[y-2][x-2] == nullptr && arr[y-1][x-1] && arr[y-1][x-1]->side != arr[y][x]->side)
					move_variants.insert(std::pair<int, int>(x - 2, y - 2));
				//up 2 . right 2 (take enemy piece)
				if (x + 2 <= size_x-1 && y - 2 >= 0 && arr[y - 2][x + 2] == nullptr && arr[y - 1][x + 1] && arr[y - 1][x + 1]->side != arr[y][x]->side)
					move_variants.insert(std::pair<int, int>(x + 2, y - 2));
			}

			//black (or king)
			if (arr[y][x]->side == Piece::Sides::black || arr[y][x]->type == Piece::Types::king)
			{
				//down 1 . left 1 (empty)
				if (x - 1 >= 0 && y + 1 <= size_y-1 && arr[y + 1][x - 1] == nullptr) move_variants.insert(std::pair<int, int>(x - 1, y + 1));
				//down 1 . right 1 (empty)
				if (x + 1 <= size_x - 1 && y + 1 <= size_y-1 && arr[y + 1][x + 1] == nullptr) move_variants.insert(std::pair<int, int>(x + 1, y + 1));
				
				//down 2 . left 2 (take enemy piece)
				if (x - 2 >= 0 && y + 2 <= size_y-1 && arr[y + 2][x - 2] == nullptr && arr[y + 1][x - 1] && arr[y + 1][x - 1]->side != arr[y][x]->side)
					move_variants.insert(std::pair<int, int>(x - 2, y + 2));
				//down 2 . right 2 (take enemy piece)
				if (x + 2 <= size_x - 1 && y + 2 <= size_y-1 && arr[y + 2][x + 2] == nullptr && arr[y + 1][x + 1] && arr[y + 1][x + 1]->side != arr[y][x]->side)
					move_variants.insert(std::pair<int, int>(x + 2, y + 2));
			}
		}
		void Press(char key)
		{
			HideCursor();
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
					if (cursor_x - 1 >= 0) cursor_x--;
					break;
				//CURSOR-RIGHT
				case 'd':
				case 'D':
					if (cursor_x + 1 <= size_x-1) cursor_x++;
					break;
				//CURSOR-UP
				case 'w':
				case 'W':
					if (cursor_y - 1 >= 0) cursor_y--;
					break;
				//CURSOR-DOWN
				case 's':
				case 'S':
					if (cursor_y + 1 <= size_y-1) cursor_y++;
					break;
				//MAKE-A-MOVE
				case 'f':
				case 'F':
					MakeAMove();
					break;
				//CANCEL-A-MOVE
				case 'x':
				case 'X':
					CancelMove();
					break;
			}
			ShowCursor();
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
		void ShowCell(int row, int col, bool ShowCursor = true)
		{
			bool IsCursor = false;
			bool IsVariant = false;

			if ((ShowCursor && row == cursor_y && col == cursor_x)
				||
				(row == start_cursor_y && col == start_cursor_x)) IsCursor = true;

			//IS VARIANT
			if (move_variants.find(std::pair<int, int>(col, row)) != move_variants.end())
				IsVariant = true;

			Colors fg, bg;
			if (row % 2 == col % 2) bg = Colors::white;
			else bg = Colors::black;

			if (arr[row][col] && arr[row][col]->side == Piece::Sides::white) fg = Colors::lwhite;
			else fg = Colors::lblack;

			SetColor(fg, bg);

			if (IsCursor)
				SetColor(Colors::cursor, bg);

			if (IsVariant)
				SetColor(Colors::variant_fg, Colors::variant_bg);

			for (int i = 0; i < cell_size_y; i++)
			{
				Cursor::set(col * cell_size_x + offset_x, row*cell_size_y + i + offset_y);
				for (int j = 0; j < cell_size_x; j++)
				{
					if (IsCursor) std::cout << cursor_symbol;
					else std::cout << ' ';
				}
			}
			SetColor(fg, bg);
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
char Board::cursor_symbol = '#';