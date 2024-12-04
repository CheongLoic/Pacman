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

//int coord_M[4][2] est un tableau contenant les coordonnees des 4 fantomes
//i est l'indice de ligne de coord_M contenant respectivement l'abscisse et l'ordonnee d'un fantome


void protect_diamond(char mat[22][52],int coord_M[4][2],int i)
{//protege les diamants apres collision et/ou supprime le monstre de l'ecran
    if (mat[coord_M[i][0]][coord_M[i][1]]=='D') //si emplacement actuel du fantome est sur D
    {
        gotoligcol(coord_M[i][0],coord_M[i][1]); //curseur sur le fantome
        Color(11,0); //met le diamant en cyan
        mat[coord_M[i][0]][coord_M[i][1]]='D';
        printf("%c",mat[coord_M[i][0]][coord_M[i][1]]); // le diamant reste afficher
        Color(15,0); //ecriture text en N/B
    }
    else {
        mat[coord_M[i][0]][coord_M[i][1]]='.'; //supprime position actuelle du fantome
        printf("%c",mat[coord_M[i][0]][coord_M[i][1]]);
    }
}

void fantomes(char mat[22][52],int coord_M[4][2],int i,int *pt_vie,int couleur_monstre)
{ //affiche les fantomes selon la couleur rentree, protege les diamants avant collision et enleve une vie au pacman s'il le touche
    if (mat[coord_M[i][0]][coord_M[i][1]]=='D') //si le fantome est sur le diamant
    {
        gotoligcol(coord_M[i][0],coord_M[i][1]);
        mat[coord_M[i][0]][coord_M[i][1]]='D';
        printf("%c",mat[coord_M[i][0]][coord_M[i][1]]);
    }
    else if (mat[coord_M[i][0]][coord_M[i][1]]=='X') //si le fantomme touche le pacman
    {
        vie(pt_vie); //Pacman perd une vie
        MessageBeep(MB_ICONSTOP); //son
        gotoligcol(coord_M[i][0],coord_M[i][1]); //position du curseur sur nouveau position du fantome
        Color(couleur_monstre,0); //met le fantome en couleur
        mat[coord_M[i][0]][coord_M[i][1]]='M';
        printf("%c",mat[coord_M[i][0]][coord_M[i][1]]);
        Color(15,0); //ecriture text en N/B
	}
    else {
        gotoligcol(coord_M[i][0],coord_M[i][1]); //position du curseur sur nouveau position du fantome
        Color(couleur_monstre,0); //met le fantome en couleur
        mat[coord_M[i][0]][coord_M[i][1]]='M';
        printf("%c",mat[coord_M[i][0]][coord_M[i][1]]);
        Color(15,0); //ecriture text en N/B
    }
}

void random_ghost(char mat[22][52],int coord_M[4][2])
{//proceddure qui place au hasard les fantomes
    int M_lig; //ordonne fantome
    int M_col; //abscisse fantome

    for (int i=0;i<4;i++)	//positionne aleatoirement les 4 fantomes dans mat
    {   do
        {   M_lig =rand()%(21-1)+1; //abscisse du fantome
            M_col =rand()%(51-1)+1; //ordonnee du fantome
        } while (mat[M_lig][M_col]=='D'||mat[M_lig][M_col]=='M'||mat[M_lig][M_col]=='X'||mat[M_lig][M_col]=='#');
        //refait une boucle si le fantome est sur un diamant, un autre fantome, Pacman, ou sur un mur

        //enregistre les coordonnees du fantome
        mat[M_lig][M_col]='M';
        coord_M[i][0]=M_lig;
        coord_M[i][1]=M_col;
    }
}


void stupid_ghost(char mat[22][52],int coord_M[4][2],int i,int *ghost_direction,int *compteur,int *mode,int *pt_vie,int linear_disp)
{ //Le fantome bouge tout seul aleatoirement en ligne droite
//coord_M est un tableau d'entiers contenant les coordonnees des fantomes
//i est l'indice de ligne du tableau coord_M, o¨´ les colonnes 0 et 1 contiennent respectivement
//l'abscisse et l'ordonnee du fantome
//ghost_direction: un pointeur (analogue a la procedure direction_pacman)
//linear_disp est le deplacement lineaire, c'est le nombre de cases que le monstre se deplace en ligne droite avant de changer de direction

    if (*compteur==linear_disp) *ghost_direction=rand()%4;
    //force le fantome a changer de direction ou non au bout d'un deplacement lineaire de linear_disp cases

    gotoligcol(coord_M[i][0],coord_M[i][1]); //curseur sur fantome
    switch(*ghost_direction) //A partir de cette ligne, le reste du prototype
    {                        //ressemble fortement a la procedure direction_pacman
    case 0: //haut
        protect_diamond(mat,coord_M,i);
        coord_M[i][0]-=1;
        if (coord_M[i][0]<1)
        {   if (*mode==0) coord_M[i][0]=20;
            else {
                coord_M[i][0]=1;
                *ghost_direction=2; }
        }
        break;
    case 1: //gauche
        protect_diamond(mat,coord_M,i);
        coord_M[i][1]-=1;
        if (coord_M[i][1]<1)
        {   if (*mode==0) coord_M[i][1]=50;
            else {
                coord_M[i][1]=1;
                *ghost_direction=3; }
        }
        break;
    case 2://bas
        protect_diamond(mat,coord_M,i);
        coord_M[i][0]+=1;
        if (coord_M[i][0]>20)
        {   if (*mode==0) coord_M[i][0]=1;
            else {
                coord_M[i][0]=20;
                *ghost_direction=0; }
        }
        break;
    case 3: //droite
        protect_diamond(mat,coord_M,i);
        coord_M[i][1]+=1;
        if (coord_M[i][1]>50)
        {   if (*mode==0) coord_M[i][1]=1;
            else {
                coord_M[i][1]=50;
                *ghost_direction=1; }
        }
        break;
    }
    wall_collision(mat,coord_M,i,ghost_direction); //incite aux fantomes betes a changer de direction lorsqu'ils rencontrent un mur
                                                    //la procedure est ecrite dans tableau_3.c
    fantomes(mat,coord_M,i,pt_vie,12); // affiche le fantome et protege les diamants
    if (*compteur==linear_disp) *compteur=0; //reinitialise le compteur pour faire un autre deplacement lineaire de linear_disp cases
}


