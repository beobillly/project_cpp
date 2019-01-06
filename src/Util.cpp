#include "../include/Util.hpp"
#include <iostream>

using namespace std;

int letterToNumber(char c)
{
	if (int(c) > 64 && int(c) < 73)
		return int(c) - 65;
	else if (int(c) > 96 && int(c) < 105)
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

bool isMin(char c)
{
	return (int(c) > 69 && int(c) < 105);
}

bool coordOk(int height, int length, int x, int y)
{
	return (x >= 0 && x <= length && y >= 0 && y <= height);
}

vector<string> splitString(string s, char delimiter)
{
	vector<string> result;
	string token("");
	for (int i = 0; i<s.size(); i++){
		if (s[i] == delimiter){
			result.insert(result.end(), token);
			token = "";
		} else token += s[i];
	}
	result.insert(result.end(), token);
	return result;
}

void debug(string s)
{
	cout << s << endl;
}

char rankToChar(Rank r)
{
	switch (r)
	{
	case Rank::KING:
		return 'K';
	case Rank::PAWN:
		return '\0';
	case Rank::ROOK:
		return 'R';
	case Rank::BISHOP:
		return 'B';
	case Rank::KNIGHT:
		return 'N';
	case Rank::QUEEN:
		return 'Q';
	default:
		return '\0';
	}
}