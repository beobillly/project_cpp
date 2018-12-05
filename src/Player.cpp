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

vector<Piece> Player::getPieces() {
	return pieces;
}

*Piece Player::getPiece(Rank r) {
	if (numberOfRank(r) != 1) return nullptr;;
	else {
		for (int i = 0; i < pieces.size(); i++) {
			if (pieces.at(i).getRank == r) return &piece.at(i);
		}
	}
}

*Piece Player::getPiece(Rank r, int x, int y) {

}

int Player::numberOfRank(Rank r) {
	int count = 0;
	for (int i = 0; i < pieces.size(); i++) {
		if (pieces.at(i).getRank == r) count++;
	}
	return count;
}