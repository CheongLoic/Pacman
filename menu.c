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

char *name()
{//procedure qui saisit le pseudo du joueur
    char pseudo[16]; //pseudo de 15 caracteres
    char *dynamique=NULL;
    printf("Saisir votre pseudo (15 caracteres max)\t");
    gets(pseudo);
    dynamique=(char*)malloc(sizeof(char)*(strlen(pseudo)+1)); //on fait allocation dynamique pour que le tableau "dynamique"
                                                            //ait le meme nombre de cases que le nombre de caracteres de pseudo
    strcpy(dynamique,pseudo); //copie la chaine pseudo dans dynamique
    return dynamique; //retourne le pointeur dynamique
}

int option_vitesse(int *vitesse,int col)
{// fonction qui renvoie la vitesse choisie par le joueur
    //l'entier col est la position du curseur en abscisse sur la console
    if (col==6) *vitesse=350;
    else if (col==24) *vitesse=200;
    else if (col==40) *vitesse=100;
    return (*vitesse); //renvoie la vitesse
}

int option_bordure(int mode,int col) //
{//fonction renvoyant 0 ou 1 pour le mode d'activation de la bordure
    if (col==2) mode=1; //bordure ON
    else mode=0; //bordure OFF
    return (mode); //retourne la variable
}


void menu()
{//Simple affichage du menu avec les options
    printf("\t\t\t\t  PACMAN\n\n");
    printf("Ce jeu a ete cree par CHEONG Loic\n\n\n\n\n");

    printf("Effacer sauvegarde et recommencer toute la partie\n");
    printf("Reprendre partie enregistree\n");
    printf("Vitesse\n");
    printf("Bordure de carte\n");
    printf("Regles du jeu\n");
    printf("Quitter et sauvegarder la partie du jeu\n\n\n");

    printf("Pour deplacer le curseur, taper les touches suivantes sur le clavier : \n");
    printf("- 's' pour se deplacer vers la gauche \n");
    printf("- 'd' pour se deplacer vers la bas\n");
    printf("- 'f' pour se deplacer vers la droite\n");
    printf("- 'e' pour se deplacer vers le haut\n");
}

void curseur_menu(char touche,int *lig, int col)
{//cette procedure deplace le curseur du menu pour que le joueur choisisse une option
    do
    {
        gotoligcol(*lig,col); //position du curseur
        if (kbhit())
        {
            touche=getch();
            switch(touche)
            {
            case 'e' : //curseur vers le haut
                *lig-=1;
                if (*lig<=7){ //position du curseur sur "Effacer sauvegarde et recommencer toute la partie"
                    *lig=7;
                    col=49;
                }
                else if (*lig==8) col=28; //position du curseur sur "Reprendre partie enregistree"
                else if (*lig==9) col=7; //position du curseur sur "Vitesse"
                else if (*lig==10) col=16; //position du curseur sur "Bordure de carte"
                else if (*lig==11) col=13; //position du curseur sur "Regles du jeu"
                break;
            case 'd' : //curseur vers le bas
                *lig+=1;
                if (*lig>=12) { //position du curseur sur "Quitter et sauvegarder la partie du jeu"
                    *lig=12;
                    col=39;
                }
                else if (*lig==8) col=28; //position du curseur sur "Reprendre partie enregistree"
                else if (*lig==9) col=7; //position du curseur sur "Vitesse"
                else if (*lig==10) col=16; //position du curseur sur "Bordure de carte"
                else if (*lig==11) col=13; //position du curseur sur "Regles du jeu"
                break;
            }
        }
    } while (touche!=' '); //attente de confirmation tant que le joueur
                        // n'appuie pas sur la barre d'espace
}


