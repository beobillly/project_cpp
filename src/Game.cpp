#include "../include/Game.hpp"

Game::Game()
{
    Player player_white = Player(std::__cxx11::string(""), true);
    Player player_black = Player("", false);
    const int TAILLE_FR = 10;
    Board board = Board(TAILLE_FR,TAILLE_FR);
    
}

Game::~Game()
{
}


int main(int argc,char **argv)
{
    return 0;
}