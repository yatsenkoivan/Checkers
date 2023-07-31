#pragma once
#include <iostream>
#include "cursor.h"
#include "colors.h"
#include "Pieces.h"


class Board {
	private:
		static int size_x;
		static int size_y;
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
		void Show() {
			for (int row = 0; row < size_y; row++) {
				for (int col = 0; col < size_x; col++) {
					if (arr[row][col]) std::cout << *arr[row][col];
					else std::cout << ' ';
				}
				std::cout << std::endl;
			}
		}
};
int Board::size_x = 8;
int Board::size_y = 8;