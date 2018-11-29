PROJET C++
----------

-------------------------------------------------------------------------------------------------------------------------------------

JEU CHOISI :
----------

— Dame Classique + Variante

— Echec classique

-------------------------------------------------------------------------------------------------------------------------------------

CONVENTIONS :
-------------

— TRUE = BLANC (WHITE)

— FALSE = NOIR (BLACK)

— [0,0] EN HAUT A GAUCHE

-----------------------------------------------------------------------------------------------------------------------------------

TRAVAIL A FAIRE :
-----------------

— affichage, à chaque tour, du plateau (damier, échiquier, . . .) en mode texte ; 
le mode texte est impératif, il n’est pas question d’utiliser une interface graphique ;

------------------------------------------------------------------------

— lecture du prochain coup au clavier ; pour les coups des dames et échec,
la notation normalisée est impérative ;

------------------------------------------------------------------------


— vérification des règles (refus de jouer un coup invalide : on redemande à
l’utilisateur) ;

------------------------------------------------------------------------

— détection de la victoire (arrêt du jeu avec affichage du vainqueur) ;

------------------------------------------------------------------------

— aide de jeu donnant la liste des coups jouables à un moment donné ;

------------------------------------------------------------------------

— à des fins de déboguage : un joueur automatique (robot) aléatoire, qui
se contente de respecter les règles mais n’a pas de stratégie ;

------------------------------------------------------------------------

— à des fins de test : possibilité de faire jouer une partie depuis une liste de coups décrits dans un fichier ; 
Le projet devra être fourni avec un jeu de tests sous cette forme-là, associés à une description du résultat attendu.

------------------------------------------------------------------------

— gestion de l’historique d’une partie (annulation, rejeu, ...) ;

------------------------------------------------------------------------

— sauvegarde et restauration d’une partie vers/depuis un fichier.

---------------------------------------------------------------------------------------------------------------------------------------

TRAVAIL RÉALISÉ :
-----------------

— Makefile OK (ne pas toucher)

— Patron des classes sans les fonctions OK (à completer)

— Getter OK (ne pas toucher aux existants, à compléter si besoin)

— SETTER (quand il faut)

— Fonctions des classes (le plus gros du travail)

------------------------------------------------------------------------

Arborescence du projet : 

    /build               : tous les produits de make
    /build/apps          : le fichier executable généré
    /build/object/src    : tous les fichiers .o générés
    /include             : tous les .hpp
    /src                 : tous les .cpp


------------------------------------------------------------------------

Classes présentes :
    
    .hpp
        Board
        Bot
        Game
        Piece
        Player
        Square    
    
    .cpp
        Board <- le plateau de Square>
        Bot <- un joueur robot >
        Game_type <- le type de jeu (ECHEC, DAME, etc)>
        Game <- le jeu qui met tout en place>
        Launcher <- le lanceur du jeu>
        Piece <- une piece de couleur avec un rang>
        Player <- un joueur humain>
        Rank <- le rang de chaque piece (KING, QUEEN, etc)>
        Square <- une case qui contiendra une piece>


------------------------------------------------------------------------

A faire : 

— deboguer la fonction init() de la classe Board
            on a une mauvaise initialisation du tableau et j'ai pas encore résolu le bug

— verifier si on doit free() en mode c++ ou pas ?

— Affecter à chaque type de partie ses règles

— La classe Launcher


------------------------------------------------------------------------

Attention :

Quand on fait des imports on fait : 

                — ../include/mon_import.hpp quand on est dans un .cpp

                — ../src/mon_import.cpp quand on est dans un .hpp (cas des enums par exemple)

Quand on declare une classe :
                
                — ne pas oublier de déclarer le constructeur vide dans le .cpp

-----------------------------------------------------------------------------------------------------------------------------------

HOW TO USE : 
-----------

1/ Allez dans le répertoire courant du dossier

2/ tapez make

3/ tapez ./build/apps/Game

4/ Suivez les instructions


-----------------------------------------------------------------------------------------------------------------------------------

OPTIONS POUR LE MAKEFILE

Les options disponibles dans le Makefile :

        build 
        clean 
        debug 
        release

tapez par exemple "make clean" pour lancer la commande

-----------------------------------------------------------------------------------------------------------------------------------
