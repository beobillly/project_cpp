#ifndef GAME
#define GAME
#include "Player.hpp"
#include "Board.hpp"
#include "../src/Game_type.cpp"
class Game
{
public:
  Game();
  Game(Game &&) = default;
  Game(const Game &) = default;
  Game &operator=(Game &&) = default;
  Game &operator=(const Game &) = default;
  ~Game();
  void init();
  Game_type ask_game_type();
  string ask_player_name();
  void refresh();
  void init_pawn(int pos, bool col);
  void init_strong(int pos, bool col);
  void init_pawn_(int pos, bool col, int start);
  Game_type getGameType();
  Board getBoard();
  Player getPlayerWhite();
  Player getPlayerBlack();

protected:
  Player player_white;
  Player player_black;
  Board board;
  Game_type game_type;
};

#endif