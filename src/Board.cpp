#include "../include/Board.hpp"

Board::Board()
{
}

Board::Board(int width, int height)
{
    this->width = width;
    this->height = height;
    init();
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

void Board::show()
{
    std::cout << "   ";
    for (int j = 0; j < Board::width; j++)
    {
        std::cout << char(j + 97) << " ";
    }
    std::cout << "" << '\n';
    for (int i = 0; i < Board::height; i++)
    {
        std::cout << i;
        std::cout << "  ";
        for (int j = 0; j < Board::width; j++)
        {
            Board::board[i][j].show();
            std::cout << " ";
        }
        std::cout << "  ";
        std::cout << i;
        std::cout << "" << '\n';
    }
    std::cout << "   ";
    for (int j = 0; j < Board::width; j++)
    {
        std::cout << char(j + 97) << " ";
    }
    std::cout << "" << '\n';
}

Piece Board::getPiece(int x, int y)
{
    return Board::board[x][y];
}

void Board::setPiece(Piece &piece, int x, int y)
{
    Board::board[x][y] = piece;
}

void Board::removePiece(int x, int y)
{
    board[x][y] = Piece(true, Rank::EMPTY, x, y);
}

void Board::init()
{
    Board::board = new Piece *[Board::height];
    for (int i = 0; i < Board::height; i++)
    {
        Board::board[i] = new Piece[Board::width];
    }

    for (int i = 0; i < Board::height; i++)
    {
        for (int j = 0; j < Board::width; j++)
        {
            bool b = j % 2 == 0;
            Piece piece = Piece(b, Rank::EMPTY, i, j);
            Board::board[i][j] = piece;
        }
    }
}

void Board::movePiece(Piece &piece, int x, int y)
{
    int oldX = piece.getPosX();
    int oldY = piece.getPosY();
    removePiece(oldX, oldY);
    board[x][y] = piece;
    piece.setPos(x, y);
    piece.setInitialPosToFalse();
}