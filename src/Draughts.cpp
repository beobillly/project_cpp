#include "../include/Draughts.hpp"
#include "../include/Util.hpp"
#include <fstream>
#include <ctype.h>
#include <tuple>
#include <vector>
#include <algorithm>


using namespace std;

Draughts::Draughts()
{
}

void Draughts::move(Player p, string path) {
    ofstream history;
    history.open(path);
    Player otherPlayer = player_black;
    if (!p.getColor()) otherPlayer = player_white;
    startLoop:
    cout << p.getName() << "'s turn" << endl;
    cout << "Please enter your move : " << endl;
    string move("");
    cin >> move;
    bool eat = false;
    if (move[3] == 'x') eat = true;
    vector<string> splitMove;
    if (eat) splitMove = split(move, "x");
    else splitMove = split(move, "-");
    if (splitMove.size() < 2) {
        cout << "Invalid move : not enough information" <<endl;
        goto startLoop;
    }
    if (!eat && splitMove.size() > 2){
        cout << "You can only move once without eating. \n If you want to eat please replace your - with x" << endl;
        goto startLoop;
    }
    int pos = stoi(splitMove[0]);
    int line = getLine(pos);
    int column = getColumn(pos);
    if (line == -1 || column == -1){
        cout << "Invalid move : invalid square number" << endl;
        goto startLoop;
    }
    Piece toMove = board.getPiece(line, column);
    if (toMove.getColor() != p.getColor()){
        cout << "Invalid move : you're trying to move the other player's piece" << endl;
        goto startLoop;
    }
    if (toMove.getRank() == Rank::EMPTY){
        cout << "Invalid move : this square is empty" << endl;
        goto startLoop;
    }
    if (!eat){
        int destPos = stoi(splitMove[1]);
        int destLine = getLine(destPos);
        int destColumn = getColumn(destPos);
        if ((p.getColor() && destLine < line-1) || (!p.getColor() && destLine > line+1) ||
                (! (destColumn == column +1 || destColumn == column -1))){
            cout << "Invalid move : you can only move to an adjacent square" << endl;
            goto startLoop;
        }
        if (board.getPiece(destLine, destColumn).getRank() == Rank::EMPTY){
            board.movePiece(toMove, destLine, destColumn);
        } else {
            cout << "Invalid move : your destination square is not empty" << endl;
            goto startLoop;
        }
    } else {
        
    }

    if (!p.getColor()) {
        string tmp = "(";
        tmp += move;
        tmp += ")";
        move = tmp;
    }
    history << move << endl;
    history.close();
}

int Draughts::getLine(int pos) {
    if (pos < 0) return -1;
    if (pos <= 5) return 0;
    if (pos <= 10) return 1;
    if (pos <= 15) return 2;
    if (pos <= 20) return 3;
    if (pos <= 25) return 4;
    if (pos <= 30) return 5;
    if (pos <= 35) return 6;
    if (pos <= 40) return 7;
    if (pos <= 45) return 8;
    if (pos <= 50) return 9;
    return -1;
}

int Draughts::getColumn(int pos){
    if (pos < 0) return -1;
    int column = pos;
    if (pos > 9) column = pos%10;
    switch (column){
        case 6 : return 0;
        case 1 : return 1;
        case 7 : return 2;
        case 2 : return 3;
        case 8 : return 4;
        case 3 : return 5;
        case 9 : return 6;
        case 4 : return 7;
        case 0 : return 8;
        case 5 : return 9;
        default: return -1;
    }
}

bool Draughts::checkEat(bool eat, Player p, int x, int y) {}
vector<tuple<int, int>> Draughts::getQueenMoves(Piece p, bool eat) {}
vector<tuple<int, int>> Draughts::getPawnMoves(Piece p, bool eat) {}
bool Draughts::coordInVector(vector<tuple<int, int>> vec, int x, int y) {}
bool Draughts::simulateMove(Piece toMove, int x, int y, bool eat, Player p) {}
bool Draughts::pawnCheck(bool color, int x, int y) {}
void Draughts::help(Player p) {}
string Draughts::getMoveNotation(Piece piece, int x, int y, bool eat, char rank) {}