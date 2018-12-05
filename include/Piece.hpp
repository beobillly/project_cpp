#ifndef PIECE
#define PIECE
#include <iostream>
#include "../src/Rank.cpp"

class Piece
{
public:
  Piece();
  Piece(bool color, Rank rank, int x, int y);
  Piece(Piece &&) = default;
  Piece(const Piece &) = default;
  Piece &operator=(Piece &&) = default;
  Piece &operator=(const Piece &) = default;
  ~Piece();
  bool getColor();
  Rank getRank();
  void show();
  void promotion(Rank RANG, bool color);
  int getPosX();
  int getPosY();

private:
  bool color;
  Rank rank;
  int x;
  int y;
};

#endif