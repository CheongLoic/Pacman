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

//LE TABLEAU 4 N'EST PAS TERMINE
//ON N'A FAIT QUE L'ALLONGMENT DU SERPENT LORSQU'IL MANGE UN DIAMANT
//IL RESTE A FAIRE :
//LE YUNKUNKUN
//LE SERPENT S'ARRETTE LORSQU'IL RENCONTRE UN MUR
//LE SERPENT PERD UNE PARTIE DE SON CORPS ET UNE VIE S'IL SE TOUCHE
//AFFICHER LES MONTSRES ??


t_serpent *init_snake(int *poslig,int *poscol,t_serpent *ancre)
{//initalise le premier element de la lisste chainee de type struct Serpent
	t_serpent *maillon=(t_serpent*)malloc(sizeof(t_serpent)); //creation 1er maillon
    ancre=maillon; //ancre pointe sur le maillon

	//initialisation des valeurs du 1er maillon
	maillon->new_X = *poscol; //abscisse
	maillon->new_Y = *poslig; //ordonnee
	maillon->last_X = *poscol; //on suppose qu'il n'y aucun obstable a cette position
	maillon->last_Y = *poslig+1; //on suppose qu'il n'y aucun obstable a cette position
	maillon->numero = 1;
	maillon->suiv = NULL; //fin de maillon
	return ancre;
}

void direction_serpent(char mat[22][52],int *direction, int *poslig, int *poscol,int *points,int *mode,int *pt_vie,t_serpent *pt)
{ //change les coordonnes de la tete du serpent selon la direction ou il avance
    //utiliser dans la procedure "deplacement"
    switch(*direction)
    {
    case 0: //haut
        *poslig-=1;//pacman monte d'une ligne
        if (*poslig<1)
        {   if (*mode==0) *poslig=20; //Rappel: mode==0 signifie que la bordure est OFF. Pacman apparait a l'autre bour du terrain
            else
            {   *poslig=2; //pacman fait machine arriere
                *direction=2; //pacman change de direction vers le bas, il rebondit
                vie(pt_vie);    //pacman perd une vie
            }
        }
        break;
    case 1: //gauche
        *poscol-=1;
        if (*poscol<1)
            {   if (*mode==0) *poscol=50;
            else {
            *poscol=2;
            *direction=3;
            vie(pt_vie);    }
        }
        break;
    case 2://bas
        *poslig+=1;
        if (*poslig>20)
        {   if (*mode==0) *poslig=1;
            else {
            *poslig=19;
            *direction=0;
            vie(pt_vie);    }
        }
        break;
    case 3: //droite
        *poscol+=1;
        if (*poscol>50)
        {   if (*mode==0) *poscol=1;
            else {
            *poscol=49;
            *direction=1;
            vie(pt_vie);    }
        }
        break;
    }
    immobile(mat,poslig,poscol,direction); //serpent reste immobile
    if (mat[*poslig][*poscol]=='M') vie(pt_vie); //si pacman touche un fantome il perd une vie

    //affecte les cooronnes de la tete deu serpent
    pt->new_X = *poscol;
    pt->new_Y = *poslig;
}

void pos_queue(char mat[22][52],t_serpent *nouveau)
{//trouve la derniere position queue
    int cas=0;
    int copyX = nouveau->new_X;
    int copyY = nouveau->new_Y;

    do
    {
        nouveau->new_X=copyX;
        nouveau->new_Y=copyY;
        switch(cas)
        {
        case 0:
            nouveau->new_Y-=1;
            cas=1;
            break;
        case 1:
            nouveau->new_X-=1;
            cas=2;
            break;
        case 2:
            nouveau->new_Y+=1;
            cas=3;
            break;
        case 3:
            nouveau->new_X+=1;
            break;
        }
    }while(mat[nouveau->new_Y][nouveau->new_X]=='X'||mat[nouveau->new_Y][nouveau->new_X]=='#'||mat[nouveau->new_Y][nouveau->new_X]=='D');

    nouveau->last_X= nouveau->new_X;
    nouveau->last_Y= nouveau->new_Y;

    nouveau->new_X=copyX;
    nouveau->new_Y=copyY;
}

