#pragma once
#include <iostream>
#include "cursor.h"
#include "colors.h"
#include "Pieces.h"


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
	public:
		Board() {
			arr = new Piece * *[size_y];
			for (int row = 0; row < size_y; row++) {
				arr[row] = new Piece * [size_x];
			}
			player = 1;
			SetBoard();
		}
		~Board() {
			for (int row = 0; row < size_y; row++) {
				for (int col = 0; col < size_x; col++) {
					delete arr[row][col];
				}
				delete[] arr[row];
			}
			delete[] arr;
		}
		void SetBoard() {
			for (int row = 0; row < size_y; row++) {
				for (int col = 0; col < size_x; col++) {
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
		void ShowCell(int row, int col) {
			Colors fg, bg;
			if (row % 2 == col % 2) bg = Colors::white;
			else bg = Colors::black;

			if (arr[row][col] && arr[row][col]->side == Piece::Sides::white) fg = Colors::lwhite;
			else fg = Colors::lblack;

			SetColor(fg, bg);

			for (int i = 0; i < cell_size_y; i++)
			{
				Cursor::set(col * cell_size_x + offset_x, row*cell_size_y + i + offset_y);
				for (int j = 0; j < cell_size_x; j++) {
					std::cout << ' ';
				}
			}
			Cursor::set(col * cell_size_x + cell_size_x / 2 + offset_x, row * cell_size_y + cell_size_y / 2 + offset_y);
			if (arr[row][col]) std::cout << *arr[row][col];
			std::cout << std::endl;
			SetColor();
			Cursor::set(0, 0);
		}
		void Show() {
			for (int row = 0; row < size_y; row++) {
				for (int col = 0; col < size_x; col++) {
					ShowCell(row, col);
				}
				std::cout << std::endl;
			}
		}
};
int Board::size_x = 8;
int Board::size_y = 8;
int Board::cell_size_x = 5;
int Board::cell_size_y = 3;
int Board::offset_x = 1;
int Board::offset_y = 2;