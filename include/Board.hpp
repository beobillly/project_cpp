#include "Piece.hpp"
#include "Piece.hpp"
#include "../src/Rank.cpp"
#include <iostream>
#ifndef BOARD
#define BOARD

using namespace std;

class Board
{
public:
  Board();
  Board(int width, int height);
  Board(Board &&) = default;
  Board(const Board &) = default;
  Board &operator=(Board &&) = default;
  Board &operator=(const Board &) = default;
  ~Board();
  int getWidth();
  int getHeight();
  void show();
  Piece& getPiece(int x, int y);
  void setPiece(Piece &piece, int x, int y);
  void removePiece(int x, int y);
  void init();
  void movePiece(Piece &piece, int x, int y);

private:
  int width;
  int height;
  Piece **board;
};

#endif