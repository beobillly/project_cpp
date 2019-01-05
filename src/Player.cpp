#include "../include/Player.hpp"

Player::Player()
{
}

Player::Player(string name, bool color)
{
	this->name = name;
	this->color = color;
	vector<Piece> vect;
	this->pieces = vect;
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

vector<Piece> Player::getPieces()
{
	return pieces;
}

vector<Piece> Player::allOfRank(Rank r)
{
	vector<Piece> piecesofRank;
	for (int i = 0; i < int(pieces.size()); i++)
	{
		if (pieces.at(i).getRank() == r)
			piecesofRank.push_back(pieces.at(i));
	}
	return piecesofRank;
}

int Player::nbOfPieces()
{
	return pieces.size();
}

Piece Player::getFirstPiece()
{
	return pieces.at(0);
}

Piece Player::getPawnOfFile(int x)
{
	if (x < 0 || x > 7)
		return Piece(true, Rank::EMPTY, 0, 0);
	for (int i = 0; i < int(pieces.size()); i++)
	{
		Piece piece = pieces.at(i);
		if (piece.getRank() == Rank::PAWN && piece.getPosX() == x)
			return piece;
	}
	return Piece(true, Rank::EMPTY, 0, 0);
}

void Player::eatPiece(Piece toEat)
{
	vector<Piece> newPieces;
	for (Piece p : pieces)
		if (!(p.getPosX() == toEat.getPosX() && p.getPosY() == toEat.getPosY()))
			newPieces.push_back(p);
	pieces = newPieces;
}

void Player::addPiece(Piece &p)
{
	auto it = pieces.end();
	pieces.insert(it, p);
}