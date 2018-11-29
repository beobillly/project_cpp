#include "../include/Piece.hpp"

Piece::Piece()
{
}

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

void Piece::promotion(Rank RANG, bool color)
{
    Piece::rank = RANG;
    Piece::color = color;
}