#ifndef PLAYER
#define PLAYER
#include <iostream>
#include <string>

using namespace std;

class Player
{
public:
  Player();
  Player(string name, bool color);
  Player(Player &&) = default;
  Player(const Player &) = default;
  Player &operator=(Player &&) = default;
  Player &operator=(const Player &) = default;
  ~Player();
  string getName();
  int getScore();
  bool getColor();

protected:
  string name;
  int score;
  bool color;
};
#endif