void deplacement(char mat[22][52],int *poslig,int *poscol,int *points,int *mode,int *pt_vie,int *direction,t_serpent *ancre)
{//le serpent se deplace
    t_serpent *nouveau=(t_serpent*)malloc(sizeof(t_serpent));
    nouveau->suiv=NULL;
    t_serpent *pt=ancre;
    int copieX=0;
    int copieY=0;

    while(pt!=NULL)
    {//premiere boucle pour changer les coordonnees du serpent dans chaque structure
        if (pt->numero==1) //si la structure lu est la tete du serpent
        {   pt->last_X=pt->new_X; //affecte la position actuelle du serpent a sa derniere position en abscisse
            pt->last_Y=pt->new_Y; //affecte la position actuelle du serpent a sa derniere position en ordonnee
            copieX=pt->last_X; //copie de la position de la derniere position de la tete du serpent car on ne peut plus acceder
                                //au structure precedent d'une liste chainee une fois qu'on a change l'adresse de la structure a lire
            copieY=pt->last_Y; //copie de la position de la derniere position de la tete du serpent car on ne peut plus acceder
                                //au structure precedent d'une liste chainee une fois qu'on a change l'adresse de la structure a lire
            direction_serpent(mat,direction,poslig,poscol,points,mode,pt_vie,pt); //change la position actuelle du serpent
            //ON NE PEUT PAS D'ABORD CHANGER LES COORDONNEES DE new_X OU new_Y,
            //SINON ON PERDRA LES COORDONNEES POUR last_X et last_Y
            pt=pt->suiv;
        }
        else
        {   pt->last_X=pt->new_X; //affecte la position actuelle du serpent a sa derniere position en abscisse
            pt->last_Y=pt->new_Y; //affecte la position actuelle du serpent a sa derniere position en ordonnee
            pt->new_X = copieX;
            pt->new_Y = copieY;
            copieX=pt->last_X; //copie de la position de la derniere position de la tete du serpent car on ne peut plus acceder
                                //au structure precedent d'une liste chainee une fois qu'on a change l'adresse de la structure a lire
            copieY=pt->last_Y; //copie de la position de la derniere position de la tete du serpent car on ne peut plus acceder
                                //au structure precedent d'une liste chainee une fois qu'on a change l'adresse de la structure a lire
            pt=pt->suiv;
        }
    }


    pt=ancre; //on va relire a depuis le debut la liste chainee
    if (mat[pt->new_Y][pt->new_X]=='D') //si le sserpent est sur un diamant
    {
        pacman(mat,pt->new_Y,pt->new_X); //affiche la tete du serpent
        while(pt->suiv!=NULL) pt=pt->suiv;

        //ajoute un caractere au queue du snake
        pt->suiv=nouveau; //adressage
        nouveau->numero=pt->numero+1;
        nouveau->new_X=pt->last_X;
        nouveau->new_Y=pt->last_Y;
        pos_queue(mat,nouveau);
        gotoligcol(nouveau->new_Y,nouveau->new_X); //curseur sur la console o¨´ va apparaitre la queue
        mat[nouveau->new_Y][nouveau->new_X]='X'; //enregistrement dans le tableau mat
        printf("%c\a",mat[nouveau->new_Y][nouveau->new_X]); //la queue du serpent s'allonge

        gotoligcol(24,8); //curseur sur le score
        *points+=5;
        printf("%d",*points);

    }
    else
    {
        free(nouveau); //libere la structure nouveau de type t_serpent non uitilisee
        pacman(mat,pt->new_Y,pt->new_X); //affiche la tete du serpent
        while(pt->suiv!=NULL) pt=pt->suiv;
        gotoligcol(pt->last_Y,pt->last_X); //curseur sur l'ancienne position de la queue du serpent
        mat[pt->last_Y][pt->last_X]='.'; //supprime l'ancienne position de la queue du serpent dans le tableau mat
        printf("%c",mat[pt->last_Y][pt->last_X]); //supprime l'ancienne position de la queue du serpent de la console
    }
}


void niveau_4(char mat[22][52],char touche,int *poslig,int *poscol,int *points, int *vitesse,int *mode,int *pt_vie,data *donnees,int *trouve,int *index_joueur,t_serpent *ancre)
{//tableau 3
    int lig=7; //ligne du curseur du menu
    int col=49; //colonne du curseur du menu

    gotoligcol(22,0);
    printf("\t\t\tNiveau 4");
    gotoligcol(24,8); //curseur sur le score
    printf("%d",*points);
    gotoligcol(*poslig,*poscol); //curseur sur pacman
    int direction=rand()%4;

    do
    {
        deplacement(mat,poslig,poscol,points,mode,pt_vie,&direction,ancre);

        touche_directionnelle(mat,&touche,poslig,poscol,points,&direction);
        Sleep(*vitesse); //arrete le programme de qq ms
        //100 rapide  200 moyen 350 lent

        if (*points==175) { //passer au tableau 4
                donnees[*index_joueur].score=175;  //pre-enregistrement des donnees du joueur
                system("cls"); //supprimer l'ecran de la console
                printf("Bravo ! Vous avez gagne\n");
                getch(); //attend que le joueur appuie sur une touche pour continuer la suite
                system("cls"); //supprime l'ecran de la console
                menu();
                curseur_menu(touche,&lig,col);
                system("cls");
                option_menu_choisi(touche,&lig,col,vitesse,mode,pt_vie,donnees,trouve,index_joueur);
                }

        if (*pt_vie==0)
        {
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

    if (touche=='q') {
        int lig=7;
        int col=49;
        *pt_vie=5;
        touche=NULL;
        system("cls");
        menu();
        curseur_menu(touche,&lig,col);
        system("cls");
        option_menu_choisi(touche,&lig,col,vitesse,mode,pt_vie,donnees,trouve,index_joueur);
    }

    long double compteur=0;
    if (touche=='p')
    {
        touche=NULL;
        do
        {
            compteur+=1;
            touche=getch();
        }while(touche!='p');
        touche=NULL;
        compteur=0; //remet le compteur a zero
        niveau_4(mat,touche,poslig,poscol,points,vitesse,mode,pt_vie,donnees,trouve,index_joueur,ancre);
    }
}


