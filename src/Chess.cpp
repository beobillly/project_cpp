#include "../include/Chess.hpp"
#include "../include/Util.hpp"
#include <fstream>
#include <ctype.h>

using namespace std;

Chess::Chess() {
	this->game_type = Game_type::ECHEC;
}

void Chess::Move(Player p, string path) {
	ofstream history;
	history.open(path);
startLoop:
	cout << p.getName << "'s turn" << endl;
	cout << "Please enter your move : " << endl;
	string move("");
	cin >> move;
	int length = move.length();
	switch (length) {
	case 2: //pawn move
	{
		int x = letterToNumber(move[0]);
		Piece pawn = p.getPawnOfFile(x);
		if (pawn.getRank() != Rank::EMPTY) {
			int y = move[1] - '1';
			if (moveOnePiece(pawn, x, y)) break;
		}
		else {
			cout << "Invalid move : No pawn found of that file" << endl;
			goto startLoop;
		}
	}
	case 3: //castling kingside + "regular move" + pawn promotion
	{
		if (move == "0-0") { //Castling kingside (right)
			if (castling(true, p)) break; 
			else {
				cout << "Invalid move"; 
				goto startLoop;
			}
		}
		else if (isMaj(move[2])) { //pawn promotion
			int x = letterToNumber(move[0]);
			Piece pawn = p.getPawnOfFile(x);
			if (pawn.getRank() != Rank::EMPTY) {
				int oldX = pawn.getPosX();
				int oldY = pawn.getPosY();
				bool color = p.getColor();
				if ((color && oldY != 6) || (!color && oldY != 1)) {
					cout << "Invalid move : The pawn of file " << move[0] << " is not available for promotion" << endl;
					goto startLoop;
				}
				board.removePiece(oldX, oldY);
				Rank r = getRankFromChar(move[2]);
				if (r == Rank::QUEEN || r == Rank::KNIGHT || r == Rank::ROOK || r == Rank::BISHOP) {
					int newY = move[1] - '1';
					Piece newPiece = Piece(color, r, oldX, newY);
					board.setPiece(newPiece, oldX, newY);
				}
				else {
					cout << "Invalid move : you cannot promote a pawn to rank " << move[2] << "." << endl;
					cout << "Please choose one of the following : Q, N, R, B" << endl;
					goto startLoop;
				}
			}
			else {
				cout << "Invalid move : No pawn found of that file" << endl;
				goto startLoop;
			}
		}
		else { //regular move
			Rank r = getRankFromChar(move[0]);
			if (r != Rank::EMPTY) {
				int x = letterToNumber(move[1]);
				int y = move[2] - '1';
				Piece piece = isMoveOk(r, board, p, x, y, false);
				if (piece.getRank() == Rank::EMPTY) {
					cout << "Invalid move : no piece found of rank " << move[0] << "able to go to " << move[1] << move[2] << endl;
					goto startLoop;
				}
				else {
					board.movePiece(piece, x, y);
					break;
				}
			}
			else {
				cout << "Invalid move : unknown rank " << move[0];
				goto startLoop; 
			}
		}
	}
	case 4: //move with capture or disambiguation
		if (move[1] == 'x') { //Capture
			Rank r = getRankFromChar(move[0]);
			if (r != Rank::EMPTY) {
				int x = letterToNumber(move[2]);
				int y = move[3] - '1';
				Piece piece = isMoveOk(r, board, p, x, y, true, -1, -1);
				if (piece.getRank() == Rank::EMPTY) {
					cout << "Invalid move : no piece found of rank " << move[0] << "able to go to " << move[1] << move[2] << endl;
					goto startLoop;
				}
				else {
					board.removePiece(x, y);
					board.movePiece(piece, x, y);
					break;
				}
			}
			else {
				cout << "Invalid move : unknown rank " << move[0];
				goto startLoop;
			}
		}
		else { // disambiguation
			int oldX = -1;
			int oldY = -1;
			if (isdigit(move[1])) oldY = move[1] - '1';
			else oldX = letterToNumber(move[1]);
			Rank r = getRankFromChar(move[0]);
			if (r != Rank::EMPTY) {
				int x = letterToNumber(move[2]);
				int y = move[3] - '1';
				Piece piece = isMoveOk(r, board, p, x, y, true, oldX, oldY);
				if (piece.getRank() == Rank::EMPTY) {
					cout << "Invalid move : no piece found of rank " << move[0] << "able to go to " << move[1] << move[2] << endl;
					goto startLoop;
				}
				else {
					board.removePiece(x, y);
					board.movePiece(piece, x, y);
					break;
				}
			}
			else {
				cout << "Invalid move : unknown rank " << move[0];
				goto startLoop;
			}
		}
	case 5: //castling queenside + capture with disambiguation
		if (move == "0-0-0") { //Castling queenside (left)
			if (castling(false, p)) break;
		}
		else { //Capture with disambiguation
			
		}
	default:
		cout << "Invalid move : unkown move " << move << endl;
		goto startLoop;
	}
	history << move << endl;
	history.close();
}

bool Chess::moveOnePiece(Piece &p, int x, int y) {
	if (coordOk(x, y)) {
		board.movePiece(p, x, y);
		return true;
	}
	return false;
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
		moveOnePiece(king, 6, row);
		Piece rook = board.getPiece(7, row);
		moveOnePiece(rook, 5, row);
	}
	else {
		Piece king = board.getPiece(4, row);
		moveOnePiece(king, 2, row);
		Piece rook = board.getPiece(0, row);
		moveOnePiece(rook, 3, row);
	}
	return true;
}

Piece Chess::isMoveOk(Rank r, Board b, Player p, int x, int y, bool eat, int oldX, int oldY) {
	return Piece(true, Rank::EMPTY, 0, 0); //a retourner en cas d'erreur
}
