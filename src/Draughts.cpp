#include "../include/Draughts.hpp"
#include "Game.cpp"

using namespace std;

Draughts::Draughts()
{
}

Draughts::Draughts(bool classique)
{
    if (classique)
    {
        this->game_type = Game_type::DAME_CLASSIQUE;
        taille = 10;
    }
    else
    {
        this->game_type = Game_type::DAME_ANGLAISE;
        taille = 8;
    }
    Board board = Board(taille, taille);
    this->board = board;
    init();
    cout << "Would you like to load a game from a file ? (Y/N)" << endl;
    string answer("");
    cin >> answer;
    if (answer == "Y")
    {
        cout << "Please enter the file path : ";
        cin >> answer;
        readFile(answer);
    }
}

void Draughts::move(Player &p, string move, string path)
{
    if (p.getName() == "Robot" && move == "")
    {
        robotMove(p, path);
        return;
    }
    ofstream history;
    history.open(path);
    Player otherPlayer = player_black;
    if (!p.getColor())
        otherPlayer = player_white;
    bool read = false;
    if (move != "")
        read = true;
startLoop:
    //test
    Piece reine = Piece(true, Rank::QUEEN, 5, 4);
    board.setPiece(reine, 5, 4);
    refresh();
    cout << p.getName() << "'s turn" << endl;
    cout << "Please enter your move : " << endl;
    move = "";
    cin >> move;
    if (move == "help")
    {
        help(p);
        move = "";
        goto startLoop;
    }
    bool eat = false;
    if (move[2] == 'x' || move[1] == 'x')
        eat = true;
    vector<string> splitMove;
    //std::cout << "eat = " << eat << '\n';
    if (eat)
        splitMove = splitString(move, 'x');
    else
        splitMove = splitString(move, '-');

    /*for (int i = 0; i < int(splitMove.size()); i++)
    {
        std::cout << splitMove.at(i) << "\n";
    }
    std::cout << "" << '\n';*/

    if (int(splitMove.size()) < 2)
    {
        cout << "Invalid move : not enough information" << endl;
        if (read)
            return;
        else
        {
            move = "";
            goto startLoop;
        }
    }
    if (!eat && splitMove.size() > 2)
    {
        cout << "You can only move once without eating. \n If you want to eat please replace your - with x" << endl;
        if (read)
            return;
        else
        {
            move = "";
            goto startLoop;
        }
    }
    int pos = stoi(splitMove[0]);
    int line = getLine(pos);
    int column = getColumn(pos);
    Piece piece = board.getPiece(line, column);
    Piece p_ = Piece(true, Rank::EMPTY, line, column);

    if (line == -1 || column == -1)
    {
        cout << "Invalid move : invalid square number" << endl;
        if (read)
            return;
        else
        {
            move = "";
            goto startLoop;
        }
    }
    Piece toMove = board.getPiece(line, column);
    if (toMove.getColor() != p.getColor())
    {
        cout << "Invalid move : you're trying to move the other player's piece" << endl;
        if (read)
            return;
        else
        {
            move = "";
            goto startLoop;
        }
    }
    if (toMove.getRank() == Rank::EMPTY)
    {
        cout << "Invalid move : this square is empty" << endl;
        if (read)
            return;
        else
        {
            move = "";
            goto startLoop;
        }
    }
    vector<int> moves;

    for (int i = 0; i < int(splitMove.size()); i++)
    {
        moves.insert(moves.end(), stoi(splitMove.at(i)));
    }

    if (isMoveOk(moves, eat, piece))
    {
        p.eatPiece(piece);
        // for (int i = 0; i < p.getPieces().size(); i++)
        // {
        //     cout << p.getPieces().at(i).getPosX() << " " << p.getPieces().at(i).getPosY() << '\n';
        // }
        int destPos = stoi(splitMove[int(splitMove.size() - 1)]);
        int destLine = getLine(destPos);
        int destColumn = getColumn(destPos);
        piece.setPos(destLine, destColumn);
        board.setPiece(piece, destLine, destColumn);
        board.setPiece(p_, line, column);
        p.addPiece(piece);
        if (eat)
        {
            vector<tuple<int, int>> eaten = eatenP(moves);
            for (int i = 0; i < int(eaten.size()); i++)
            {
                p.eatPiece(board.getPiece(std::get<0>(eaten.at(i)), std::get<1>(eaten.at(i))));
                board.removePiece(std::get<0>(eaten.at(i)), std::get<1>(eaten.at(i)));
            }
        }

        if (piece.getRank() == Rank::MAN)
        {
            if (p.getColor() && destLine == 0)
            {
                Piece dame = Piece(true, Rank::QUEEN, piece.getPosX(), piece.getPosY());
                board.setPiece(dame, dame.getPosX(), dame.getPosY());
                p.addPiece(dame);
            }
            if (!p.getColor() && destLine == taille - 1)
            {
                Piece dame = Piece(false, Rank::QUEEN, piece.getPosX(), piece.getPosY());
                board.setPiece(dame, dame.getPosX(), dame.getPosY());
                p.addPiece(dame);
            }
        }
    }
    else
    {
        if (read)
            return;
        else
        {
            move = "";
            goto startLoop;
        }
    }

    if (!p.getColor())
    {
        string tmp = "(";
        tmp += move;
        tmp += ")";
        move = tmp;
    }
    history << move << endl;
    history.close();
}

