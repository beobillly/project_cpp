#ifndef PLAYER
#define PLAYER
#include <iostream>
#include <string>
#include <vector>
#include "Piece.hpp"

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
	vector<Piece> allOfRank(Rank r);
	int nbOfPieces();
	Piece getFirstPiece();
	Piece getPawnOfFile(int x);
	void eatPiece(Piece toEat);
	vector<Piece> getPieces();

protected:
	string name;
	int score;
	bool color;
	vector<Piece> pieces;
};
#endif