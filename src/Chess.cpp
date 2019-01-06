#include "../include/Chess.hpp"
#include "../include/Util.hpp"
#include <fstream>
#include <ctype.h>
#include <tuple>
#include <vector>
#include <algorithm>
#include "../include/Bot.hpp"
#include <time.h>

using namespace std;

Chess::Chess()
{
	this->game_type = Game_type::ECHEC;
	int taille = 8;
	Board board = Board(taille, taille);
	this->board = board;
	init();
}

void Chess::Move(Player &p, string path)
{
	if (p.getName() == "Robot")
	{
		robotMove(p, path);
		return;
	}
	ofstream history;
	history.open(path);
	Player otherPlayer = player_black;
	if (!p.getColor())
		otherPlayer = player_white;
startLoop:
	cout << p.getName() << "'s turn" << endl;
	cout << "Please enter your move : " << endl;
	string move("");
	cin >> move;
	if (move == "help")
	{
		help(p);
		goto startLoop;
	}
	int length = move.length();
	switch (length)
	{
	case 2: //pawn move
	{
		int y = letterToNumber(move[0]);
		int x = '8' - move[1];
		Piece pawn = isMoveOk(Rank::PAWN, p, x, y, false, -1, -1);
		if (pawn.getRank() != Rank::EMPTY)
		{
			p.eatPiece(pawn);
			board.movePiece(pawn, x, y);
			p.addPiece(pawn);
			break;
		}
		else
			goto startLoop;
	}
	case 3: //castling kingside + "regular move" + pawn promotion
	{
		if (move == "0-0")
		{ //Castling kingside (right)
			if (castling(true, p))
				break;
			else
			{
				cout << "Invalid move";
				goto startLoop;
			}
		}
		else if (isMaj(move[2]))
		{ //pawn promotion
			int y = letterToNumber(move[0]);
			int x = '8' - move[1];
			Piece pawn = isMoveOk(Rank::PAWN, p, x, y, false, -1, -1);
			if (pawn.getRank() != Rank::EMPTY)
			{
				int oldX = pawn.getPosX();
				int oldY = pawn.getPosY();
				bool color = p.getColor();
				if ((color && oldY != 6) || (!color && oldY != 1))
				{
					cout << "Invalid move : The pawn of file " << move[0] << " is not available for promotion" << endl;
					goto startLoop;
				}
				board.removePiece(oldX, oldY);
				Rank r = getRankFromChar(move[2]);
				if (r == Rank::QUEEN || r == Rank::KNIGHT || r == Rank::ROOK || r == Rank::BISHOP)
				{
					Piece newPiece = Piece(color, r, x, y);
					board.setPiece(newPiece, x, y);
				}
				else
				{
					cout << "Invalid move : you cannot promote a pawn to rank " << move[2] << "." << endl;
					cout << "Please choose one of the following : Q, N, R, B" << endl;
					goto startLoop;
				}
			}
			else
				goto startLoop;
		}
		else
		{ //regular move
			Rank r = getRankFromChar(move[0]);
			if (r != Rank::EMPTY)
			{
				int y = letterToNumber(move[1]);
				int x = '8' - move[2];
				Piece piece = isMoveOk(r, p, x, y, false, -1, -1);
				if (piece.getRank() == Rank::EMPTY)
					goto startLoop;
				else
				{
					p.eatPiece(piece);
					board.movePiece(piece, x, y);
					p.addPiece(piece);
					break;
				}
			}
			else
			{
				cout << "Invalid move : unknown rank " << move[0] << endl;
				goto startLoop;
			}
		}
	}
	case 4:
	{ //move with capture or disambiguation
		if (move[1] == 'x')
		{ //Capture
			int y = letterToNumber(move[2]);
			int x = '8' - move[3];
			if (isMin(move[0]))
			{ //Pawn capture
				int oldY = letterToNumber(move[0]);
				Piece piece = isMoveOk(Rank::PAWN, p, x, y, true, -1, oldY);
				if (piece.getRank() == Rank::EMPTY)
					goto startLoop;
				else
				{
					otherPlayer.eatPiece(board.getPiece(x, y));
					p.eatPiece(piece);
					board.movePiece(piece, x, y);
					p.addPiece(piece);
					break;
				}
			}
			else
			{
				Rank r = getRankFromChar(move[0]);
				if (r != Rank::EMPTY)
				{
					Piece piece = isMoveOk(r, p, x, y, true, -1, -1);
					if (piece.getRank() == Rank::EMPTY)
						goto startLoop;
					else
					{
						otherPlayer.eatPiece(board.getPiece(x, y));
						p.eatPiece(piece);
						board.movePiece(piece, x, y);
						p.addPiece(piece);
						break;
					}
				}
				else
				{
					cout << "Invalid move : unknown rank " << move[0] << endl;
					goto startLoop;
				}
			}
		}
		else
		{ // disambiguation
			int oldX = -1;
			int oldY = -1;
			if (isdigit(move[1]))
				oldX = '8' - move[1];
			else
				oldY = letterToNumber(move[1]);
			Rank r = getRankFromChar(move[0]);
			if (r != Rank::EMPTY)
			{
				int y = letterToNumber(move[2]);
				int x = '8' - move[3];
				Piece piece = isMoveOk(r, p, x, y, false, oldX, oldY);
				if (piece.getRank() == Rank::EMPTY)
					goto startLoop;
				else
				{
					p.eatPiece(piece);
					board.movePiece(piece, x, y);
					p.addPiece(piece);
					break;
				}
			}
			else
			{
				cout << "Invalid move : unknown rank " << move[0] << endl;
				goto startLoop;
			}
		}
	}
	case 5:
	{ //castling queenside + capture with disambiguation + double disambiguation
		if (move == "0-0-0")
		{ //Castling queenside (left)
			if (castling(false, p))
				break;
		}
		else if (move[2] == 'x')
		{ //Capture with disambiguation
			int y = letterToNumber(move[3]);
			int x = '8' - move[4];
			int oldX = -1;
			int oldY = -1;
			if (isdigit(move[1]))
				oldX = '8' - move[1];
			else
				oldY = letterToNumber(move[1]);
			Rank r = getRankFromChar(move[0]);
			if (r != Rank::EMPTY)
			{
				Piece piece = isMoveOk(r, p, x, y, true, oldX, oldY);
				if (piece.getRank() == Rank::EMPTY)
					goto startLoop;
				else
				{
					otherPlayer.eatPiece(board.getPiece(x, y));
					p.eatPiece(piece);
					board.movePiece(piece, x, y);
					p.addPiece(piece);
					break;
				}
			}
			else
			{
				cout << "Invalid move : unknown rank " << move[0] << endl;
				goto startLoop;
			}
		}
		else
		{ //Double disambiguation
			int y = letterToNumber(move[3]);
			int x = '8' - move[4];
			int oldY = letterToNumber(move[1]);
			int oldX = '8' - move[2];
			Rank r = getRankFromChar(move[0]);
			if (r != Rank::EMPTY)
			{
				Piece piece = isMoveOk(r, p, x, y, false, oldX, oldY);
				if (piece.getRank() == Rank::EMPTY)
					goto startLoop;
				else
				{
					p.eatPiece(piece);
					board.movePiece(piece, x, y);
					p.addPiece(piece);
					break;
				}
			}
			else
			{
				cout << "Invalid move : unknown rank " << move[0] << endl;
				goto startLoop;
			}
		}
	}
	case 6: //capture with double disambiguation
	{
		if (move[3] == 'x')
		{
			int y = letterToNumber(move[4]);
			int x = '8' - move[5];
			int oldY = letterToNumber(move[1]);
			int oldX = '8' - move[2];
			Rank r = getRankFromChar(move[0]);
			if (r != Rank::EMPTY)
			{
				Piece piece = isMoveOk(r, p, x, y, true, oldX, oldY);
				if (piece.getRank() == Rank::EMPTY)
					goto startLoop;
				else
				{
					otherPlayer.eatPiece(board.getPiece(x, y));
					p.eatPiece(piece);
					board.movePiece(piece, x, y);
					p.addPiece(piece);
					break;
				}
			}
			else
			{
				cout << "Invalid move : unknown rank " << move[0] << endl;
				goto startLoop;
			}
		}
	}
	default:
		cout << "Invalid move : unkown move " << move << endl;
		goto startLoop;
	}
	history << move << endl;
	history.close();
}

