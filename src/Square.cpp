#include "../include/Square.hpp"

Square::Square(bool color, Piece *piece, int x, int y)
{
    this->color = color;
    this->piece = piece;
    this->x = x;
    this->y = y;
}

Square::~Square()
{
}

bool Square::getColor()
{
    return Square::color;
}
Piece *Square::getPiece()
{
    return Square::piece;
}
int Square::getX()
{
    return Square::x;
}
int Square::getY()
{
    return Square::y;
}