int Draughts::getLine(int pos)
{
    if (pos < 0)
        return -1;
    if (pos <= 5)
        return 0;
    if (pos <= 10)
        return 1;
    if (pos <= 15)
        return 2;
    if (pos <= 20)
        return 3;
    if (pos <= 25)
        return 4;
    if (pos <= 30)
        return 5;
    if (pos <= 35)
        return 6;
    if (pos <= 40)
        return 7;
    if (pos <= 45)
        return 8;
    if (pos <= 50)
        return 9;
    return -1;
}

int Draughts::getColumn(int pos)
{
    if (pos < 0)
        return -1;
    int column = pos;
    if (pos > 9)
        column = pos % 10;
    switch (column)
    {
    case 6:
        return 0;
    case 1:
        return 1;
    case 7:
        return 2;
    case 2:
        return 3;
    case 8:
        return 4;
    case 3:
        return 5;
    case 9:
        return 6;
    case 4:
        return 7;
    case 0:
        return 8;
    case 5:
        return 9;
    default:
        return -1;
    }
}

bool Draughts::hasLost(Player p)
{
    if (p.getPieces().size() == 0)
    {
        std::cout << "the player " << p.getName() << " a perdu" << '\n';
        if (p.getColor())
        {
            std::cout << "Congratulation to the BLACK for the win" << '\n';
        }
        else
        {
            std::cout << "Congratulation to the WHITE for the win" << '\n';
        }
        return true;
    }
    return false;
}

