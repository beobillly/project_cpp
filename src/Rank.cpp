#ifndef RANK
#define RANK
#include <iostream>
enum class Rank
{
    KING,
    QUEEN,
    KNIGHT,
    BISHOP,
    ROOK,
    PAWN,
    MAN,
    EMPTY
};

inline std::ostream &operator<<(std::ostream &os, Rank r)
{
    switch (r)
    {
    case Rank::KING:
        os << "KING";
        break;
    case Rank::QUEEN:
        os << "QUEEN";
        break;
    case Rank::KNIGHT:
        os << "KNIGHT";
        break;
    case Rank::BISHOP:
        os << "BISHOP";
        break;
    case Rank::ROOK:
        os << "ROOK";
        break;
    case Rank::PAWN:
        os << "PAWN";
        break;
    case Rank::MAN:
        os << "MAN";
        break;
    default:
        os << "";
        break;
    }
    return os;
}

#endif