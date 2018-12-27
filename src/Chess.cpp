#include "../include/Chess.hpp"
#include "../include/Util.hpp"
#include <fstream>
#include <ctype.h>

using namespace std;

Chess::Chess()
{
	this->game_type = Game_type::ECHEC;
}

void Chess::Move(Player p, string path)
{
	ofstream history;
	history.open(path);
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
		int x = letterToNumber(move[0]);
		Piece pawn = p.getPawnOfFile(x);
		if (pawn.getRank() != Rank::EMPTY)
		{
			int y = move[1] - '1';
			if (moveOnePiece(pawn, x, y))
				break;
		}
		else
		{
			cout << "Invalid move : No pawn found of that file" << endl;
			goto startLoop;
		}
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
			int x = letterToNumber(move[0]);
			Piece pawn = p.getPawnOfFile(x);
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
					int newY = move[1] - '1';
					Piece newPiece = Piece(color, r, oldX, newY);
					board.setPiece(newPiece, oldX, newY);
				}
				else
				{
					cout << "Invalid move : you cannot promote a pawn to rank " << move[2] << "." << endl;
					cout << "Please choose one of the following : Q, N, R, B" << endl;
					goto startLoop;
				}
			}
			else
			{
				cout << "Invalid move : No pawn found of that file" << endl;
				goto startLoop;
			}
		}
		else
		{ //regular move
			Rank r = getRankFromChar(move[0]);
			if (r != Rank::EMPTY)
			{
				int x = letterToNumber(move[1]);
				int y = move[2] - '1';
				Piece piece = isMoveOk(r, board, p, x, y, false);
				if (piece.getRank() == Rank::EMPTY)
				{
					cout << "Invalid move : no piece found of rank " << move[0] << "able to go to " << move[1] << move[2] << endl;
					goto startLoop;
				}
				else
				{
					board.movePiece(piece, x, y);
					break;
				}
			}
			else
			{
				cout << "Invalid move : unknown rank " << move[0];
				goto startLoop;
			}
		}
	}
	case 4: //move with capture or disambiguation
		if (move[1] == 'x')
		{ //Capture
			Rank r = getRankFromChar(move[0]);
			if (r != Rank::EMPTY)
			{
				int x = letterToNumber(move[2]);
				int y = move[3] - '1';
				Piece piece = isMoveOk(r, board, p, x, y, true, -1, -1);
				if (piece.getRank() == Rank::EMPTY)
				{
					cout << "Invalid move : no piece found of rank " << move[0] << "able to go to " << move[1] << move[2] << endl;
					goto startLoop;
				}
				else
				{
					board.removePiece(x, y);
					board.movePiece(piece, x, y);
					break;
				}
			}
			else
			{
				cout << "Invalid move : unknown rank " << move[0];
				goto startLoop;
			}
		}
		else
		{ // disambiguation
			int oldX = -1;
			int oldY = -1;
			if (isdigit(move[1]))
				oldY = move[1] - '1';
			else
				oldX = letterToNumber(move[1]);
			Rank r = getRankFromChar(move[0]);
			if (r != Rank::EMPTY)
			{
				int x = letterToNumber(move[2]);
				int y = move[3] - '1';
				Piece piece = isMoveOk(r, board, p, x, y, true, oldX, oldY);
				if (piece.getRank() == Rank::EMPTY)
				{
					cout << "Invalid move : no piece found of rank " << move[0] << "able to go to " << move[1] << move[2] << endl;
					goto startLoop;
				}
				else
				{
					board.removePiece(x, y);
					board.movePiece(piece, x, y);
					break;
				}
			}
			else
			{
				cout << "Invalid move : unknown rank " << move[0];
				goto startLoop;
			}
		}
	case 5: //castling queenside + capture with disambiguation
		if (move == "0-0-0")
		{ //Castling queenside (left)
			if (castling(false, p))
				break;
		}
		else
		{ //Capture with disambiguation
		}
	default:
		cout << "Invalid move : unkown move " << move << endl;
		goto startLoop;
	}
	history << move << endl;
	history.close();
}

bool Chess::moveOnePiece(Piece &p, int x, int y)
{
	if (coordOk(x, y))
	{
		board.movePiece(p, x, y);
		return true;
	}
	return false;
}

bool Chess::coordOk(int x, int y)
{
	//add return false if the move puts the king in check
	return (x >= 0 && x <= 7 && y >= 0 && y <= 7);
}

bool Chess::castlingOk(bool side, Player p)
{ // side = true -> kingside
	//To add :
	//return false if at least one of the pieces have moved
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

bool Chess::castling(bool side, Player p)
{
	if (!castlingOk(side, p))
		return false;
	int row = 7;
	if (p.getColor())
		row = 0;
	if (side)
	{
		Piece king = board.getPiece(4, row);
		moveOnePiece(king, 6, row);
		Piece rook = board.getPiece(7, row);
		moveOnePiece(rook, 5, row);
	}
	else
	{
		Piece king = board.getPiece(4, row);
		moveOnePiece(king, 2, row);
		Piece rook = board.getPiece(0, row);
		moveOnePiece(rook, 3, row);
	}
	return true;
}

Piece Chess::isMoveOk(Rank r, Board b, Player p, int x, int y, bool eat, int oldX, int oldY)
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
		if (oldX == -1 && oldY == -1)
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
		if (isChecked(p, b, x, y))
		{
			return Piece(true, Rank::EMPTY, 0, 0);
		}
	}

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
bool isChecked(Player p, Board b, int x, int y) //-> retourne vrai si mettre le roi du joueur p en [x][y] le mettrai en echec
{
	return false;
}
