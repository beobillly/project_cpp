#include "../include/Util.hpp"

int letterToNumber(char c)
{
	if (int(c) > 64 && int(c) < 73)
		return int(c) - 65;
	if (int(c) > 96 && int(c) < 105)
		return int(c) - 97;
	else
		return -1;
}

Rank getRankFromChar(char c)
{
	switch (c)
	{
	case 'K':
		return Rank::KING;
	case 'Q':
		return Rank::QUEEN;
	case 'B':
		return Rank::BISHOP;
	case 'N':
		return Rank::KNIGHT;
	case 'R':
		return Rank::ROOK;
	default:
		return Rank::EMPTY;
	}
}

bool isMaj(char c)
{
	return (int(c) > 64 && int(c) < 90);
}

bool isMin(char c) {
	return (int(c) > 69 && int(c) < 105);
}

bool coordOk(int height, int length, int x, int y) {
	return (x >= 0 && x <= length && y >= 0 && y <= height);
}