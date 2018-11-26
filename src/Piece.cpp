#include "../include/Piece.hpp"

Piece::Piece(bool color, Rank rank)
{
    this->color = color;
    this->rank = rank;
}

Piece::~Piece()
{
}

bool Piece::getColor()
{
    return Piece::color;
}
Rank Piece::getRank()
{
    return Piece::rank;
}