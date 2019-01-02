#include "Game.hpp"
#ifndef CHESS
#define CHESS

class Chess : public Game {
public :
	Chess();
	void Move(Player p, string path);
private : 
	bool checkEat(bool eat, Player p, int x, int y);
	vector<tuple<int, int>> getKingMoves(Piece p, bool eat);
	vector<tuple<int, int>> getQueenMoves(Piece p, bool eat);
	vector<tuple<int, int>> getKnightMoves(Piece p, bool eat);
	vector<tuple<int, int>> getBishopMoves(Piece p, bool eat);
	vector<tuple<int, int>> getRookMoves(Piece p, bool eat);
	vector<tuple<int, int>> getPawnMoves(Piece p, bool eat);
	bool castlingOk(bool side, Player p);
	bool castling(bool side, Player p);
	Piece isMoveOk(Rank r, Player p, int x, int y, bool eat, int oldX, int oldY);
	bool isChecked(Player p, int x, int y);
	Piece getRightPiece(Rank r, vector<Piece> piecesToCheck, Player p, int x, int y, bool eat);
	vector<Piece> getPiecesToCheck(Rank r, Player p, int x, int y, int oldX, int oldY);
	bool coordInVector(vector<tuple<int, int>> vec, int x, int y);
	vector<tuple<int, int>> getPossibleKingMoves();
	vector<tuple<int, int>> getPossibleKnightgMoves();
	bool addRookOrBishopMove(int x, int y, bool eat, vector<tuple<int, int>> moves);
	bool simulateMove(Piece toMove, int x, int y, bool eat, Player p);
	bool queenOrRookCheck(bool color, int x, int y);
	bool queenOrBishopCheck(bool color, int x, int y);
	bool knightCheck(bool color, int x, int y);
	bool pawnCheck(bool color, int x, int y);
	void help(Player p);
	string getMoveNotation(Piece piece, int x, int y, bool eat, char rank);
};

#endif // !CHESS