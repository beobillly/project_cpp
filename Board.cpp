#include "Board.hpp"

Board::Board(int width, int height)
{
    this->width = width;
    this->height = height;
    board = new Square *[height];
    for (int i = 0; i < width; i++)
    {
        board[i] = new Square[height];
        for (int j = 0; j < height; j++)
        {
            Piece piece = Piece(i % 2 == 0, Rank::EMPTY);
            board[i][j] = Square(i % 2 == 0, &piece, i, j);
        }
    }
}

Board::~Board() {}

int Board::getWidth()
{
    return Board::width;
}
int Board::getHeight()
{
    return Board::height;
}