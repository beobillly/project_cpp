#ifndef GAME
#define GAME
#include "Player.hpp"
#include "Board.hpp"

class Game
{
  public:
    Game();
    Game(Game &&) = default;
    Game(const Game &) = default;
    Game &operator=(Game &&) = default;
    Game &operator=(const Game &) = default;
    ~Game();
    
  private:
    Player player_white;
    Player player_black;
    Board board;
};

#endif