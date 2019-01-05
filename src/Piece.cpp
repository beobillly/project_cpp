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
    this->isInitialPos = true;
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

void Piece::show()
{

    switch (Piece::rank)
    {
    case Rank::KING:
        if (color)
            std::cout << "\u2654";
        else
            std::cout << "\u265A";
        break;
    case Rank::QUEEN:
        if (color)
            std::cout << "\u2655";
        else
            std::cout << "\u265B";
        break;
    case Rank::KNIGHT:
        if (color)
            std::cout << "\u2658";
        else
            std::cout << "\u265E";
        break;
    case Rank::BISHOP:
        if (color)
            std::cout << "\u2657";
        else
            std::cout << "\u265D";
        break;
    case Rank::ROOK:
        if (color)
            std::cout << "\u2656";
        else
            std::cout << "\u265C";
        break;
    case Rank::PAWN:
        if (color)
            std::cout << "\u2659";
        else
            std::cout << "\u265F";
        break;
    case Rank::MAN:
        if (color)
            std::cout << "\u26C2";
        else
            std::cout << "\u26C0";
        break;
    case Rank::EMPTY:
        std::cout << "\u2003";
        break;
    }
}

void Piece::promotion(Rank RANG, bool color)
{
    Piece::rank = RANG;
    Piece::color = color;
}

int Piece::getPosX()
{
    return x;
}

int Piece::getPosY()
{
    return y;
}

void Piece::setPos(int x, int y)
{
    this->x = x;
    this->y = y;
}

bool Piece::getInitialPos()
{
    return Piece::isInitialPos;
}

void Piece::setInitialPosToFalse()
{
    Piece::isInitialPos = false;
}