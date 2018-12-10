#include "../include/Chess.hpp"

using namespace std;

Chess::Chess() {
	this->game_type = Game_type::ECHEC;
}

void Chess::Move(Player p) {
startLoop:
	cout << "Please enter your move : " << endl;
	string move("");
	cin >> move;
	int length = move.length();
	switch (length) {
	case 2:
	{
		int x = letterToNumber(move[0]);
		Piece* pawn = p.getPawnOfFile(x);
		if (pawn != nullptr) {
			int y = move[1] - '1';
			if (moveOnePiece(pawn, x, y)) break;
		}
	}
	case 3:
	{
		if (move == "0-0") { //Castling kingside (right)
			if (castling(true, p)) break;
		}
		else if (move == "0-0-0") { //Castling queenside (left)
			if (castling(false, p)) break;
		}
		else {
			Rank r = getRankFromChar(move[0]);
			if (r != Rank::EMPTY) {
				vector<Piece*> pieces = p.allOfRank(r);
				if (pieces.size() == 1) {
					Piece * piece = pieces.at(0);
					int x = letterToNumber(move[1]);
					int y = move[2] - '1';
					if (moveOnePiece(piece, x, y)) break;
				}
			}
		}
	}
	case 4:
		break;
	default:
		cout << "Invalid move" << endl;
		goto startLoop;
	}
}

bool Chess::moveOnePiece(Piece* p, int x, int y) {
	if (coordOk(x, y)) {
		board.movePiece(p, x, y);
		return true;
	}
	return false;
}

Rank Chess::getRankFromChar(char c) {
	switch (c) {
	case 'K': return Rank::KING;
	case 'Q': return Rank::QUEEN;
	case 'B': return Rank::BISHOP;
	case 'N': return Rank::KNIGHT;
	case 'R': return Rank::ROOK;
	default: return Rank::EMPTY;
	}
}

int Chess::letterToNumber(char c) {
	if (int(c) > 96 && int(c) < 105) return int(c) - 97;
	else return -1;
}

bool Chess::coordOk(int x, int y) {
	//add return false if the move puts the king in check
	return (x >= 0 && x <= 7 && y >= 0 && y <= 7);
}

bool Chess::castlingOk(bool side, Player p) { // side = true -> kingside
	//To add : 
	//return false if at least one of the pieces have moved
	//return false if king is in check
	//return false if the king passes through a square that is attacked by an enemy piece
	if (side) {
		if (p.getColor == true && board.getPiece(5, 7).getRank == Rank::EMPTY && board.getPiece(6, 7).getRank == Rank::EMPTY) return true;
		else if (p.getColor == false && board.getPiece(5, 0).getRank == Rank::EMPTY && board.getPiece(6, 0).getRank == Rank::EMPTY) return true;
		else return false;
	}
	else {
		if (p.getColor == true && board.getPiece(1, 7).getRank == Rank::EMPTY && board.getPiece(2, 7).getRank == Rank::EMPTY && board.getPiece(3, 7).getRank == Rank::EMPTY) return true;
		else if (p.getColor == false && board.getPiece(1, 0).getRank == Rank::EMPTY && board.getPiece(2, 0).getRank == Rank::EMPTY && board.getPiece(3, 0).getRank == Rank::EMPTY) return true;
		else return false;
	}
}

bool Chess::castling(bool side, Player p) {
	if (!castlingOk(side, p)) return false;
	int row = 7;
	if (p.getColor) row = 0;
	if (side) {
		Piece king = board.getPiece(4, row);
		moveOnePiece(&king, 6, row);
		Piece rook = board.getPiece(7, row);
		moveOnePiece(&rook, 5, row);
	}
	else {
		Piece king = board.getPiece(4, row);
		moveOnePiece(&king, 2, row);
		Piece rook = board.getPiece(0, row);
		moveOnePiece(&rook, 3, row);
	}
	return true;
}
