#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <math.h>
#include "Bibli_menu.h"
#include "Bibli_sauv.h"
#include "Bibli_tab_1.h"
#include "Bibli_tab_2.h"
#include "Bibli_tab_3.h"
#include "Bibli_tab_4.h"

int main()
{   Color(15,0);//ecrit au debut l'affichage sur la console en N/B

    //A lire les codes sources dans l'ordre :
    //main, sauv.c, menu.c, tableau 1.c, tableau_2.c, tableau_3.c, tableau_4.c

    srand(time(NULL));

    //DECLARATION DES VARIABLES
    char *pseudo=NULL;
    data *donnees; //pointeur de structure contenant le nom, le dernier tableau joue et le score du joueur
    //data est une structure definie dans la bibliotheque sauv.h
    int index_joueur=0;//index du tableau de structure ou les informations du joueur seront exploitees
    int trouve=0; //cette variable vaut 0 lorsqu'on a un nouveau joueur, sinon vaut 1 lorsque le joueur a deja joue

    char touche;
    int vitesse=200; //vitesse normal par defaut au debut si le joueur ne selectionne pas son choix de vitesse
    int mode=0; //mode de bordure OFF par defaut
    int lig=7; //position du curseur de la console
    int col=49; //position du curseur de la console
    int pt_vie=5; //points de vie


    pseudo=name(); //demande au joueur de rentrer son pseudo qui sera affecte au pointeur
                    //la fonction name() est dans menu.c
    donnees=lecture_fichier(); //lecture du fichier contenant les donnees des joueurs,et on affecte l'adresse d'un tableau de structure data a donnees
                            //la procedure  lecture_fichier() est dans sauv.c
    recherche_joueur(donnees,pseudo,&index_joueur,&trouve); //procedure qui cherche le pseudo du joueur dans le tableau de structure donnees
                                                            // pour les exploiter sinon les  cree
                                                            //la procedure recherche_joueur(...) est dans sauv.c
    getch();//attend que le joueur appuie sur une touche pour passer a la page suivante


    system("cls");//efface tout l'ecran de la console
    menu(); //affiche le menu
    curseur_menu(touche,&lig,col); //deplace le curseur du menu
                                    //la fonction est dans menu.c
    system("cls");//supprime l'ecran de le console
    option_menu_choisi(touche,&lig,col,&vitesse,&mode,&pt_vie,donnees,&trouve,&index_joueur); //selon l'option du menu chois, redirige le joueur vers une autre page
    //la procedure est dans menu.c

}
