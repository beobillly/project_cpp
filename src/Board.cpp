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
    for (int i = 0; i < Board::height; i++)
    {
        for (int j = 0; j < Board::width; j++)
        {
            Board::board[i][j].show();
        }
        std::cout << "" << '\n';
    }
}

Square Board::getSquare(int x, int y)
{
    return Board::board[x][y];
}

/*Ca bug la dedans*/

void Board::init()
{
    Board::board = new Square *[Board::height];
    for (int i = 0; i < Board::height; i++)
    {
        Board::board[i] = new Square[Board::width];
    }

    for (int i = 0; i < Board::height; i++)
    {
        for (int j = 0; j < Board::width; j++)
        {
            bool b = j % 2 == 0;
            Piece piece = Piece(b, Rank::KNIGHT);
            Square square = Square(&piece, i, j);
            Board::board[i][j].setPiece(&piece);
        }
    }
}