#include "Square.hpp"
#include "Piece.hpp"
#include "Rank.cpp"
#include <iostream>

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

private:
  int width;
  int height;
  Square **board;
};
