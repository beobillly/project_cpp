#ifndef LAUNCHER
#define LAUNCHER
#include "../include/Chess.hpp"
#include "../include/Draughts.hpp"
#include "../include/Util.hpp"
#include <time.h>

int main(int argc, char **argv)
{
startLoop:
    string name("");
    bool ok_tmp = false;
    std::srand(time(NULL));
    int r = std::rand() % 3 + 1;
    int res = 0;
    while (!ok_tmp)
    {
        std::cout << "A quel jeu voulez vous jouer ?\n1 -> ECHEC\n2 -> DAME_CLASSIQUE\n3 -> DAME_ANGLAISE\n4 -> ALEATOIRE\n5 -> QUITTER" << '\n';

        std::cin >> name;
        try
        {
            res = std::stoi(name);
            if (res < 1 || res > 5)
            {
                ok_tmp = false;
                std::cout << "Vous n'avez pas rentré une valeur valide (1 - 2 - 3 - 4 - 5)\nVeuillez recommencer" << '\n';
            }
            else
            {
                ok_tmp = true;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "Vous n'avez pas rentré une valeur valide (1 - 2 - 3 - 4 - 5)\nVeuillez recommencer" << '\n';
        }
    }
    if (res == 4)
    {
        res = r;
    }
    if (res == 1)
    {
        //ECHEC;
        Chess chess = Chess();
        //TEST
        //showPieces(chess.getPlayerBlack());
        //showPieces(chess.getPlayerWhite());
        chess.test();
        //TEST
        while (true)
        {
            chess.refresh();
            chess.move(chess.getPlayerWhite(), "", "../history.txt");
            if (chess.checkMate(chess.getPlayerBlack()))
            {
                std::cout << chess.getPlayerWhite().getName() << " wins !" << endl;
                goto gameOver;
            }
            chess.refresh();
            chess.move(chess.getPlayerBlack(), "", "../history.txt");
            if (chess.checkMate(chess.getPlayerWhite()))
            {
                std::cout << chess.getPlayerBlack().getName() << " wins !" << endl;
                goto gameOver;
            }
        }
    }
    else if (res == 2)
    {
        Draughts draughts = Draughts(true);
        while (true)
        {
            draughts.refresh();
            draughts.move(draughts.getPlayerWhite(), "", "../history.txt");
            if (draughts.hasLost(draughts.getPlayerBlack()))
            {
                std::cout << draughts.getPlayerWhite().getName() << " wins !" << endl;
                goto gameOver;
            }
            draughts.refresh();
            draughts.move(draughts.getPlayerBlack(), "", "../history.txt");
            if (draughts.hasLost(draughts.getPlayerWhite()))
            {
                std::cout << draughts.getPlayerBlack().getName() << " wins !" << endl;
                goto gameOver;
            }
        }
    }
    else if (res == 3)
    {
        Draughts draughts = Draughts(false);
        while (true)
        {

            draughts.move(draughts.getPlayerWhite(),"", "../history.txt");
            if (draughts.hasLost(draughts.getPlayerBlack()))
            {
                std::cout << draughts.getPlayerWhite().getName() << " wins !" << endl;
                goto gameOver;
            }

            draughts.move(draughts.getPlayerBlack(),"", "../history.txt");
            if (draughts.hasLost(draughts.getPlayerWhite()))
            {
                std::cout << draughts.getPlayerBlack().getName() << " wins !" << endl;
                goto gameOver;
            }
        }
    }
    else
    {
        std::cout << "bye" << '\n';
        exit(0);
    }

gameOver:
    std::cout << "Do you want to play again ? \n1 -> REPLAY \n2 -> EXIT" << endl;
    string answer("");
    std::cin >> answer;
    int ans = std::stoi(answer);
    if (ans == 1)
        goto startLoop;
    else if (ans == 2)
        exit(0);
    else
        goto gameOver;
}
#endif