bool Chess::castlingOk(bool side, Player &p)
{ // side = true -> kingside
	//To add :
	//return false if king is in check
	//return false if the king passes through a square that is attacked by an enemy piece
	if (side)
	{
		if (p.getColor() == true && board.getPiece(5, 7).getRank() == Rank::EMPTY && board.getPiece(6, 7).getRank() == Rank::EMPTY)
			return true;
		else if (p.getColor() == false && board.getPiece(5, 0).getRank() == Rank::EMPTY && board.getPiece(6, 0).getRank() == Rank::EMPTY)
			return true;
		else
			return false;
	}
	else
	{
		if (p.getColor() == true && board.getPiece(1, 7).getRank() == Rank::EMPTY && board.getPiece(2, 7).getRank() == Rank::EMPTY && board.getPiece(3, 7).getRank() == Rank::EMPTY)
			return true;
		else if (p.getColor() == false && board.getPiece(1, 0).getRank() == Rank::EMPTY && board.getPiece(2, 0).getRank() == Rank::EMPTY && board.getPiece(3, 0).getRank() == Rank::EMPTY)
			return true;
		else
			return false;
	}
}

bool Chess::castling(bool side, Player &p)
{
	if (!castlingOk(side, p))
		return false;

	int row = 7;
	if (p.getColor())
		row = 0;

	Piece king;
	Piece rook;
	int xKing = 6;
	int xRook = 7;
	if (side)
	{
		king = board.getPiece(4, row);
		rook = board.getPiece(7, row);
	}
	else
	{
		king = board.getPiece(4, row);
		rook = board.getPiece(0, row);
		xKing = 2;
		xRook = 3;
	}
	if (king.getInitialPos() == false || rook.getInitialPos() == false)
		return false;
	board.movePiece(king, xKing, row);
	board.movePiece(rook, xRook, row);
	return true;
}