void niveau_2(char mat[22][52],char touche,int *poslig,int *poscol,int *points, int *vitesse,int *mode,int *pt_vie,data *donnees,int *trouve,int *index_joueur,int coord_M[4][2])
{//tableau 2
    int lig=7; //ligne du curseur du menu
    int col=49; //colonne du curseur du menu
    long double compteur=0; //compteur de temps utiliser lorsque la touche p est appuye pour mettre le jeu en pause.
    //voir la fin de cette procedure pour comprendre son utilisation

    gotoligcol(22,0);
    printf("\t\t\tNiveau 2");
    gotoligcol(24,8); //curseur sur le score
    printf("%d", *points); //affiche le score
    gotoligcol(*poslig,*poscol); //curseur sur pacman
    int direction=rand()%4; //direction aleatoire du pacman

    int compteur0=0,compteur1=2,compteur2=4,compteur3=6; //les compteurs ne commencent pas tous a la meme valeur pour donner un aspect plus aleatoire
    //compteur des fantomes pour pouvoir changer de direction

    //direction aleatoire des fantomes
    int ghost0_direction=rand()%4;
    int ghost1_direction=rand()%4;
    int ghost2_direction=rand()%4;
    int ghost3_direction=rand()%4;

    do
    {
        direction_pacman(mat,&direction,poslig,poscol,points,mode,pt_vie); //fait avancer le pacman
        pacman(mat,*poslig,*poscol); //affiche le Pacman

        //deplacement lineaire des fantomes de 10 cases
        stupid_ghost(mat,coord_M,0,&ghost0_direction,&compteur0,mode,pt_vie,10);
        stupid_ghost(mat,coord_M,1,&ghost1_direction,&compteur1,mode,pt_vie,10);
        //deplacement lineaire des fantomes de 5 cases
        stupid_ghost(mat,coord_M,2,&ghost2_direction,&compteur2,mode,pt_vie,5);
        stupid_ghost(mat,coord_M,3,&ghost3_direction,&compteur3,mode,pt_vie,5);
        compteur0+=1;
        compteur1+=1;
        compteur2+=1;
        compteur3+=1;

        gotoligcol(*poslig,*poscol); //remet le curseur sur le Pacman
        Sleep(*vitesse); //arrete le programme de qq ms
        //100 rapide  200 moyen 350 lent
        touche_directionnelle(mat,&touche,poslig,poscol,points,&direction);//pacman change de direction selon la touche

        if (*points==100)
        { //joueur passe au tableau 3
            *pt_vie=5;//reinitialisation des points de vie
            donnees[*index_joueur].tableau=3;  //pre-enregistrement des donnees du joueur
            donnees[*index_joueur].score=100;   //pre-enregistrement des donnees du joueur
            system("cls"); //supprimer l'ecran de la console
            printf("Bravo ! Vous avez gagne\n");
            getch(); //attend que le joueur appuie sur une touche pour continuer la suite
            system("cls"); //supprime l'ecran de la console
            terrain_vide(mat);
            bordure(mat,mode);
            map(mat); //insertion de la map
            diamants(mat);
            afficher(mat); //affiche laire de jeu avec les diamants
            random_pacman(mat,poslig,poscol); //place au hasard le pacman
            random_ghost(mat,coord_M);
            niveau_3(mat,touche,poslig,poscol,points,vitesse,mode,pt_vie,donnees,trouve,index_joueur,coord_M);
        }

        if (*pt_vie==0)
        {//retour au menu
            *pt_vie=5;
            touche=NULL;
            system("cls");
            printf("Vous avez perdu");
            getch(); //attend que le joueur appuie sur une touche pour continuer la suite
            system("cls"); //supprime l'ecran de la console
            menu();
            curseur_menu(touche,&lig,col);
            system("cls");
            option_menu_choisi(touche,&lig,col,vitesse,mode,pt_vie,donnees,trouve,index_joueur);
        }

    } while (touche!='p'&&touche!='q'); //tant que le joueur n'appuie pas sur
                        //la touche 'p' ou 'q', le jeu ne s'arrete pas

    if (touche=='q')
    { //le joueur quitte la partie et retourne au menu
        *pt_vie=5;
        touche=NULL;
        system("cls");
        menu();
        curseur_menu(touche,&lig,col);
        system("cls");
        option_menu_choisi(touche,&lig,col,vitesse,mode,pt_vie,donnees,trouve,index_joueur);
    }

    if (touche=='p') //Le jeu est en mode pause
    {
        touche=NULL;
        do
        {
            compteur+=1; //Rappel : compteur est un long double. On incremente le compteur pour faire occuper le programme en arriere plan
                        //lorsque le jeu est en pause
            Sleep(50); //ralentit le programme pour eviter que le compteur de type long double n'atteigne trop vite sa valeur maximale
            touche=getch();
        }while(touche!='p'); //on incremente le compteur tant que le joueur n'a pas reappuye sur la touche p
        touche=NULL;
        compteur=0; //remet le compteur a zero
        niveau_2(mat,touche,poslig,poscol,points,vitesse,mode,pt_vie,donnees,trouve,index_joueur,coord_M);
    }

}


