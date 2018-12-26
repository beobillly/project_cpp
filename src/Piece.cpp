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
        std::cout << "K";
        break;
    case Rank::QUEEN:
        std::cout << "Q";
        break;
    case Rank::KNIGHT:
        std::cout << "T";
        break;
    case Rank::BISHOP:
        std::cout << "B";
        break;
    case Rank::ROOK:
        std::cout << "R";
        break;
    case Rank::PAWN:
        std::cout << "P";
        break;
    case Rank::MAN:
        std::cout << "x";
        break;
    case Rank::EMPTY:
        std::cout << " ";
        break;

    default:
        std::cout << " ";
        break;
    }
    if (Piece::rank != Rank::EMPTY)
    {
        if (Piece::color)
        {
            std::cout << "w";
        }
        else
        {
            std::cout << "b";
        }
    }
    else
    {
        std::cout << " ";
    }
}

void Piece::promotion(Rank RANG, bool color)
{
    Piece::rank = RANG;
    Piece::color = color;
}

int Piece::getPosX()
{
    return Piece::x;
}

int Piece::getPosY()
{
    return Piece::y;
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