vector<Piece> Chess::getPiecesToCheck(Rank r, Player p, int x, int y, int oldX, int oldY)
{
	vector<Piece> piecesToCheck;
	auto it = piecesToCheck.end();
	if (oldX != -1 && oldY != -1)
	{
		if (oldX == x && oldY == y)
		{
			cout << "Invalid move : You can't move to where you already are" << endl;
			return piecesToCheck;
		}
		Piece piece = board.getPiece(oldX, oldY);
		if (piece.getRank() == Rank::EMPTY)
		{
			char file = x + 65;
			cout << "Invalid move : " << file << y + 1 << " is empty" << endl;
			return piecesToCheck;
		}
		else if (piece.getColor() != p.getColor())
		{
			cout << "Invalid move : not your piece !" << endl;
			return piecesToCheck;
		}
		else
			piecesToCheck.insert(it, (piece));
	}
	else if (oldX != -1 || oldY != -1)
	{
		bool xSpecified = false;
		if (oldX != -1)
			xSpecified = true;
		vector<Piece> piecesOfRank = p.allOfRank(r);
		if (piecesOfRank.empty())
		{
			cout << "Invalid move : You don't have any pieces of rank " << r << " left" << endl;
			return piecesToCheck;
		}
		for (Piece p : piecesOfRank)
		{
			it = piecesToCheck.end();
			if ((xSpecified && p.getPosX() == oldX) || (!xSpecified && p.getPosY() == oldY))
				piecesToCheck.insert(it, (p));
		}
		if (piecesToCheck.empty())
		{
			int file = 8 - oldX;
			char rank = oldY + 65;
			if (xSpecified)
				cout << "Invalid move : None of your pieces of rank " << r << " are on file " << file << endl;
			else
				cout << "Invalid move : None of your pieces of rank " << r << " are on row " << rank << endl;
			return piecesToCheck;
		}
	}
	else
	{
		piecesToCheck = p.allOfRank(r);
		if (piecesToCheck.size() == 0)
		{
			cout << "Invalid move : You don't have any pieces of rank " << r << " left" << endl;
			return piecesToCheck;
		}
	}
	return piecesToCheck;
}

bool Chess::checkEat(bool eat, Player &p, int x, int y)
{
	Piece toEat = board.getPiece(x, y);
	if (eat == false && toEat.getRank() != Rank::EMPTY)
	{
		char file = x + 65;
		cout << "Invalid move : There is a piece on " << file << y + 1 << ". If you want to eat it, please add x after the rank" << endl;
		;
		return false;
	}
	else if (eat == true && toEat.getRank() == Rank::EMPTY)
	{
		char file = x + 65;
		cout << "Invalid move : No piece on " << file << y + 1 << ". If you want to move there anyway please remove the x" << endl;
		return false;
	}
	else if (eat == true && toEat.getColor() == p.getColor())
	{
		cout << "Invalid move : you can not eat your own piece ! " << endl;
		return false;
	}
	return true;
}

