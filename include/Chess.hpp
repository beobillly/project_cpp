#include "Game.hpp"
#ifndef CHESS
#define CHESS

class Chess : public Game {
public :
	Chess();
	void Move(Player p, string path);
	bool castlingOk(bool side, Player p);
	bool castling(bool side, Player p);
	Piece isMoveOk(Rank r, Board b, Player p, int x, int y, bool eat, int oldX, int oldY);
	bool isChecked(Player p, Board b, int x, int y);
};

#endif // !CHESS