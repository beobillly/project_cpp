#ifndef SQUARE
#define SQUARE
#include "Piece.hpp"

class Square
{
public:
  Square();
  Square(bool color, Piece *piece, int x, int y);
  Square(Square &&) = default;
  Square(const Square &) = default;
  Square &operator=(Square &&) = default;
  Square &operator=(const Square &) = default;
  ~Square();
  bool getColor();
  Piece *getPiece();
  int getX();
  int getY();

private:
  bool color;
  Piece *piece;
  int x;
  int y;
};

#endif