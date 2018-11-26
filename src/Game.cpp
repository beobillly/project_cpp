#include "../include/Game.hpp"

Game::Game()
{
    Player player_white = Player("", true);
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