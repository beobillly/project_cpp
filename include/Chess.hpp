#include "Game.hpp"
#ifndef CHESS
#define CHESS

class Chess : public Game
{
  public:
	Chess();
	void Move(Player &p,string move, string path);
	bool checkMate(Player &p);
    void test();

private:
	bool checkEat(bool eat, Player &p, int x, int y);
	vector<tuple<int, int>> getKingMoves(Piece &p, bool eat);
	vector<tuple<int, int>> getQueenMoves(Piece &p, bool eat);
	vector<tuple<int, int>> getKnightMoves(Piece &p, bool eat);
	vector<tuple<int, int>> getBishopMoves(Piece &p, bool eat);
	vector<tuple<int, int>> getRookMoves(Piece &p, bool eat);
	vector<tuple<int, int>> getPawnMoves(Piece &p, bool eat);
	vector<tuple<int, int>> getMoves(Piece &p, bool eat);
	bool castlingOk(bool side, Player &p);
	bool castling(bool side, Player &p);
	Piece isMoveOk(Rank r, Player &p, int x, int y, bool eat, int oldX, int oldY);
	bool isChecked(Player &p, int x, int y);
	Piece getRightPiece(vector<Piece> &piecesToCheck, Player &p, int x, int y, bool eat);
	vector<Piece> getPiecesToCheck(Rank r, Player p, int x, int y, int oldX, int oldY);
	bool coordInVector(vector<tuple<int, int>> vec, int x, int y);
	vector<tuple<int, int>> getPossibleKingMoves();
	vector<tuple<int, int>> getPossibleKnightMoves();
	bool addRookOrBishopMove(int x, int y, bool eat, vector<tuple<int, int>> &moves, Piece &p);
	bool simulateMove(Piece &toMove, int x, int y, bool eat, Player &p);
	bool queenOrRookCheck(bool color, int x, int y);
	bool queenOrBishopCheck(bool color, int x, int y);
	bool knightCheck(bool color, int x, int y);
	bool pawnCheck(bool color, int x, int y);
	void help(Player &p);
	string getMoveNotation(Piece &piece, int x, int y, bool eat);
	void robotMove(Player &robot, string path);
	void showPieces(Player &p);
	void readFile(string path);
};

#endif // !CHESS