#include "../include/Draughts.hpp"
#include "../include/Util.hpp"

using namespace std;

Draughts::Draughts()
{
}

bool Draughts::checkEat(bool eat, Player p, int x, int y) {}
vector<tuple<int, int>> Draughts::getQueenMoves(Piece p, bool eat) {}
vector<tuple<int, int>> Draughts::getPawnMoves(Piece p, bool eat) {}
bool Draughts::coordInVector(vector<tuple<int, int>> vec, int x, int y) {}
bool Draughts::simulateMove(Piece toMove, int x, int y, bool eat, Player p) {}
bool Draughts::queenOrRookCheck(bool color, int x, int y) {}
bool Draughts::queenOrBishopCheck(bool color, int x, int y) {}
bool Draughts::pawnCheck(bool color, int x, int y) {}
void Draughts::help(Player p) {}
string Draughts::getMoveNotation(Piece piece, int x, int y, bool eat, char rank) {}