vector<tuple<int, int>> Chess::getPossibleKingMoves()
{
	vector<tuple<int, int>> moves;
	auto it = moves.end();
	moves.insert(it, (make_tuple(-1, -1)));
	it = moves.end();
	moves.insert(it, (make_tuple(-1, 0)));
	it = moves.end();
	moves.insert(it, (make_tuple(-1, 1)));
	it = moves.end();
	moves.insert(it, (make_tuple(0, -1)));
	it = moves.end();
	moves.insert(it, (make_tuple(0, 1)));
	it = moves.end();
	moves.insert(it, (make_tuple(1, -1)));
	it = moves.end();
	moves.insert(it, (make_tuple(1, 0)));
	it = moves.end();
	moves.insert(it, (make_tuple(1, 1)));
	return moves;
}

vector<tuple<int, int>> Chess::getPossibleKnightMoves()
{
	vector<tuple<int, int>> moves;
	auto it = moves.end();
	moves.insert(it, (make_tuple(-2, -1)));
	it = moves.end();
	moves.insert(it, (make_tuple(-2, 1)));
	it = moves.end();
	moves.insert(it, (make_tuple(-1, -2)));
	it = moves.end();
	moves.insert(it, (make_tuple(-1, 2)));
	it = moves.end();
	moves.insert(it, (make_tuple(1, -2)));
	it = moves.end();
	moves.insert(it, (make_tuple(1, 2)));
	it = moves.end();
	moves.insert(it, (make_tuple(2, -1)));
	it = moves.end();
	moves.insert(it, (make_tuple(2, 1)));
	return moves;
}

vector<tuple<int, int>> Chess::getKingMoves(Piece &p, bool eat)
{
	vector<tuple<int, int>> possibleMoves;
	int x = p.getPosX();
	int y = p.getPosY();
	vector<tuple<int, int>> kingMoves = getPossibleKingMoves();
	for (int i = 0; i < int(kingMoves.size()); i++)
	{
		auto it = possibleMoves.end();
		int destX = x + get<0>(kingMoves[i]);
		int destY = y + get<1>(kingMoves[i]);
		if (coordOk(7, 7, destX, destY) && ((eat && board.getPiece(destX, destY).getRank() != Rank::EMPTY && board.getPiece(destX, destY).getColor() != p.getColor()) ||
											(!eat && board.getPiece(destX, destY).getRank() == Rank::EMPTY)))
			possibleMoves.insert(it, (make_tuple(destX, destY)));
	}
	return possibleMoves;
}
vector<tuple<int, int>> Chess::getQueenMoves(Piece &p, bool eat)
{
	vector<tuple<int, int>> rookMoves = getRookMoves(p, eat);
	vector<tuple<int, int>> bishopMoves = getBishopMoves(p, eat);
	move(bishopMoves.begin(), bishopMoves.end(), back_inserter(rookMoves));
	return rookMoves;
}
vector<tuple<int, int>> Chess::getKnightMoves(Piece &p, bool eat)
{
	vector<tuple<int, int>> possibleMoves;
	int x = p.getPosX();
	int y = p.getPosY();
	vector<tuple<int, int>> knightmoves = getPossibleKnightMoves();
	for (int i = 0; i < int(knightmoves.size()); i++)
	{
		auto it = possibleMoves.end();
		int destX = x + get<0>(knightmoves[i]);
		int destY = y + get<1>(knightmoves[i]);

		if (coordOk(7, 7, destX, destY) && ((eat && board.getPiece(destX, destY).getRank() != Rank::EMPTY && board.getPiece(destX, destY).getColor() != p.getColor()) ||
											(!eat && board.getPiece(destX, destY).getRank() == Rank::EMPTY)))
			possibleMoves.insert(it, (make_tuple(destX, destY)));
	}
	return possibleMoves;
}

bool Chess::addRookOrBishopMove(int x, int y, bool eat, vector<tuple<int, int>> &moves, Piece &p)
{
	bool squareHasPiece = board.getPiece(x, y).getRank() != Rank::EMPTY;
	bool isOtherColor = board.getPiece(x, y).getColor() != p.getColor();
	auto it = moves.end();
	if (!squareHasPiece) moves.insert(it, (make_tuple(x, y)));
	else if ((!eat && squareHasPiece) || (eat && squareHasPiece && !isOtherColor)) return false;
	else if (eat && squareHasPiece && isOtherColor)
	{
		it = moves.end();
		moves.insert(it, (make_tuple(x, y)));
		return false;
	}
	return true;
}

