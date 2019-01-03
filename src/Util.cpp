#include "../include/Util.hpp"
#include <iostream>

using namespace std;

int letterToNumber(char c)
{
	if (int(c) > 64 && int(c) < 73) return int(c) - 65;
	else if (int(c) > 96 && int(c) < 105) return int(c) - 97;
	else return -1;
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

vector<string> split(string s, string delimiter){
	vector<string> result;
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != string::npos) {
		token = s.substr(0, pos);
		result.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	return result;
}

void debug (string s){
	cout << s << endl;
}