#include "../include/Square.hpp"

Square::Square()
{
}

Square::Square(Piece *piece, int x, int y)
{
    // Square::color = color;
    Piece p = Piece(piece->getColor(), piece->getRank(), 0, 0);
    Square::piece = piece;
    Square::x = x;
    Square::y = y;
}

Square::~Square()
{
}

// bool Square::getColor()
// {
//     return Square::color;
// }
Piece *Square::getPiece()
{
    return Square::piece;
}
void Square::setPiece(Piece *piece_bis)
{
    Piece p_tmp = Piece(piece_bis->getColor(), piece_bis->getRank(), 0, 0);
    Square::piece = &p_tmp;
}

int Square::getX()
{
    return Square::x;
}
int Square::getY()
{
    return Square::y;
}

void Square::show()
{

    switch (Square::piece->getRank())
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
    if (Square::piece->getColor())
    {
        std::cout << "w";
    }
    else
    {
        std::cout << "b";
    }
}
