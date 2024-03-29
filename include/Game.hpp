#ifndef GAME
#define GAME
#include "Player.hpp"
#include "Board.hpp"
#include "../src/Game_type.cpp"
#include "../include/Bot.hpp"
#include "../include/Util.hpp"
#include <fstream>
#include <ctype.h>
#include <tuple>
#include <vector>
#include <algorithm>
#include "../include/Bot.hpp"
#include <time.h>

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
  void init_pawn(int pos, Player &p);
  void init_strong(int pos, Player &p);
  void init_man(int pos, Player &p, int start);
  Game_type getGameType();
  Board getBoard();
  Player &getPlayerWhite();
  Player &getPlayerBlack();
  void readFile(string path);
  virtual void move (Player &p, string move, string path) = 0;

protected:
  Player player_white;
  Player player_black;
  Board board;
  Game_type game_type;
};

#endif