bool Draughts::isMoveOk(vector<int> positions, bool eat, Piece &p)
{
    int pos, line, column, Ipos, Iline, Icolumn;
    for (int i = 0; i < int(positions.size()); i++)
    {
        pos = positions.at(i);
        line = getLine(pos);
        column = getColumn(pos);
        if (!coordOk(taille - 1, taille - 1, line, column))
        { //Si les coordonnées x et/ou y sont en dehors du tableau
            char file = column + 65;
            cout << "Invalid move : " << file << line + 1 << " is out of bounds" << endl;
            return false;
        }
    }

    if (int(positions.size()) < 2)
    {
        std::cout << "Invalid move : you must at least one position to move" << '\n';
        return false;
    }

    if (!eat && int(positions.size()) > 2)
    {
        std::cout << "Invalid move : you can not move more than one position if you don't eat" << '\n';
        return false;
    }

    pos = positions.at(1);
    line = getLine(pos);
    column = getColumn(pos);

    Ipos = positions.at(0);
    Iline = getLine(Ipos);
    Icolumn = getColumn(Ipos);

    tuple<int, int> t = make_tuple(line, column);

    if (!eat && p.getRank() == Rank::MAN)
    {
        vector<tuple<int, int>> moves = getManMoves(p, eat, Iline, Icolumn);
        //std::cout << "moves.size() = " << moves.size() << '\n';
        for (int i = 0; i < int(moves.size()); i++)
        {
            /* std::cout << "t(0) = " << std::get<0>(t);
            std::cout << "et t(1) = " << std::get<1>(t) << '\n';*/
            if (moves.at(i) == t)
            {
                return true;
            }
        }
        std::cout << "Invalid move : " << Ipos << "-" << pos << " is not a possible move" << '\n';
        return false;
    }

    else if (!eat && p.getRank() == Rank::QUEEN)
    {
        vector<tuple<int, int>> moves = getQueenMoves(p, eat, Iline, Icolumn);
        std::cout << "je suis ici" << '\n';
        std::cout << "moves.size() = " << moves.size() << '\n';

        for (int i = 0; i < int(moves.size()); i++)
        {
            std::cout << "t(0) = " << std::get<0>(t);
            std::cout << "et t(1) = " << std::get<1>(t) << '\n';
            if (moves.at(i) == t)
            {
                return true;
            }
        }
        std::cout << "Invalid move : " << Ipos << "-" << pos << " is not a possible move" << '\n';
        return false;
    }

    else if (eat && p.getRank() == Rank::QUEEN)
    {
        vector<tuple<int, int>> moves;
        for (int j = 1; j < int(positions.size()); j++)
        {
            pos = positions.at(j);
            line = getLine(pos);
            column = getColumn(pos);
            Ipos = positions.at(j - 1);
            Iline = getLine(Ipos);
            Icolumn = getColumn(Ipos);

            t = make_tuple(line, column);
            moves = getQueenMoves(p, eat, Iline, Icolumn);
            bool isok = false;
            for (int i = 0; i < int(moves.size()); i++)
            {
                if (moves.at(i) == t)
                {
                    isok = true;
                }
            }
            if (!isok)
            {
                std::cout << "Invalid move : " << Ipos << "x" << pos << " is not a possible move" << '\n';
                return false;
            }
        }
        return true;
    }

    else if (eat && p.getRank() == Rank::MAN)
    {
        vector<tuple<int, int>> moves;
        //std::cout << "je mange avec un pion" << '\n';
        for (int j = 1; j < int(positions.size()); j++)
        {
            pos = positions.at(j);
            line = getLine(pos);
            column = getColumn(pos);
            Ipos = positions.at(j - 1);
            Iline = getLine(Ipos);
            Icolumn = getColumn(Ipos);

            t = make_tuple(line, column);
            moves = getManMoves(p, eat, Iline, Icolumn);
            bool isok = false;
            for (int i = 0; i < int(moves.size()); i++)
            {
                /*std::cout << "t(0) = " << std::get<0>(t);
                std::cout << "et t(1) = " << std::get<1>(t) << '\n';
                std::cout << "moves.at(i)(0) = " << std::get<0>(moves.at(i));
                std::cout << "et moves.at(i)(1)) = " << std::get<1>(moves.at(i)) << '\n';*/
                if (moves.at(i) == t)
                {
                    //std::cout << "j'ai trouvé une bonne position" << '\n';
                    isok = true;
                }
            }
            if (!isok)
            {
                std::cout << "Invalid move : " << Ipos << "x" << pos << " is not a possible move" << '\n';
                return false;
            }
        }
        return true;
    }
    else
    {
        std::cout << "Invalid move : Unknown move" << '\n';
        return false;
    }
}

