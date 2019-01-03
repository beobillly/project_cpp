#include "../include/Game.hpp"

Game::Game()
{
    // Game_type gt = Game::ask_game_type();
    // this->game_type = gt;
    Player p1 = Player(Game::ask_player_name(), true);
    Player p2 = Player(Game::ask_player_name(), false);
    this->player_white = p1;
    this->player_black = p2;
    
}

Game::~Game()
{
}

void Game::init()
{
    Game::board.init();

    switch (Game::game_type)
    {
    case Game_type::ECHEC:
        init_pawn(1, player_white);
        init_strong(0, player_white);
        init_strong(Game::board.getHeight() - 1, player_black);
        init_pawn(Game::board.getHeight() - 2, player_black);
        break;
    case Game_type::DAME_CLASSIQUE:
        init_pawn_(0, player_black, 1);
        init_pawn_(1, player_black, 0);
        init_pawn_(2, player_black, 1);
        init_pawn_(3, player_black, 0);
        init_pawn_(6, player_white, 1);
        init_pawn_(7, player_white, 0);
        init_pawn_(8, player_white, 1);
        init_pawn_(9, player_white, 0);
        break;
    case Game_type::DAME_ANGLAISE:
        init_pawn_(0, player_black, 1);
        init_pawn_(1, player_black, 0);
        init_pawn_(2, player_black, 1);
        init_pawn_(5, player_white, 0);
        init_pawn_(6, player_white, 1);
        init_pawn_(7, player_white, 0);
        break;

    default:
        break;
    }
}

string Game::ask_player_name()
{
    std::cout << "Comment voulez vous vous appeler ?" << '\n';
    string name("");
    std::cin >> name;
    return name;
}

void Game::refresh()
{
    Game::board.show();
}

void Game::init_pawn(int pos, Player &p)
{
    bool col = p.getColor();
    for (int i = 0; i < Game::board.getWidth(); i++)
    {
        Piece piece = Piece(col, Rank::PAWN, pos, i);
        Game::board.setPiece(piece, pos, i);
        p.addPiece(piece);
    }
}

void Game::init_strong(int pos, Player &p)
{
    bool col = p.getColor();
    Piece rookl = Piece(col, Rank::ROOK, pos, 0);
    Piece knightl = Piece(col, Rank::KNIGHT, pos, 1);
    Piece bishopl = Piece(col, Rank::BISHOP, pos, 2);
    Piece queen = Piece(col, Rank::QUEEN, pos, 3);
    Piece king = Piece(col, Rank::KING, pos, 4);
    Piece bishopr = Piece(col, Rank::BISHOP, pos, 5);
    Piece knightr = Piece(col, Rank::KNIGHT, pos, 6);
    Piece rookr = Piece(col, Rank::ROOK, pos, 7);

    Game::board.setPiece(rookl, pos, 0);
    Game::board.setPiece(knightl, pos, 1);
    Game::board.setPiece(bishopl, pos, 2);
    Game::board.setPiece(queen, pos, 3);
    Game::board.setPiece(king, pos, 4);
    Game::board.setPiece(bishopr, pos, 5);
    Game::board.setPiece(knightr, pos, 6);
    Game::board.setPiece(rookr, pos, 7);

    p.addPiece(rookl);
    p.addPiece(knightl);
    p.addPiece(bishopl);
    p.addPiece(queen);
    p.addPiece(king);
    p.addPiece(bishopr);
    p.addPiece(knightr);
    p.addPiece(rookr);
}

void Game::init_pawn_(int pos, Player &p, int start)
{
    bool col = p.getColor();
    for (int i = start; i < Game::board.getWidth(); i = i + 2)
    {
        Piece piece = Piece(col, Rank::PAWN, pos, i);
        Game::board.setPiece(piece, pos, i);
        p.addPiece(piece);
    }
}

Board Game::getBoard()
{
    return board;
}
Player & Game::getPlayerWhite()
{
    return player_white;
}
Player & Game::getPlayerBlack()
{
    return player_black;
}

Game_type Game::getGameType()
{
    return game_type;
}