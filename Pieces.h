#pragma once
#include <ostream>

struct Piece {
	enum Types {
		man = 'O', king = '0'
	};
	enum Sides {
		white, black
	};
	Types type;
	Sides side;
	Piece(Types type=Types::man, Sides side=Sides::white) : type{ type }, side{ side } {}

	friend std::ostream& operator<<(std::ostream& out, const Piece& piece) {
		out << char(piece.type);
		return out;
	}
};