vector<tuple<int, int>> Chess::getRookMoves(Piece &p, bool eat)
{
	vector<tuple<int, int>> possibleMoves;
	int x = p.getPosX();
	int y = p.getPosY();
	/* LEFT */ for (int i = y - 1; i >= 0; i--) if (!addRookOrBishopMove(x, i, eat, possibleMoves, p)) break;
	/* RIGHT */ for (int i = y + 1; i <= 7; i++) if (!addRookOrBishopMove(x, i, eat, possibleMoves, p)) break;
	/* UP */ for (int i = x - 1; i >= 0; i--) if (!addRookOrBishopMove(x, i, eat, possibleMoves, p)) break;
	/* DOWN */ for (int i = x + 1; i <= 7; i++) if (!addRookOrBishopMove(x, i, eat, possibleMoves, p))break;
	return possibleMoves;
}

vector<tuple<int, int>> Chess::getBishopMoves(Piece &p, bool eat)
{
	vector<tuple<int, int>> possibleMoves;
	int x = p.getPosX();
	int y = p.getPosY();
	/* UP LEFT */ for (int i = 1; i <= 7; i++)
		if (!coordOk(7, 7, x - i, y - i) || !addRookOrBishopMove(x - i, y - i, eat, possibleMoves, p))
			break;
	/* UP RIGHT */ for (int i = 1; i <= 7; i++)
		if (!coordOk(7, 7, x - i, y + i) || !addRookOrBishopMove(x - i, y + i, eat, possibleMoves, p))
			break;
	/* DOWN LEFT */ for (int i = 1; i <= 7; i++)
		if (!coordOk(7, 7, x + i, y - i) || !addRookOrBishopMove(x + i, y - i, eat, possibleMoves, p))
			break;
	/* DOWN RIGHT */ for (int i = 1; i <= 7; i++)
		if (!coordOk(7, 7, x + i, y + i) || !addRookOrBishopMove(x + i, y + i, eat, possibleMoves, p))
			break;
	return possibleMoves;
}

vector<tuple<int, int>> Chess::getPawnMoves(Piece &p, bool eat)
{
	vector<tuple<int, int>> possibleMoves;
	auto it = possibleMoves.end();
	int x = p.getPosX();
	int y = p.getPosY();
	int side = -1;
	if (p.getColor())
		side = 1;
	if (!eat) {
		if (coordOk(7, 7, x + side, y) && board.getPiece(x + side, y).getRank() == Rank::EMPTY) {
			possibleMoves.insert(it, make_tuple(x + side, y));
			it = possibleMoves.end();
			if (coordOk(7, 7, x + 2 * side, y) && p.getInitialPos() &&
				board.getPiece(x + 2 * side, y).getRank() == Rank::EMPTY)
				possibleMoves.insert(it, (make_tuple(x + 2 * side, y)));
		}
	} else {
		it = possibleMoves.end();
		if (coordOk(7, 7, x + side, y - 1) && board.getPiece(x + side, y - 1).getRank() != Rank::EMPTY &&
			board.getPiece(x + side, y - 1).getColor() != p.getColor())
			possibleMoves.insert(it, (make_tuple(x + side, y - 1)));
		it = possibleMoves.end();
		if (coordOk(7, 7, x + side, y + 1) && y != 7 &&
			board.getPiece(x + side, y + 1).getRank() != Rank::EMPTY &&
			board.getPiece(x + side, y + 1).getColor() != p.getColor())
			possibleMoves.insert(it, (make_tuple(x + side, y + 1)));
	}
	return possibleMoves;
}

vector<tuple<int, int>> Chess::getMoves(Piece &p, bool eat)
{
	switch (p.getRank())
	{
	case Rank::BISHOP:
		return getBishopMoves(p, eat);
	case Rank::KING:
		return getKingMoves(p, eat);
	case Rank::KNIGHT:
		return getKnightMoves(p, eat);
	case Rank::PAWN:
		return getPawnMoves(p, eat);
	case Rank::QUEEN:
		return getQueenMoves(p, eat);
	case Rank::ROOK:
		return getRookMoves(p, eat);
	default:
		return getPawnMoves(p, eat);
	}
}

bool Chess::coordInVector(vector<tuple<int, int>> vec, int x, int y)
{
	for (int i = 0; int(vec.size()) > i; i++)
	{
		if (get<0>(vec[i]) == x && get<1>(vec[i]) == y)
			return true;
	}
	return false;
}