vector<tuple<int, int>> Draughts::getQueenMoves(Piece &p, bool eat, int x, int y)
{
    vector<tuple<int, int>> possibleMoves;

    /* UP LEFT */ for (int i = 1; i <= taille - 1; i++)
        if (coordOk(taille - 1, taille - 1, x - i, y - i))
        {
            addQueenMove(x - i, y - i, eat, possibleMoves, p, i, 1);
        }

    /* UP RIGHT */ for (int i = 1; i <= taille - 1; i++)
        if (coordOk(taille - 1, taille - 1, x - i, y + i))
        {
            addQueenMove(x - i, y + i, eat, possibleMoves, p, i, 2);
        }

    /* DOWN LEFT */ for (int i = 1; i <= taille - 1; i++)
        if (coordOk(taille - 1, taille - 1, x + i, y - i))
        {
            addQueenMove(x + i, y - i, eat, possibleMoves, p, i, 3);
        }

    /* DOWN RIGHT */ for (int i = 1; i <= taille - 1; i++)
        if (coordOk(taille - 1, taille - 1, x + i, y + i))
        {
            addQueenMove(x + i, y + i, eat, possibleMoves, p, i, 4);
        }

    return possibleMoves;
}
/*
direction : 1 -> UP LEFT
            2 -> UP RIGHT
            3 -> DOWN LEFT
            4 -> DOWN RIGHT
*/
bool Draughts::addQueenMove(int x, int y, bool eat, vector<tuple<int, int>> &moves, Piece &p, int depart, int direction)
{
    bool squareHasPiece = board.getPiece(x, y).getRank() != Rank::EMPTY;
    bool isOtherColor = board.getPiece(x, y).getColor() != p.getColor();
    auto it = moves.end();
    if ((!eat && !squareHasPiece) || (eat && !squareHasPiece))
        moves.insert(it, (make_tuple(x, y)));
    else if (!eat && squareHasPiece)
        return false;
    else if (eat && squareHasPiece && isOtherColor)
    {
        if (direction == 1)
        {
            /* UP LEFT */ for (int i = depart; i <= taille - 1; i++)
                if (coordOk(taille - 1, taille - 1, x - i, y - i))
                {
                    addQueenMoveNext(x - i, y - i, moves, p);
                }
        }
        else if (direction == 2)
        {
            /* UP RIGHT */ for (int i = depart; i <= taille - 1; i++)
                if (coordOk(taille - 1, taille - 1, x - i, y + i))
                {
                    addQueenMoveNext(x - i, y + i, moves, p);
                }
        }
        else if (direction == 3)
        {
            /* DOWN LEFT */ for (int i = depart; i <= taille - 1; i++)
                if (coordOk(taille - 1, taille - 1, x + i, y - i))
                {
                    addQueenMoveNext(x + i, y - i, moves, p);
                }
        }
        else
        {
            /* DOWN RIGHT */ for (int i = depart; i <= taille - 1; i++)
                if (coordOk(taille - 1, taille - 1, x + i, y + i))
                {
                    addQueenMoveNext(x + i, y + i, moves, p);
                }
        }

        return false;
    }
    return true;
}

bool Draughts::addQueenMoveNext(int x, int y, vector<tuple<int, int>> &moves, Piece &p)
{
    bool squareHasPiece = board.getPiece(x, y).getRank() != Rank::EMPTY;
    auto it = moves.end();
    if (!squareHasPiece)
    {
        moves.insert(it, (make_tuple(x, y)));
        return true;
    }
    else
    {
        return false;
    }
}

vector<tuple<int, int>> Draughts::getManMoves(Piece &p, bool eat, int x, int y)
{
    vector<tuple<int, int>> possibleMoves;

    int i = 1;

    //std::cout << "position x : " << x << "  position y : " << y << '\n';

    /* UP LEFT */
    if (coordOk(taille - 1, taille - 1, x - i, y - i))
    {
        //std::cout << "on test UP left" << '\n';
        addManMove(x - i, y - i, eat, possibleMoves, p, i, 1);
    }

    /* UP RIGHT */
    if (coordOk(taille - 1, taille - 1, x - i, y + i))
    {
        //std::cout << "on test UP right" << '\n';
        addManMove(x - i, y + i, eat, possibleMoves, p, i, 2);
    }
    /* DOWN LEFT */
    if (coordOk(taille - 1, taille - 1, x + i, y - i))
    {
        //std::cout << "on test Down left" << '\n';
        addManMove(x + i, y - i, eat, possibleMoves, p, i, 3);
    }
    /* DOWN RIGHT */
    if (coordOk(taille - 1, taille - 1, x + i, y + i))
    {
        //std::cout << "on test DOWN right" << '\n';
        addManMove(x + i, y + i, eat, possibleMoves, p, i, 4);
    }
    return possibleMoves;
}

