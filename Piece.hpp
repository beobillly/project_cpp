#include "Rank.cpp"

class Piece
{
public:
  Piece();
  Piece(bool color, Rank rank);
  Piece(Piece &&) = default;
  Piece(const Piece &) = default;
  Piece &operator=(Piece &&) = default;
  Piece &operator=(const Piece &) = default;
  ~Piece();
  bool getColor();
  Rank getRank();

private:
  bool color;
  Rank rank;
};