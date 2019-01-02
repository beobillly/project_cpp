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

void Draughts::move(Player p, string path)
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
    vector<string> split;
    //boost::split(split, move, [](char c){return c == '-';});
}

bool Draughts::checkEat(bool eat, Player p, int x, int y) {}
vector<tuple<int, int>> Draughts::getQueenMoves(Piece p, bool eat) {}
vector<tuple<int, int>> Draughts::getPawnMoves(Piece p, bool eat) {}
bool Draughts::coordInVector(vector<tuple<int, int>> vec, int x, int y) {}
bool Draughts::simulateMove(Piece toMove, int x, int y, bool eat, Player p) {}
bool Draughts::pawnCheck(bool color, int x, int y) {}
void Draughts::help(Player p) {}
string Draughts::getMoveNotation(Piece piece, int x, int y, bool eat, char rank) {}