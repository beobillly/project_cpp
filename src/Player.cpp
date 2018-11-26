#include "../include/Player.hpp"

Player::Player()
{
}

Player::Player(string name, bool color)
{
    this->name = name;
    this->color = color;
    this->score = 0;
}

Player::~Player() {}

string Player::getName()
{
    return Player::name;
}

int Player::getScore()
{
    return Player::score;
}

bool Player::getColor()
{
    return Player::color;
}