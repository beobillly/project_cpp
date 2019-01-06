#include "../src/Rank.cpp"
#include <vector>
#include <string>

using namespace std;

int letterToNumber(char c);
Rank getRankFromChar(char c);
bool isMaj(char c);
bool isMin(char c);
bool coordOk(int height, int length, int x, int y);
vector<string> splitString(string s, char delimiter);
void debug (string s);
char rankToChar(Rank r);