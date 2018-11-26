projet de c++

Dame Classique -> Variante

Echec classique

CONVENTIONS :

TRUE = BLANC (WHITE)
FALSE = NOIR (BLACK)

[0,0] EN HAUT A GAUCHE

affichage, à chaque tour, du plateau (damier, échiquier, . . .) en mode
texte ; le mode texte est impératif, il n’est pas question d’utiliser
une interface graphique ;
— lecture du prochain coup au clavier ; pour les coups des dames et échec,
la notation normalisée est impérative ;
— vérification des règles (refus de jouer un coup invalide : on redemande à
l’utilisateur) ;
— détection de la victoire (arrêt du jeu avec affichage du vainqueur) ;
— aide de jeu donnant la liste des coups jouables à un moment donné ;
— à des fins de déboguage : un joueur automatique (robot) aléatoire, qui
se contente de respecter les règles mais n’a pas de stratégie ;
— à des fins de test : possibilité de faire jouer une partie depuis une liste de
coups décrits dans un fichier ; Le projet devra être fourni avec un jeu de
tests sous cette forme-là, associés à une description du résultat attendu.
— gestion de l’historique d’une partie (annulation, rejeu, ...) ;
— sauvegarde et restauration d’une partie vers/depuis un fichier.