#include "../include/Game.hpp"

Game::Game()
{
    Game_type gt = Game::ask_game_type();
    this->game_type = gt;
    Player p1 = Player(Game::ask_player_name(), false);
    Player p2 = Player(Game::ask_player_name(), true);
    this->player_black = p1;
    this->player_white = p2;
    int taille = 8;
    if (gt == Game_type::DAME_CLASSIQUE)
    {
        taille = 10;
    }
    Board board = Board(taille, taille);
    Game::board = board;
    init();
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
        init_pawn(1, true);
        init_strong(0, true);
        init_strong(Game::board.getHeight() - 1, false);
        init_pawn(Game::board.getHeight() - 2, false);
        break;
    case Game_type::DAME_CLASSIQUE:
        init_pawn_(0, false, 1);
        init_pawn_(1, false, 0);
        init_pawn_(2, false, 1);
        init_pawn_(3, false, 0);
        init_pawn_(6, true, 1);
        init_pawn_(7, true, 0);
        init_pawn_(8, true, 1);
        init_pawn_(9, true, 0);
        break;
    case Game_type::DAME_ANGLAISE:
        init_pawn_(0, false, 1);
        init_pawn_(1, false, 0);
        init_pawn_(2, false, 1);
        init_pawn_(5, true, 0);
        init_pawn_(6, true, 1);
        init_pawn_(7, true, 0);
        break;

    default:
        break;
    }
}

int main(int argc, char **argv)
{
    Game game = Game();
    game.refresh();

    std::cout << "je suis la" << '\n';
    cout << "ça marche" << endl;
    return 0;
}

Game_type Game::ask_game_type()
{
    string name("");
    bool ok_tmp = false;
    int r = std::rand() % 3 + 1;
    int res = 0;
    while (!ok_tmp)
    {
        std::cout << "A quel jeu voulez vous jouer ?\n1 -> ECHEC\n2 -> DAME_CLASSIQUE\n3 -> DAME_ANGLAISE\n4 -> ALEATOIRE\n5 -> QUITTER" << '\n';

        std::cin >> name;
        try
        {
            res = std::stoi(name);
            if (res < 1 || res > 5)
            {
                ok_tmp = false;
                std::cout << "Vous n'avez pas rentré une valeur valide (1 - 2 - 3 - 4 - 5)\nVeuillez recommencer" << '\n';
            }
            else
            {
                ok_tmp = true;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "Vous n'avez pas rentré une valeur valide (1 - 2 - 3 - 4 - 5)\nVeuillez recommencer" << '\n';
        }
    }

    switch (res)
    {

    case 1:
        return Game_type::ECHEC;
    case 2:
        return Game_type::DAME_CLASSIQUE;
    case 3:
        return Game_type::DAME_ANGLAISE;
    case 4:
        if (r == 1)
        {
            return Game_type::ECHEC;
        }
        else if (r == 2)
        {
            return Game_type::DAME_CLASSIQUE;
        }
        else
        {
            return Game_type::DAME_ANGLAISE;
        }
    case 5:
        std::cout << "Bye" << '\n';
        exit(0);

    default:
        return Game_type::EMPTY;
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

void Game::init_pawn(int pos, bool col)
{
    for (int i = 0; i < Game::board.getWidth(); i++)
    {
        Piece piece = Piece(col, Rank::PAWN, pos, i);
        Game::board.setPiece(piece, pos, i);
    }
}

void Game::init_strong(int pos, bool col)
{
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
}

void Game::init_pawn_(int pos, bool col, int start)
{
    for (int i = start; i < Game::board.getWidth(); i = i + 2)
    {
        Piece piece = Piece(col, Rank::PAWN, pos, i);
        Game::board.setPiece(piece, pos, i);
    }
}