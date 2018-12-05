#include "../include/Piece.hpp"

Piece::Piece()
{
}

Piece::Piece(bool color, Rank rank, int x, int y)
{
    this->color = color;
    this->rank = rank;
	this->x = x;
	this->y = y;
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