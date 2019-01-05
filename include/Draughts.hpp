#include "Game.hpp"
#ifndef DRAUGHTS
#define DRAUGHTS

class Draughts : public Game
{
public:
  Draughts();
  Draughts(bool classique);
  void move(Player p, string path);
  bool hasLost(Player p);

private:
  bool checkEat(bool eat, Player p, int x, int y);
  vector<tuple<int, int>> getQueenMoves(Piece p, bool eat);
  vector<tuple<int, int>> getPawnMoves(Piece p, bool eat);
  bool coordInVector(vector<tuple<int, int>> vec, int x, int y);
  bool simulateMove(Piece toMove, int x, int y, bool eat, Player p);
  bool pawnCheck(bool color, int x, int y);
  void help(Player p);
  string getMoveNotation(Piece piece, int x, int y, bool eat, char rank);
  int getLine(int pos);
  int getColumn(int pos);
};

#endif // !DRAUGHTS