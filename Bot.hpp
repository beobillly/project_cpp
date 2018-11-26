#include "Player.hpp"

class Bot: Player
{
public:
    Bot();
    Bot(Bot &&) = default;
    Bot(const Bot &) = default;
    Bot &operator=(Bot &&) = default;
    Bot &operator=(const Bot &) = default;
    ~Bot();

private:
    
};