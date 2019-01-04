#include "../include/Chess.hpp"
#include "../include/Util.hpp"
#include <fstream>
#include <ctype.h>
#include <tuple>
#include <vector>
#include <algorithm>

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
	int length = move.length();
	switch (length)
	{
	case 2: //pawn move
	{
		int y = letterToNumber(move[0]);
		int x = '8' - move[1];
		cout << "x : " << x << endl;
		Piece pawn = isMoveOk(Rank::PAWN, p, x, y, false, -1, -1);
		if (pawn.getRank() != Rank::EMPTY)
		{
			board.movePiece(pawn, x, y);
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
					board.movePiece(piece, x, y);
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
				int oldX = letterToNumber(move[0]);
				Piece piece = isMoveOk(Rank::PAWN, p, x, y, true, oldX, -1);
				if (piece.getRank() == Rank::EMPTY)
					goto startLoop;
				else
				{
					otherPlayer.eatPiece(board.getPiece(x, y));
					board.movePiece(piece, x, y);
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
						board.movePiece(piece, x, y);
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
					board.movePiece(piece, x, y);
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
					board.movePiece(piece, x, y);
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
					board.movePiece(piece, x, y);
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
					board.movePiece(piece, x, y);
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
			char file = x + 65;
			if (xSpecified)
				cout << "Invalid move : None of your pieces of rank "
					 << " are on file " << file << endl;
			else
				cout << "Invalid move : None of your pieces of rank "
					 << " are on row " << oldY + 1 << endl;
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
		cout << "in checkeat \n piece on " << file << y << "is of color : " << toEat.getColor() << "and rank : " <<toEat.getRank() << endl;
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

vector<tuple<int, int>> Chess::getPossibleKnightgMoves()
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
	for (int i = 0; i < kingMoves.size(); i++)
	{
		auto it = possibleMoves.end();
		int destX = x + get<0>(kingMoves[i]);
		int destY = y + get<1>(kingMoves[i]);
		if (coordOk(7, 7, destX, destY) && ((eat && board.getPiece(destX, destY).getRank() != Rank::EMPTY) ||
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
	vector<tuple<int, int>> knightmoves = getPossibleKnightgMoves();
	for (int i = 0; i < knightmoves.size(); i++)
	{
		auto it = possibleMoves.end();
		int destX = x + get<0>(knightmoves[i]);
		int destY = y + get<1>(knightmoves[i]);
		if (coordOk(7, 7, destX, destY) && ((eat && board.getPiece(destX, destY).getRank() != Rank::EMPTY) ||
											(!eat && board.getPiece(destX, destY).getRank() == Rank::EMPTY)))
			possibleMoves.insert(it, (make_tuple(destX, destY)));
	}
	return possibleMoves;
}

bool Chess::addRookOrBishopMove(int x, int y, bool eat, vector<tuple<int, int>> &moves)
{
	bool squareHasPiece = board.getPiece(x, y).getRank() != Rank::EMPTY;
	auto it = moves.end();
	if ((!eat && !squareHasPiece) || (eat && !squareHasPiece))
		moves.insert(it, (make_tuple(x, y)));
	else if (!eat && squareHasPiece)
		return false;
	else if (eat && squareHasPiece)
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
	/* LEFT */ for (int i = y - 1; i >= 0; i--)
		if (!addRookOrBishopMove(x, i, eat, possibleMoves))
			break;
	/* RIGHT */ for (int i = y + 1; i <= 7; i++)
		if (!addRookOrBishopMove(x, i, eat, possibleMoves))
			break;
	/* UP */ for (int i = x - 1; x >= 0; x--)
		if (!addRookOrBishopMove(x, i, eat, possibleMoves))
			break;
	/* DOWN */ for (int i = x + 1; x <= 7; x++)
		if (!addRookOrBishopMove(x, i, eat, possibleMoves))
			break;
	return possibleMoves;
}

vector<tuple<int, int>> Chess::getBishopMoves(Piece &p, bool eat)
{
	vector<tuple<int, int>> possibleMoves;
	int x = p.getPosX();
	int y = p.getPosY();
	/* UP LEFT */ for (int i = 1; i <= 7; i++)
		if (!coordOk(7, 7, x - i, y - i) || !addRookOrBishopMove(x - i, y - i, eat, possibleMoves))
			break;
	/* UP RIGHT */ for (int i = 1; i <= 7; i++)
		if (!coordOk(7, 7, x - i, y + i) || !addRookOrBishopMove(x - i, y + i, eat, possibleMoves))
			break;
	/* DOWN LEFT */ for (int i = 1; i <= 7; i++)
		if (!coordOk(7, 7, x + i, y - i) || !addRookOrBishopMove(x + i, y - i, eat, possibleMoves))
			break;
	/* DOWN RIGHT */ for (int i = 1; i <= 7; i++)
		if (!coordOk(7, 7, x + i, y + i) || !addRookOrBishopMove(x + i, y + i, eat, possibleMoves))
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
	if (p.getColor()) side = 1;
	if (coordOk(7, 7, x + side, y) && board.getPiece(x + side, y).getRank() == Rank::EMPTY)
	{
		possibleMoves.insert(it, make_tuple(x + side, y));
		it = possibleMoves.end();
		if (coordOk(7, 7, x + 2 * side, y) && p.getInitialPos() && board.getPiece(x + 2 * side, y).getRank() == Rank::EMPTY)
			possibleMoves.insert(it, (make_tuple(x + 2 * side, y)));
	}
	it = possibleMoves.end();
	if (eat && coordOk(7, 7, x + side, y - 1) && board.getPiece(x + side, y - 1).getRank() != Rank::EMPTY)
		possibleMoves.insert(it, (make_tuple(x + side, y - 1)));
	it = possibleMoves.end();
	if (eat && coordOk(7, 7, x + side, y + 1) && y != 7 && board.getPiece(x + side, y + 1).getRank() != Rank::EMPTY)
		possibleMoves.insert(it, (make_tuple(x + side, y + 1)));
	return possibleMoves;
}

bool Chess::coordInVector(vector<tuple<int, int>> vec, int x, int y)
{
	for (int i = 0; i < vec.size(); i++)
	{
		if (get<0>(vec[i]) == x && get<1>(vec[i]) == y)
			return true;
	}
	return false;
}

Piece Chess::getRightPiece(Rank r, vector<Piece> &piecesToCheck, Player &p, int x, int y, bool eat)
{
	vector<Piece> okPieces;
	for (Piece piece : piecesToCheck)
	{
		vector<tuple<int, int>> possibleMoves;
		auto it = okPieces.end();
		switch (r)
		{
		case Rank::BISHOP:
			possibleMoves = getBishopMoves(piece, eat); break;
		case Rank::KING:
			possibleMoves = getKingMoves(piece, eat); break;
		case Rank::KNIGHT:
			possibleMoves = getKnightMoves(piece, eat); break;
		case Rank::PAWN:
			possibleMoves = getPawnMoves(piece, eat); break;
		case Rank::QUEEN:
			possibleMoves = getQueenMoves(piece, eat); break;
		case Rank::ROOK:
			possibleMoves = getRookMoves(piece, eat); break;
		default:
			break;
		}
		if (coordInVector(possibleMoves, x, y)) okPieces.insert(it, piece);
	}
	if (okPieces.size() > 1)
	{
		cout << "Invalid move : multiples pieces can do this move. Please provide disambiguiation" << endl;
		return Piece(true, Rank::EMPTY, 0, 0);
	}
	else if (okPieces.size() == 0)
	{
		cout << "Invalid move : No pieces can do this move" << endl;
		return Piece(true, Rank::EMPTY, 0, 0);
	}
	else
		return okPieces.at(0);
}

bool Chess::simulateMove(Piece &toMove, int x, int y, bool eat, Player &p)
{
	int oldX = toMove.getPosX();
	int oldY = toMove.getPosY();
	Piece oldPiece = board.getPiece(x, y);
	board.movePiece(toMove, x, y);
	Piece king = p.allOfRank(Rank::KING).at(0);
	if (isChecked(p, king.getPosX(), king.getPosY()))
	{
		board.movePiece(toMove, oldX, oldY);
		board.movePiece(oldPiece, x, y);
	}
	return true;
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
	Piece toMove = getRightPiece(r, piecesToCheck, p, x, y, eat);
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
	vector<tuple<int, int>> possibleKnightSpots = getPossibleKnightgMoves();
	for (int i = 0; i < possibleKnightSpots.size(); i++)
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
			if (!pieceDownLeft.getColor() && pieceDownLeft.getRank() == Rank::PAWN)
				return true;
		}
		if (coordOk(7, 7, x + 1, y + 1))
		{
			Piece pieceDownRight = board.getPiece(x + 1, y + 1);
			if (!pieceDownRight.getColor() && pieceDownRight.getRank() == Rank::PAWN)
				return true;
		}
	}
	else
	{
		if (coordOk(7, 7, x - 1, y - 1))
		{
			Piece pieceUpLeft = board.getPiece(x - 1, y - 1);
			if (!pieceUpLeft.getColor() && pieceUpLeft.getRank() == Rank::PAWN)
				return true;
		}
		if (coordOk(7, 7, x - 1, y + 1))
		{
			Piece pieceUpRight = board.getPiece(x - 1, y + 1);
			if (!pieceUpRight.getColor() && pieceUpRight.getRank() == Rank::PAWN)
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
			return true;
	/* DOWN */ for (int i = x + 1; i <= 7; i++)
		if (queenOrRookCheck(color, i, y))
			return true;
	/* LEFT */ for (int i = y - 1; i >= 0; i--)
		if (queenOrRookCheck(color, x, i))
			return true;
	/* RIGHT */ for (int i = y + 1; i <= 7; i++)
		if (queenOrRookCheck(color, x, i))
			return true;

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

check:
	board.movePiece(king, oldX, oldY);
	return true;

not_check:
	board.movePiece(king, oldX, oldY);
	return false;
}

string Chess::getMoveNotation(Piece piece, int x, int y, bool eat, char rank)
{
	char fileFrom = piece.getPosX() + 65;
	char rankFrom = piece.getPosY() + 1;
	char fileDest = x + 65;
	string move = "";
	move += rank;
	if (eat)
	{
		move += 'x';
	}
	move += fileDest;
	move += y + 1;
	return move;
}

void Chess::help(Player p)
{
	vector<tuple<int, int>> movesWithEat;
	vector<tuple<int, int>> movesWithoutEat;
	for (Piece piece : p.getPieces())
	{
		char rank = '\0';
		switch (piece.getRank())
		{
		case Rank::PAWN:
			movesWithEat = getPawnMoves(piece, true);
			movesWithoutEat = getPawnMoves(piece, false);
			break;
		case Rank::BISHOP:
			rank = 'B';
			movesWithEat = getBishopMoves(piece, true);
			movesWithoutEat = getBishopMoves(piece, false);
			break;
		case Rank::KING:
			rank = 'K';
			movesWithEat = getKingMoves(piece, true);
			movesWithoutEat = getKingMoves(piece, false);
			break;
		case Rank::KNIGHT:
			rank = 'N';
			movesWithEat = getKnightMoves(piece, true);
			movesWithoutEat = getKnightMoves(piece, true);
			break;
		case Rank::QUEEN:
			rank = 'Q';
			movesWithEat = getQueenMoves(piece, true);
			movesWithoutEat = getQueenMoves(piece, false);
			break;
		case Rank::ROOK:
			rank = 'R';
			movesWithEat = getRookMoves(piece, true);
			movesWithoutEat = getRookMoves(piece, false);
			break;
		default:
			break;
		}
		for (int i = 0; i < movesWithEat.size(); i++)
			cout << getMoveNotation(piece, get<0>(movesWithEat[i]), get<1>(movesWithEat[i]), true, rank) << endl;
		for (int i = 0; i < movesWithoutEat.size(); i++)
			cout << getMoveNotation(piece, get<0>(movesWithEat[i]), get<1>(movesWithEat[i]), false, rank) << endl;
	}
}

Piece isMoveOkOld(Rank r, Board b, Player p, int x, int y, bool eat, int oldX, int oldY)
{
	// Piece isMoveOk(Rank r, Board b, Player p, int x, int y, bool eat, int oldX, int oldY) -> cherche une piece du joueur p de rang r qui peut se déplacer en [x][y]. Retourne piece de rang EMPTY si 0 ou 2 pieces trouvées.
	// 	  Si oldX (/oldY) =! -1, posX (/posY) de la piece trouvée doit etre égale à oldX (/oldY).   ok
	// 	! A verifier dans la fonction : le deplacement a la bonne forme,  ok
	// 									si eat = false la case d'arrivée est vide,    ok
	// 									si eat = true la case d'arrivée contient une piece de l'autre joueur,    ok
	// 									si r != Knight toutes les cases entre depart et arrivée sont vides,   ok
	// 									si r = King ne le met pas en echec   ok

	vector<Piece> piecesofRank = p.allOfRank(r);
	vector<Piece> piecesofRankNicePlaced;
	Piece piece;
	if (piecesofRank.size() < 1)
	{
		return Piece(true, Rank::EMPTY, 0, 0);
	}
	else if (piecesofRank.size() == 1)
	{
		piece = piecesofRank.at(0);
		oldX = piecesofRank.at(0).getPosX();
		oldY = piecesofRank.at(0).getPosY();
	}
	else
	{
		if (oldX == -1 && oldY == -1) //NO !!!!!!!!!!!!!!!!!!!!!
		{
			return Piece(true, Rank::EMPTY, 0, 0);
		}
		if (oldX != -1 && oldY == -1)
		{
			for (int i = 0; i < piecesofRank.size(); i++)
			{
				if (piecesofRank.at(i).getPosX() == oldX)
					piecesofRankNicePlaced.push_back(piecesofRank.at(i));
			}
		}
		if (oldY != -1 && oldX == -1)
		{
			for (int i = 0; i < piecesofRank.size(); i++)
			{
				if (piecesofRank.at(i).getPosY() == oldY)
					piecesofRankNicePlaced.push_back(piecesofRank.at(i));
			}
		}
		if (oldY != -1 && oldX != -1)
		{
			for (int i = 0; i < piecesofRank.size(); i++)
			{
				if (piecesofRank.at(i).getPosY() == oldY && piecesofRank.at(i).getPosX() == oldX)
					piecesofRankNicePlaced.push_back(piecesofRank.at(i));
			}
		}
		if (piecesofRankNicePlaced.size() > 1 || piecesofRankNicePlaced.size() < 1)
		{
			return Piece(true, Rank::EMPTY, 0, 0);
		}
		if (piecesofRankNicePlaced.size() == 1)
		{
			piece = piecesofRank.at(0);
			oldX = piecesofRankNicePlaced.at(0).getPosX();
			oldY = piecesofRankNicePlaced.at(0).getPosY();
		}
	}
	/*cas nimp*/
	if (x > b.getWidth() - 1 || y > b.getWidth() - 1 || x < 0 || y < 0)
	{
		return Piece(true, Rank::EMPTY, 0, 0);
	}
	/*pas de mouvement*/
	if (oldX == x && oldY == y)
	{
		return Piece(true, Rank::EMPTY, 0, 0);
	}
	/*Verification eat*/
	if (eat)
	{
		if (b.getPiece(x, y).getColor() == p.getColor())
			return Piece(true, Rank::EMPTY, 0, 0);
	}
	else
	{
		if (b.getPiece(x, y).getRank() != Rank::EMPTY)
			return Piece(true, Rank::EMPTY, 0, 0);
	}

	/*Verification deplacement du roi*/
	if (r == Rank::KING)
	{
		/*if (isChecked(p, b, x, y))
		{
			return Piece(true, Rank::EMPTY, 0, 0);
		}
	}*/

		/*Verification deplacement + cases vides pour le trajet de tout sauf KNIGHT*/
		switch (r)
		{
		case Rank::BISHOP:
			if (oldX > x)
			{
				if (oldY > y)
				{
					/*pour savoir si c'est une diagonale*/
					if ((oldY - y - (oldX - x)) != 0)
					{
						return Piece(true, Rank::EMPTY, 0, 0);
					}
					/*verification case vide*/
					int j = oldY - 1;
					for (int i = oldX - 1; i > x; i--)
					{
						if (b.getPiece(i, j).getRank() != Rank::EMPTY)
						{
							return Piece(true, Rank::EMPTY, 0, 0);
						}
						j--;
					}
				}
				else if (oldY == y)
				{
					return Piece(true, Rank::EMPTY, 0, 0);
				}
				else
				{
					/*pour savoir si c'est une diagonale*/
					if ((oldY - y + (oldX - x)) != 0)
					{
						return Piece(true, Rank::EMPTY, 0, 0);
					}
					/*verification case vide*/
					int j = oldY + 1;
					for (int i = oldX - 1; i > x; i--)
					{
						if (b.getPiece(i, j).getRank() != Rank::EMPTY)
						{
							return Piece(true, Rank::EMPTY, 0, 0);
						}
						j++;
					}
				}
			}
			else if (oldX == x)
			{
				return Piece(true, Rank::EMPTY, 0, 0);
			}
			else
			{
				if (oldY > y)
				{
					/*pour savoir si c'est une diagonale*/
					if ((oldY - y + (oldX - x)) != 0)
					{
						return Piece(true, Rank::EMPTY, 0, 0);
					}
					/*verification case vide*/
					int j = oldY - 1;
					for (int i = oldX + 1; i < x; i++)
					{
						if (b.getPiece(i, j).getRank() != Rank::EMPTY)
						{
							return Piece(true, Rank::EMPTY, 0, 0);
						}
						j--;
					}
				}
				else if (oldY == y)
				{
					return Piece(true, Rank::EMPTY, 0, 0);
				}
				else
				{
					/*pour savoir si c'est une diagonale*/
					if ((oldY - y - (oldX - x)) != 0)
					{
						return Piece(true, Rank::EMPTY, 0, 0);
					}
					/*verification case vide*/
					int j = oldY + 1;
					for (int i = oldX + 1; i < x; i++)
					{
						if (b.getPiece(i, j).getRank() != Rank::EMPTY)
						{
							return Piece(true, Rank::EMPTY, 0, 0);
						}
						j++;
					}
				}
			}
			break;
		case Rank::PAWN:
			if (!eat)
			{
				if (piece.getInitialPos())
				{
					if (oldY - y == 2)
					{
						if (b.getPiece(x, oldY - 1).getRank() != Rank::EMPTY)
						{
							return Piece(true, Rank::EMPTY, 0, 0);
						}
					}
					if (oldY - y == -2)
					{
						if (b.getPiece(x, oldY + 1).getRank() != Rank::EMPTY)
						{
							return Piece(true, Rank::EMPTY, 0, 0);
						}
					}
				}
				else
				{
					if (oldY - y != 1 && oldY - y != -1)
						return Piece(true, Rank::EMPTY, 0, 0);
				}
			}
			else
			{
				/*cas piece blanche ---> descente*/
				if (piece.getColor())
				{
					if (oldY - y == -1)
					{
						if (oldX - x != -1 && oldX - x != 1)
						{
							return Piece(true, Rank::EMPTY, 0, 0);
						}
					}
					else
					{
						return Piece(true, Rank::EMPTY, 0, 0);
					}
				}
				else
				{
					if (oldY - y == 1)
					{
						if (oldX - x != -1 && oldX - x != 1)
						{
							return Piece(true, Rank::EMPTY, 0, 0);
						}
					}
					else
					{
						return Piece(true, Rank::EMPTY, 0, 0);
					}
				}
				break;
			case Rank::ROOK:
				if (oldX > x)
				{
					if (oldY > y)
					{
						return Piece(true, Rank::EMPTY, 0, 0);
					}

					else if (oldY == y)
					{
						/*verification case vide*/
						for (int i = oldX - 1; i > x; i--)
						{
							if (b.getPiece(i, y).getRank() != Rank::EMPTY)
							{
								return Piece(true, Rank::EMPTY, 0, 0);
							}
						}
					}
					else
					{
						return Piece(true, Rank::EMPTY, 0, 0);
					}
				}
				else if (oldX == x)
				{
					if (oldY > y)
					{
						/*verification case vide*/
						for (int i = oldY - 1; i > y; i--)
						{
							if (b.getPiece(x, i).getRank() != Rank::EMPTY)
							{
								return Piece(true, Rank::EMPTY, 0, 0);
							}
						}
					}
					else if (oldY == y)
					{
						return Piece(true, Rank::EMPTY, 0, 0);
					}
					else
					{
						/*verification case vide*/
						for (int i = oldY + 1; i < y; i++)
						{
							if (b.getPiece(x, i).getRank() != Rank::EMPTY)
							{
								return Piece(true, Rank::EMPTY, 0, 0);
							}
						}
					}
				}
				else
				{
					if (oldY > y)
					{
						return Piece(true, Rank::EMPTY, 0, 0);
					}

					else if (oldY == y)
					{
						/*verification case vide*/
						for (int i = oldX + 1; i < x; i++)
						{
							if (b.getPiece(i, y).getRank() != Rank::EMPTY)
							{
								return Piece(true, Rank::EMPTY, 0, 0);
							}
						}
					}
					else
					{
						return Piece(true, Rank::EMPTY, 0, 0);
					}
				}
				break;
			case Rank::QUEEN:
				if (oldX > x)
				{
					if (oldY > y)
					{
						/*pour savoir si c'est une diagonale*/
						if ((oldY - y - (oldX - x)) != 0)
						{
							return Piece(true, Rank::EMPTY, 0, 0);
						}
						/*verification case vide*/
						int j = oldY - 1;
						for (int i = oldX - 1; i > x; i--)
						{
							if (b.getPiece(i, j).getRank() != Rank::EMPTY)
							{
								return Piece(true, Rank::EMPTY, 0, 0);
							}
							j--;
						}
					}

					else if (oldY == y)
					{
						/*verification case vide*/
						for (int i = oldX - 1; i > x; i--)
						{
							if (b.getPiece(i, y).getRank() != Rank::EMPTY)
							{
								return Piece(true, Rank::EMPTY, 0, 0);
							}
						}
					}
					else
					{
						/*pour savoir si c'est une diagonale*/
						if ((oldY - y + (oldX - x)) != 0)
						{
							return Piece(true, Rank::EMPTY, 0, 0);
						}
						/*verification case vide*/
						int j = oldY + 1;
						for (int i = oldX - 1; i > x; i--)
						{
							if (b.getPiece(i, j).getRank() != Rank::EMPTY)
							{
								return Piece(true, Rank::EMPTY, 0, 0);
							}
							j++;
						}
					}
				}
				else if (oldX == x)
				{
					if (oldY > y)
					{
						/*verification case vide*/
						for (int i = oldY - 1; i > y; i--)
						{
							if (b.getPiece(x, i).getRank() != Rank::EMPTY)
							{
								return Piece(true, Rank::EMPTY, 0, 0);
							}
						}
					}
					else if (oldY == y)
					{
						return Piece(true, Rank::EMPTY, 0, 0);
					}
					else
					{
						/*verification case vide*/
						for (int i = oldY + 1; i < y; i++)
						{
							if (b.getPiece(x, i).getRank() != Rank::EMPTY)
							{
								return Piece(true, Rank::EMPTY, 0, 0);
							}
						}
					}
				}
				else
				{
					if (oldY > y)
					{
						/*pour savoir si c'est une diagonale*/
						if ((oldY - y + (oldX - x)) != 0)
						{
							return Piece(true, Rank::EMPTY, 0, 0);
						}
						/*verification case vide*/
						int j = oldY - 1;
						for (int i = oldX + 1; i < x; i++)
						{
							if (b.getPiece(i, j).getRank() != Rank::EMPTY)
							{
								return Piece(true, Rank::EMPTY, 0, 0);
							}
							j--;
						}
					}

					else if (oldY == y)
					{
						/*verification case vide*/
						for (int i = oldX + 1; i < x; i++)
						{
							if (b.getPiece(i, y).getRank() != Rank::EMPTY)
							{
								return Piece(true, Rank::EMPTY, 0, 0);
							}
						}
					}
					else
					{
						/*pour savoir si c'est une diagonale*/
						if ((oldY - y - (oldX - x)) != 0)
						{
							return Piece(true, Rank::EMPTY, 0, 0);
						}
						/*verification case vide*/
						int j = oldY + 1;
						for (int i = oldX + 1; i < x; i++)
						{
							if (b.getPiece(i, j).getRank() != Rank::EMPTY)
							{
								return Piece(true, Rank::EMPTY, 0, 0);
							}
							j++;
						}
					}
				}
				break;
			case Rank::KING:

				if (oldX - x != 1 && oldX - x != -1)
				{
					return Piece(true, Rank::EMPTY, 0, 0);
				}
				if (oldY - y != 1 && oldY - y != -1)
				{
					return Piece(true, Rank::EMPTY, 0, 0);
				}

				break;
			case Rank::KNIGHT:
				/*cas L vertical*/
				if (oldX - x == 1 || oldX - x == -1)
				{
					if (oldY - y != 2 && oldY - y != -2)
					{
						return Piece(true, Rank::EMPTY, 0, 0);
					}
				}
				/*cas L horizontal*/
				else if (oldX - x == 2 || oldX - x == -2)
				{
					if (oldY - y != 1 && oldY - y != -1)
					{
						return Piece(true, Rank::EMPTY, 0, 0);
					}
				}
				/*cas autre*/
				else
				{
					return Piece(true, Rank::EMPTY, 0, 0);
				}
				break;

			default:
				return Piece(true, Rank::EMPTY, 0, 0);
				break;
			}
		}
		/*Et SI ENFIN tous les tests sont passés alors on renvoie la piece*/
		return piece;
	}
}