void option_menu_choisi(char touche,int *lig,int col,int *vitesse,int *mode,int *pt_vie,data *donnees,int *trouve,int *index_joueur)
{ //Une fois l'option du menu choisi, redirige le joueur vers une autre page ou il y a les contenus de l'option

    //declararion des variables pour le case 21 :Recommencer partie
    char mat[22][52];
    int coord_M[4][2];
    int points=0;
    int poslig;
    int poscol;
    long double compteur=0; //compteur de temps
    *pt_vie=5; //reinitilise les points de vie si le joueur quitte une partie

    switch(*lig)
    {//la valeur des lig dans les case sont les numeros de ligne des options du menu

    case 7: //option : Effacer sauvegarde et recommencer toute la partie
        //reinitialise le tableau et le score
        donnees[*index_joueur].tableau=1;
        donnees[*index_joueur].score=0;

        system("cls"); //supprime l'ecran de la console

        //les procedures sont dans tableau 1.c
        terrain_vide(mat); //remplit le tableau par des points
        bordure(mat,mode); //affiche les bordures selon le mode d'activation
        diamants(mat); //place les diamants
        afficher(mat); //affiche l'aire de jeu avec les diamants
        random_pacman(mat,&poslig,&poscol); //place au hasard le pacman
        niveau_1(mat,touche,&poslig,&poscol,&points,vitesse,mode,pt_vie,donnees,trouve,index_joueur); //le joueur commence le tableau 1
        break;
    case 8 ://option : Reprendre partie enregistree
        switch(donnees[*index_joueur].tableau)
        { //redirige le joueur vers un tableau de niveau selon la valeur enregistree
            case 1: //Si niveau 1
                system("cls"); //efface tout l'ecran de la console
                terrain_vide(mat); //remplit le tableau par des points
                bordure(mat,mode);//affiche les bordures selon le mode d'activation
                diamants(mat);  //place les diamants
                afficher(mat); //affiche l'aire de jeu avec les diamants
                random_pacman(mat,&poslig,&poscol); //place au hasard le pacman
                niveau_1(mat,touche,&poslig,&poscol,&points,vitesse,mode,pt_vie,donnees,trouve,index_joueur); //le joueur commence le tableau 1
                 //la procedure niveau_1 est ecrit dans tableau 1.c
                break;
            case 2: //si niveau 2
                points=50;
                system("cls"); //efface tout l'ecran de la console
                terrain_vide(mat); //remplit le tableau par des points
                bordure(mat,mode);//affiche les bordures selon le mode d'activation
                diamants(mat);  //place les diamants
                afficher(mat); //affiche l'aire de jeu avec les diamants
                random_pacman(mat,&poslig,&poscol); //place au hasard le pacman
                random_ghost(mat,coord_M); //place les fantomes au hasard
                niveau_2(mat,touche,&poslig,&poscol,&points,vitesse,mode,pt_vie,donnees,trouve,index_joueur,coord_M); //le joueur commence le tableau 2
                 //la procedure niveau_2 est ecrit dans tableau_2.c
                break;
            case 3: //si niveau 3
                points=100;
                system("cls"); //efface tout l'ecran de la console
                terrain_vide(mat); //remplit le tableau par des points
                bordure(mat,mode);//affiche les bordures selon le mode d'activation
                map(mat); //met les murs/obstacles
                diamants(mat);  //place les diamants
                afficher(mat); //affiche l'aire de jeu avec les diamants
                random_pacman(mat,&poslig,&poscol); //place au hasard le pacman
                random_ghost(mat,coord_M); //place les fantomes au hasard
                niveau_3(mat,touche,&poslig,&poscol,&points,vitesse,mode,pt_vie,donnees,trouve,index_joueur,coord_M);//le joueur commence le tableau 3
                 //la procedure niveau_3 est ecrit dans tableau_3.c
                break;
            case 4:
                points=150;
                system("cls"); //efface tout l'ecran de la console
                terrain_vide(mat); //remplit le tableau par des points
                bordure(mat,mode);//affiche les bordures selon le mode d'activation
                diamants(mat);  //place les diamants
                afficher(mat); //affiche l'aire de jeu avec les diamants
                random_pacman(mat,&poslig,&poscol); //place au hasard le serpent
                t_serpent *ancre=init_snake(&poslig,&poscol,ancre);//initialise la position du snake
                                                                    //la structure t_serpent est definie dans "Bibli_tab_4.h"
                                                                    //la fonction init_snake(...) est ecrit dans tableau_4.c
                niveau_4(mat,touche,&poslig,&poscol,&points,vitesse,mode,pt_vie,donnees,trouve,index_joueur,ancre);
                //la procedure niveau_4 est ecrit dans tableau_4.c
                break;
        }
    case 9: //option : Vitesse
        printf("Veuillez choisir votre vitesse de deplacement :\n");
        printf("1-Lent\t\t2-Normal\t3-Rapide\n");
        col=6;
        do
        {
            gotoligcol(*lig,col); //positionne le curseur
            *lig=1;
            if (kbhit())
            {
                touche=getch();
                switch(touche)
                {
                case 's' : //curseur vers la gauche
                    col-=1;
                    if (col<=23)col=6;
                    else col=24; //position du curseur sur
                    break;
                case 'f' : //deplace curseur vers la droite
                    col+=1;
                    if (col>=25) col=40;
                    else col=24;
                    break;
                }
            }
        } while (touche!=' '); //attente de confirmation tant que le joueur
                        // n'appuie pas sur la barre d'espace
        system("cls");
        *vitesse=option_vitesse(vitesse,col);
        *lig=7;
        col=49;
        *pt_vie=5;
        touche=NULL;
        menu();
        curseur_menu(touche,lig,col);
        system("cls");
        option_menu_choisi(touche,lig,col,vitesse,mode,pt_vie,donnees,trouve,index_joueur);
        break;

    case 10: //option : Bordure
        printf("Activation/Desactivation de la bordure\n");
        printf("En mode ON, si votre Pacman touche une bordure, il perd une vie\n");
        printf("En mode OFF, si votre Pacman sort du terrain, il reapparaitra au cote oppose\n");
        printf("Choix du mode de jeu :\n");
        printf("ON\tOFF");
        *lig=4;
        col=2;
        do
        {
        gotoligcol(*lig,col); //positionne le curseur sur la console
            if (kbhit())
            {
                touche=getch();
                switch(touche)
                {
                case 's' : //curseur vers la gauche
                    col=2;
                    break;
                case 'f' : //curseur vers la droite
                    col=11;
                    break;
                }
            }
        } while (touche!=' '); //attente de confirmation tant que le joueur
                        // n'appuie pas sur la barre d'espace

        //RETOUR AU MENU SI JOUEUR APPUIE SUR LA BARRE ESPACE
        *mode=option_bordure(mode,col);
        system("cls");
        //*vitesse=option_vitesse(vitesse,col);
        *lig=7;
        col=49;
        *pt_vie=5;
        touche=NULL;
        menu();
        curseur_menu(touche,lig,col);
        system("cls");
        option_menu_choisi(touche,lig,col,vitesse,mode,pt_vie,donnees,trouve,index_joueur);
        break;

    case 11: //option : Regles du jeu
        //affiche le regles
        printf("\t\t\t========REGLES DU JEU========\n");
        printf("Vous etes Pacman, symbolise par un X. Mangez le plus vite possible tous les\n");
        printf("diamants D sur le terrain sans vous faire devorer a votre tour par les monstres M.\n");
        printf("Accedez a differents niveaux lorsque vous avez finis de manger les diamants.\n");
        printf("Faites votre meilleur score ! Bon jeu.\n");
        printf("\t\t\t=============================\n\n");

        printf("Touches du clavier:\n\t\t\t\t8. Haut\n\t\t4. Gauche\t2. Bas\t\t6. Droite\n");
        printf("q. Retourner au menu\n");
        printf("p. Pause\n");
        printf("Barre d'espace = Confirmer mon choix\n\n");

        touche=NULL;//precotion
        do
        {
            compteur+=1;//Rappel : compteur est un long double. On incremente le compteur pour faire occuper le programme en arriere plan
            Sleep(50); //ralentit le programme pour eviter que le compteur de type long double n'atteigne trop vite sa valeur maximale
            touche=getch();
        }while(touche!=' ');

        //RETOUR AU MENU SI JOUEUR APPUIE SUR LA BARRE ESPACE
        system("cls");
        *lig=7;
        col=49;
        *pt_vie=5;
        touche=NULL;
        menu();
        curseur_menu(touche,lig,col);
        system("cls");
        option_menu_choisi(touche,lig,col,vitesse,mode,pt_vie,donnees,trouve,index_joueur);
        break;

    case 12: //option : quitter et sauvegarder
        printf("Votre partie a ete sauvegarde\nVous vous appretez a quitter le jeu\nMerci d'avoir joue !\n");
        sauvegarder(donnees,trouve); //sauvegarde les donnees du joueur dans un fichier
        //la procedure de sauvegarde est ecrit dans le code source sauv.c
        exit(0);//force le programme a s'arreter
        break;
    }
}







