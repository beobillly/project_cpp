#include "Game.hpp"
#ifndef CHESS
#define CHESS

class Chess : public Game {
public :
	Chess();
	void Move(Player p);
	Rank getRankFromChar(char c);
	int letterToNumber(char c);
	bool coordOk(int x, int y);
	bool moveOnePiece(Piece * piece, int x, int y);
	bool castlingOk(bool side, Player p);
	bool castling(bool side, Player p);
};

#endif // !CHESS