Piece Chess::getRightPiece(vector<Piece> &piecesToCheck, Player &p, int x, int y, bool eat)
{
	Piece errorPiece = Piece(true, Rank::EMPTY, 0, 0);
	vector<Piece> okPieces;
	for (Piece &piece : piecesToCheck)
	{
		vector<tuple<int, int>> possibleMoves = getMoves(piece, eat);
		auto it = okPieces.end();
		if (coordInVector(possibleMoves, x, y))
			okPieces.insert(it, piece);
	}
	if (okPieces.size() > 1)
	{
		cout << "Invalid move : multiples pieces can do this move. Please provide disambiguiation" << endl;
		return errorPiece;
	}
	else if (okPieces.size() == 0)
	{
		cout << "Invalid move : No pieces can do this move" << endl;
		return errorPiece;
	}
	else {
		Piece rightPiece = okPieces.at(0);
		return rightPiece;
	}
}

bool Chess::simulateMove(Piece &toMove, int x, int y, bool eat, Player &p)
{
	Player otherPlayer = player_black;
	if (!p.getColor())
		otherPlayer = player_white;
	int oldX = toMove.getPosX();
	int oldY = toMove.getPosY();
	Piece &oldPiece = board.getPiece(x, y);
	p.eatPiece(toMove);
	board.movePiece(toMove, x, y);
	p.addPiece(toMove);
	Piece king = p.allOfRank(Rank::KING).at(0);
	bool toReturn = true;
	if (isChecked(p, king.getPosX(), king.getPosY())) toReturn = false;
	p.eatPiece(toMove);
	board.movePiece(toMove, oldX, oldY);
	p.addPiece(toMove);
	if (eat) {
		otherPlayer.eatPiece(oldPiece);
		board.movePiece(oldPiece, x, y);
		otherPlayer.addPiece(oldPiece);
	}
	return toReturn;
}

Piece Chess::isMoveOk(Rank r, Player &p, int x, int y, bool eat, int oldX, int oldY)
{
	Piece errorPiece = Piece(true, Rank::EMPTY, 0, 0);
	if (!coordOk(7, 7, x, y))
	{ //Si les coordonnées x et/ou y sont en dehors du tableau
		char file = x + 65;
		cout << "Invalid move : " << file << y + 1 << " is out of bounds" << endl;
		return errorPiece;
	}
	vector<Piece> piecesToCheck = getPiecesToCheck(r, p, x, y, oldX, oldY);
	if (piecesToCheck.size() == 0)
		return errorPiece;
	if (!checkEat(eat, p, x, y))
		return errorPiece;
	Piece toMove = getRightPiece(piecesToCheck, p, x, y, eat);
	if (toMove.getRank() == Rank::EMPTY)
		return toMove;
	else if (!simulateMove(toMove, x, y, eat, p))
	{ //Verifie que le mouvement ne mets pas le roi en echec
		cout << "Invalid move : this move would put your king in check" << endl;
		return errorPiece;
	}
	return toMove;
}

bool Chess::queenOrRookCheck(bool color, int x, int y)
{
	Piece pieceOnXY = board.getPiece(x, y);
	if (pieceOnXY.getColor() != color && (pieceOnXY.getRank() == Rank::QUEEN || pieceOnXY.getRank() == Rank::ROOK))
		return true;
	return false;
}

bool Chess::queenOrBishopCheck(bool color, int x, int y)
{
	Piece pieceOnXY = board.getPiece(x, y);
	if (pieceOnXY.getColor() != color && (pieceOnXY.getRank() == Rank::QUEEN || pieceOnXY.getRank() == Rank::BISHOP))
		return true;
	return false;
}

bool Chess::knightCheck(bool color, int x, int y)
{
	vector<tuple<int, int>> possibleKnightSpots = getPossibleKnightMoves();
	for (int i = 0; i < int(possibleKnightSpots.size()); i++)
	{
		int newX = x + get<0>(possibleKnightSpots[i]);
		int newY = y + get<1>(possibleKnightSpots[i]);
		if (coordOk(7, 7, newX, newY))
		{
			Piece pieceOnNewXNewY = board.getPiece(newX, newY);
			if (pieceOnNewXNewY.getColor() != color && pieceOnNewXNewY.getRank() == Rank::KNIGHT)
				return true;
		}
	}
	return false;
}

