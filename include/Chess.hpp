#include "Game.hpp"
#ifndef CHESS
#define CHESS

class Chess : public Game {
public :
	Chess();
	void Move(Player p, string path);
	Rank getRankFromChar(char c);
	int letterToNumber(char c);
	bool coordOk(int x, int y);
	bool moveOnePiece(Piece &piece, int x, int y);
	bool castlingOk(bool side, Player p);
	bool castling(bool side, Player p);
	Piece isMoveOk(Rank r, Board b, Player p, int x, int y, bool eat, int oldX, int oldY);
};

#endif // !CHESS