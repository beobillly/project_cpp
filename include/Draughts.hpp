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
  void help(Player &p);
  int getLine(int pos);
  int getColumn(int pos);
  bool isMoveOk(vector<int> positions, bool eat, Piece &p);
  int taille;
  vector<tuple<int, int>> getManMoves(Piece &p, bool eat, int x, int y);
  vector<tuple<int, int>> getQueenMoves(Piece &p, bool eat, int x, int y);
  bool addQueenMove(int x, int y, bool eat, vector<tuple<int, int>> &moves, Piece &p, int depart, int direction);
  void addManMove(int x, int y, bool eat, vector<tuple<int, int>> &moves, Piece &p, int depart, int direction);
  bool addQueenMoveNext(int x, int y, vector<tuple<int, int>> &moves, Piece &p);
  vector<tuple<int, int>> eatenP(vector<int> positions);
  vector<tuple<int, int>> getMoves(Piece &p, bool eat);
  string getMoveNotation(Piece &piece, int x, int y, bool eat);
  void robotMove(Player &robot, string path);
};

#endif // !DRAUGHTS