bool Chess::pawnCheck(bool color, int x, int y)
{
	if (color)
	{
		if (coordOk(7, 7, x + 1, y - 1))
		{
			Piece pieceDownLeft = board.getPiece(x + 1, y - 1);
			if (pieceDownLeft.getColor() == false && pieceDownLeft.getRank() == Rank::PAWN)
				return true;
		}
		if (coordOk(7, 7, x + 1, y + 1))
		{
			Piece pieceDownRight = board.getPiece(x + 1, y + 1);
			if (pieceDownRight.getColor() == false && pieceDownRight.getRank() == Rank::PAWN)
				return true;
		}
	}
	else
	{
		if (coordOk(7, 7, x - 1, y - 1))
		{
			Piece pieceUpLeft = board.getPiece(x - 1, y - 1);
			if (pieceUpLeft.getColor() && pieceUpLeft.getRank() == Rank::PAWN)
				return true;
		}
		if (coordOk(7, 7, x - 1, y + 1))
		{
			Piece pieceUpRight = board.getPiece(x - 1, y + 1);
			if (pieceUpRight.getColor() && pieceUpRight.getRank() == Rank::PAWN)
				return true;
		}
	}
	return false;
}

bool Chess::isChecked(Player &p, int x, int y) //-> retourne vrai si mettre le roi du joueur p en [x][y] le mettrai en echec
//Regarde si une piece du joueur adverse est sur l'une des colonnes ou diagonales du roi
{
	bool color = p.getColor();
	Piece king = p.allOfRank(Rank::KING).at(0);
	int oldX = king.getPosX();
	int oldY = king.getPosY();

	//QUEEN AND ROOKS
	/* UP */ for (int i = x - 1; i >= 0; i--)
		if (queenOrRookCheck(color, i, y))
			goto check;
	/* DOWN */ for (int i = x + 1; i <= 7; i++)
		if (queenOrRookCheck(color, i, y))
			goto check;
	/* LEFT */ for (int i = y - 1; i >= 0; i--)
		if (queenOrRookCheck(color, x, i))
			goto check;
	/* RIGHT */ for (int i = y + 1; i <= 7; i++)
		if (queenOrRookCheck(color, x, i))
			goto check;

	//QUEEN AND BISHOPS
	/* UP LEFT */
	for (int i = 1; i <= 7; i++)
	{
		if (!coordOk(7, 7, x - i, y - i))
			break;
		if (queenOrBishopCheck(color, x - i, y - i))
			goto check;
	}
	/* UP RIGHT */
	for (int i = 1; i <= 7; i++)
	{
		if (!coordOk(7, 7, x - i, y + i))
			break;
		if (queenOrBishopCheck(color, x - i, y + i))
			goto check;
	}
	/* DOWN LEFT */
	for (int i = 1; i <= 7; i++)
	{
		if (!coordOk(7, 7, x + i, y - i))
			break;
		if (queenOrBishopCheck(color, x + i, y - i))
			goto check;
	}
	/* DOWN RIGHT */
	for (int i = 1; i <= 7; i++)
	{
		if (!coordOk(7, 7, x + i, y + i))
			break;
		if (queenOrBishopCheck(color, x + i, y + i))
			goto check;
	}

	//KNIGHT
	if (knightCheck(color, x, y))
		goto check;

	//PAWN
	if (pawnCheck(color, x, y))
		goto check;

	board.movePiece(king, oldX, oldY);
	return false;

check:
	board.movePiece(king, oldX, oldY);
	return true;
}

bool Chess::checkMate(Player &p)
{ //returns true if the king of player p is checkmated
	Piece king = p.allOfRank(Rank::KING).at(0);
	if (!isChecked(p, king.getPosX(), king.getPosY()))
		return false;
	for (Piece piece : p.getPieces())
	{
		if (piece.getRank() != Rank::KING)
		{
			vector<tuple<int, int>> movesWithEat = getMoves(piece, true);
			vector<tuple<int, int>> movesWithoutEat = getMoves(piece, false);
			for (int i = 0; i < int(movesWithEat.size()); i++)
			{
				if (simulateMove(piece, get<0>(movesWithEat[i]), get<1>(movesWithEat[i]), true, p))
					return false;
			}
			for (int i = 0; i < int(movesWithoutEat.size()); i++)
				if (simulateMove(piece, get<0>(movesWithoutEat[i]), get<1>(movesWithoutEat[i]), false, p))
					return false;
		}
	}
	return true;
}

string Chess::getMoveNotation(Piece &piece, int x, int y, bool eat)
{
	string move = "";
	char rank = rankToChar(piece.getRank());
	move += rank;
	if (eat) move += 'x';
	char rankDest = y + 65;
	move += rankDest;
	int fileDest = 8 - x;
	move += to_string(fileDest);
	return move;
}