void Draughts::addManMove(int x, int y, bool eat, vector<tuple<int, int>> &moves, Piece &p, int depart, int direction)
{
    bool squareHasPiece = board.getPiece(x, y).getRank() != Rank::EMPTY;
    bool isOtherColor = board.getPiece(x, y).getColor() != p.getColor();

    //std::cout << "je regarde la case " << x << " " << y << '\n';
    auto it = moves.end();
    {
        if (direction == 1)
        {
            /* UP LEFT */
            if ((!eat && !squareHasPiece && p.getColor()))
            {
                //std::cout << "jinsere un pion dans mon vector" << '\n';
                moves.insert(it, (make_tuple(x, y)));
            }
            else if (eat && squareHasPiece && isOtherColor && (p.getColor() || game_type == Game_type::DAME_CLASSIQUE))
            {
                if (coordOk(taille - 1, taille - 1, x - 1, y - 1) && board.getPiece(x - 1, y - 1).getRank() == Rank::EMPTY)
                {
                    //std::cout << "jinsere un pion dans mon vector" << '\n';

                    it = moves.end();
                    moves.insert(it, (make_tuple(x - 1, y - 1)));
                }
            }
        }
        else if (direction == 2)
        {
            /* UP RIGHT */
            if ((!eat && !squareHasPiece && p.getColor()))
            {
                moves.insert(it, (make_tuple(x, y)));
                //std::cout << "jinsere un pion dans mon vector" << '\n';
            }
            else if (eat && squareHasPiece && isOtherColor && (p.getColor() || game_type == Game_type::DAME_CLASSIQUE))
            {
                /*std::cout << "je peux peut-etre manger" << '\n';
                std::cout << coordOk(taille - 1, taille - 1, x - 1, y + 1) << '\n';
                std::cout << "x = " << x - 1 << " y = " << y + 1 << '\n';*/
                if (coordOk(taille - 1, taille - 1, x - 1, y + 1) && board.getPiece(x - 1, y + 1).getRank() == Rank::EMPTY)
                {
                    //std::cout << "jinsere un pion dans mon vector" << '\n';

                    it = moves.end();
                    moves.insert(it, (make_tuple(x - 1, y + 1)));
                }
            }
        }
        else if (direction == 3)
        {
            /* DOWN LEFT */
            if ((!eat && !squareHasPiece && !p.getColor()))
            {
                moves.insert(it, (make_tuple(x, y)));
                //std::cout << "jinsere un pion dans mon vector" << '\n';
            }
            else if (eat && squareHasPiece && isOtherColor && (!p.getColor() || game_type == Game_type::DAME_CLASSIQUE))
            {
                if (coordOk(taille - 1, taille - 1, x + 1, y - 1) && board.getPiece(x + 1, y - 1).getRank() == Rank::EMPTY)
                {
                    it = moves.end();
                    //std::cout << "jinsere un pion dans mon vector" << '\n';

                    moves.insert(it, (make_tuple(x + 1, y - 1)));
                }
            }
        }
        else
        {
            /* DOWN RIGHT */
            if ((!eat && !squareHasPiece && !p.getColor()))
            {
                moves.insert(it, (make_tuple(x, y)));
                //std::cout << "jinsere un pion dans mon vector" << '\n';
            }
            else if (eat && squareHasPiece && isOtherColor && (!p.getColor() || game_type == Game_type::DAME_CLASSIQUE))
            {
                if (coordOk(taille - 1, taille - 1, x + 1, y + 1) && board.getPiece(x + 1, y + 1).getRank() == Rank::EMPTY)
                {
                    it = moves.end();
                    //std::cout << "jinsere un pion dans mon vector" << '\n';

                    moves.insert(it, (make_tuple(x + 1, y + 1)));
                }
            }
        }
    }
}