void Chess::help(Player &p)
{
	for (Piece piece : p.getPieces())
	{
		vector<tuple<int, int>> movesWithEat = getMoves(piece, true);
		vector<tuple<int, int>> movesWithoutEat = getMoves(piece, false);
		for (int i = 0; i < movesWithEat.size(); i++)
			cout << getMoveNotation(piece, get<0>(movesWithEat[i]), get<1>(movesWithEat[i]), true) << endl;
		for (int i = 0; i < movesWithoutEat.size(); i++)
			cout << getMoveNotation(piece, get<0>(movesWithoutEat[i]), get<1>(movesWithoutEat[i]), false) << endl;
	}
}

void Chess::robotMove(Player &robot, string path)
{
	ofstream history;
	history.open(path);
	string moveDone("");
	srand(time(NULL));
	cout << "Robot's move :" << endl;
Loop:
	cout << "loop started" << endl;
	int nbOfPieces = robot.nbOfPieces() - 1;
	int randPiece = std::rand() % nbOfPieces;
	Piece toMove = robot.getPieces().at(randPiece);
	vector<tuple<int, int>> movesWithEat = getMoves(toMove, true);
	vector<tuple<int, int>> movesWithoutEat = getMoves(toMove, false);
	if (movesWithoutEat.empty() && movesWithEat.empty()) goto Loop;
	int randMove = std::rand() % (movesWithEat.size() + movesWithoutEat.size() - 1);

	//TEST
	int file = 8 - toMove.getPosX();
	char rank = toMove.getPosY() + 65;
	cout << "Chosen piece : " << toMove.getRank() << file << rank << endl;
	cout << "With eat size : " << movesWithEat.size() << endl;
	cout << "Without eat size : " << movesWithoutEat.size() << endl;
	cout << "rand move : " << randMove << endl;
	//TEST

	if (randMove < movesWithEat.size() && !movesWithEat.empty())
	{
		Player other = getPlayerWhite();
		if (robot.getColor())
			other = getPlayerBlack();
		int x = get<0>(movesWithEat[randMove]);
		int y = get<1>(movesWithEat[randMove]);
		if (!simulateMove(toMove, x, y, true, robot)) goto Loop;
		Piece toEat = board.getPiece(x, y);
		other.eatPiece(toEat);
		robot.eatPiece(toMove);
		board.movePiece(toMove, x, y);
		robot.addPiece(toMove);
		moveDone = getMoveNotation(toMove, x, y, true);
	} else {
		int x =  get<0>(movesWithoutEat[randMove]);
		int y =  get<1>(movesWithoutEat[randMove]);
		if (!simulateMove(toMove, x, y, false, robot)) goto Loop;
		robot.eatPiece(toMove);
		board.movePiece(toMove,x,y);
		robot.addPiece(toMove);
		moveDone = getMoveNotation(toMove, x, y, false);
	}
	history << moveDone << endl;
	history.close();
}

// TESTS ............................................

void Chess::showPieces (Player &p){
	cout << p.getName() << " pieces : " << endl;
	for (Piece piece : p.getPieces()){
		int file = 8 - piece.getPosX();
		char rank = piece.getPosY() + 65;
		cout << piece.getRank() << " " << file << rank  << endl;
	}
}

void Chess::test(){
	// x = [1..8] y = [a..h}
	/*Piece piece = player_white.allOfRank(Rank::PAWN).at(0);
	board.movePiece(piece, 2, 0);
	vector<tuple<int, int>> movesWithEat = getMoves(piece, true);
	vector<tuple<int, int>> movesWithoutEat = getMoves(piece, false);
	for (int i = 0; i < movesWithEat.size(); i++)
		cout << getMoveNotation(piece, get<0>(movesWithEat[i]), get<1>(movesWithEat[i]), true) << endl;
	for (int i = 0; i < movesWithoutEat.size(); i++)
		cout << getMoveNotation(piece, get<0>(movesWithoutEat[i]), get<1>(movesWithoutEat[i]), false) << endl;

	/*vector<Piece> pieces = player_black.allOfRank(Rank::KING);
	for (Piece piece : pieces) {
		int file = 8 - piece.getPosX();
		char rank = piece.getPosY() + 65;
		cout << "The piece on " << file << rank << " can do these moves : " << endl;
	}*/
}

// END TESTS ........................................