vector<tuple<int, int>> Draughts::eatenP(vector<int> positions)
{
    vector<tuple<int, int>> res;
    for (int i = 0; i < int(positions.size() - 1); i++)
    {
        int pos = positions.at(i);
        int line = getLine(pos);
        int column = getColumn(pos);
        /*UP LEFT*/
        if ((positions.at(i) - positions.at(i + 1)) == taille + 1)
        {
            res.insert(res.end(), make_tuple(line - 1, column - 1));
        }
        /*UP RIGHT*/
        if ((positions.at(i) - positions.at(i + 1)) == taille - 1)
        {
            res.insert(res.end(), make_tuple(line - 1, column + 1));
        }
        /*DOWN LEFT*/
        if ((positions.at(i) - positions.at(i + 1)) == -taille + 1)
        {
            res.insert(res.end(), make_tuple(line + 1, column - 1));
        }
        /*DOWN RIGHT*/
        else
        {
            res.insert(res.end(), make_tuple(line + 1, column + 1));
        }
    }
    return res;
}

void Draughts::help(Player &p)
{
    for (Piece piece : p.getPieces())
    {
        vector<tuple<int, int>> movesWithEat = getMoves(piece, true);
        vector<tuple<int, int>> movesWithoutEat = getMoves(piece, false);
        for (int i = 0; i < int(movesWithEat.size()); i++)
            cout << getMoveNotation(piece, get<0>(movesWithEat[i]), get<1>(movesWithEat[i]), true) << endl;
        for (int i = 0; i < int(movesWithoutEat.size()); i++)
            cout << getMoveNotation(piece, get<0>(movesWithoutEat[i]), get<1>(movesWithoutEat[i]), false) << endl;
    }
}

vector<tuple<int, int>> Draughts::getMoves(Piece &p, bool eat)
{
    switch (p.getRank())
    {
    case Rank::QUEEN:
        return getQueenMoves(p, eat, p.getPosX(), p.getPosY());
    case Rank::MAN:
        return getManMoves(p, eat, p.getPosX(), p.getPosY());
    default:
        return getManMoves(p, eat, p.getPosX(), p.getPosY());
    }
}

string Draughts::getMoveNotation(Piece &piece, int x, int y, bool eat)
{
    string move = "";
    int posDep = ((piece.getPosY() / 2 + 1) + (piece.getPosX() * (taille / 2)));
    move += to_string(posDep);
    if (eat)
        move += 'x';
    else
        move += '-';
    int posArr = ((y / 2 + 1) + (x * (taille / 2)));
    move += to_string(posArr);
    return move;
}
void Draughts::robotMove(Player &robot, string path)
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
    if (movesWithoutEat.empty() && movesWithEat.empty())
        goto Loop;
    int randMove = std::rand() % (movesWithEat.size() + movesWithoutEat.size() - 1);

    //TEST
    int file = 8 - toMove.getPosX();
    char rank = toMove.getPosY() + 65;
    cout << "Chosen piece : " << toMove.getRank() << file << rank << endl;
    cout << "With eat size : " << movesWithEat.size() << endl;
    cout << "Without eat size : " << movesWithoutEat.size() << endl;
    cout << "rand move : " << randMove << endl;
    //TEST

    if (randMove < int(movesWithEat.size()) && !movesWithEat.empty())
    {
        Player other = getPlayerWhite();
        if (robot.getColor())
            other = getPlayerBlack();
        int x = get<0>(movesWithEat[randMove]);
        int y = get<1>(movesWithEat[randMove]);

        Piece toEat = board.getPiece(x, y);
        other.eatPiece(toEat);
        robot.eatPiece(toMove);
        board.movePiece(toMove, x, y);
        robot.addPiece(toMove);
        moveDone = getMoveNotation(toMove, x, y, true);
    }
    else
    {
        int x = get<0>(movesWithoutEat[randMove]);
        int y = get<1>(movesWithoutEat[randMove]);

        robot.eatPiece(toMove);
        board.movePiece(toMove, x, y);
        robot.addPiece(toMove);
        moveDone = getMoveNotation(toMove, x, y, false);
    }
    history << moveDone << endl;
    history.close();
}