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

void Color(int couleurDuTexte,int couleurDeFond) // fonction d'affichage de couleurs
{//affiche le texte en couleur et le surligne selon la meme ou une autre couleur
// Trouver sur https://www.developpez.net/forums/d309614/c-cpp/c/couleurs-c/
//0=Noir  9=Bleu  10=Vert 11=Cyan 12=Rouge  14=Jaune  15=Blanc
        HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(H,couleurDeFond*16+couleurDuTexte);
}

void gotoligcol( int lig, int col )
{//positionne le curseur sur la console
    COORD mycoord;
    mycoord.X = col;
    mycoord.Y = lig;
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), mycoord );
}

void terrain_vide(char mat[22][52])
{//remplit le tableau de caracteres mat par des points '.'
    for (int i=1;i<21;i++)
    {
        for (int j=1;j<51;j++) mat[i][j]='.'; //remplissage du tableau
    }
}

void bordure(char mat[22][52], int *mode)
{//les bordures sont activees ou non selon le mode d'activation
  //Que les bordures sont actives ou non, le pacman aura toujours la meme surface de jeu
    if (*mode==0) //bordure OFF
    {   for (int j=0;j<52;j++)  {
            mat[0][j]=' ';
            mat[21][j]=' ';     }
        for (int i=0;i<22;i++)
        {   mat[i][0]=' ';
            mat[i][51]=' ';
        }
    }
    else if (*mode==1) //bordure ON
    { //les bordures sont des #
        for (int j=0;j<52;j++)
        {   mat[0][j]='#';
            mat[21][j]='#';
        }
        for (int i=0;i<22;i++)
        {   mat[i][0]='#';
            mat[i][51]='#';
        }
    }
}

void diamants(char mat[22][52])
{//cette procedure place 5 diamants au hasard dans le tableau mat
    int lig;
    int col;
    for (int i=0;i<5;i++)
    {
        do { //affecte une position aleatoire aux diamants tant qu'ils sont sur un diamant deja place, ou sur un mur
        lig =rand()%(21-1)+1;
        col =rand()%(51-1)+1;
        } while (mat[lig][col]=='D'||mat[lig][col]=='#'); //recommence la boucle si le diamant est sur un autre diamant ou sur un mur
        mat[lig][col]='D'; //enregistre l'emplacement du diamant dans mat
    }
}

void afficher(char mat[22][52])
{//cette procedure affiche l'aire de jeu du Pacman avec diamants, murs, bordures
    for (int i=0;i<22;i++)
    {
        for (int j=0;j<52;j++) {
            if (mat[i][j]=='D') {
                Color(11,0); //diamants mis en couleur Cyan
                printf("%c",mat[i][j]);
                Color(15,0);} //ecriture texte en N/B
            else printf("%c",mat[i][j]); //affichage du reste du tableau
        }
        printf("\n"); // retour a la ligne apres ecriture complet d'une ligne du tableau
    }
    printf("\nVie : 5"); //écrit à la ligne 23 de la console
    printf("\nScore :"); //écrit à la ligne 24 de la console
}

void random_pacman(char mat[22][52],int *poslig,int *poscol)
{ //positionne au hasard le Pacman dans l'aire de jeu
    do
    {
        *poslig = rand()%(21-1)+1; //poslig €[1;21[
        *poscol = rand()%(51-1)+1; //poscol €[1;51[
    } while (mat[*poslig][*poscol]=='D'||mat[*poslig][*poscol]=='#'); //refait une boucle si le pacmann et sur un diamant ou sur un mur
    pacman(mat,*poslig,*poscol); //affiche le pacman sur la console
}

void pacman(char mat[22][52],int poslig,int poscol)
{//cette procedure affecte la position du Pacman dans le tableau mat et l'affiche sur l'aire de jeu
// a la ligne poslig et colonne poscol
    gotoligcol(poslig,poscol); //positionne le curseur
    Color(14,0); //Pacman jaune
    mat[poslig][poscol]='X';
    printf("%c",mat[poslig][poscol]); //remplace la position du curseur par X, et le curseur avance
                                    // il se decale d'une colonne
    Color(15,0); //texte ecriture en N/B
    gotoligcol(poslig,poscol); //on repositionne le curseur sur le pacman
}

void score(char mat[22][52],int *poslig,int *poscol,int *points)
{ //le score du pacman augmente s'il mange un diamant.
    if (mat[*poslig][*poscol]=='D') //si pacman est sur le diamant
    {
        *points+=10; //points augmente de 10
        gotoligcol(24,8); //positionne le curseur sur le score ecrit sur la console
        printf("%d\a", *points); //affiche le score avec un bip
        pacman(mat,*poslig,*poscol); //affiche le pacman
    }
}

void vie(int* pt_vie)
{//enleve une vie au Pacman
    *pt_vie-=1;
    gotoligcol(23,6);
    printf("%d",*pt_vie); //affiche le nombre de vies
    MessageBeep(MB_ICONSTOP); //emission d'un son
}

void direction_pacman(char mat[22][52],int *direction, int *poslig, int *poscol,int *points,int *mode,int *pt_vie)
{ //selon la direction du pacman, il avance d'une case et perd une vie ou augmente son score selon la case
    switch(*direction)
        {
        case 0: //haut
            mat[*poslig][*poscol]='.'; //ancienne position du pacman remplacee par un point
            printf("%c",mat[*poslig][*poscol]); //affichage du point sur l'ancienne position
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
            mat[*poslig][*poscol]='.';
            printf("%c",mat[*poslig][*poscol]);
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
            mat[*poslig][*poscol]='.';
            printf("%c",mat[*poslig][*poscol]);
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
            mat[*poslig][*poscol]='.';
            printf("%c",mat[*poslig][*poscol]);
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
    immobile(mat,poslig,poscol,direction); //si le pacman renconre un mur, il reste immobile
    score(mat,poslig,poscol,points); //augmente et affiche le score
    if (mat[*poslig][*poscol]=='M') {
        vie(pt_vie); //si pacman touche un fantome il perd une vie
    }
}


void touche_directionnelle(char mat[22][52],char *touche,int *poslig,int *poscol,int *points,int *direction)
{//selon la touche appuyee par le joueur, le Pacman va dans une direction donnee
     if (kbhit())
        {
            *touche=getch(); //en appuyant sur une touche, la variable est affectee à une valeur
                            //sans appuyer sur la touche Entree
            switch(*touche) //Selon les touches appuyees, le pacman va se deplacer ou non
            {
            case 'd' : // Le Pacman se deplace vers le bas d'une case
                mat[*poslig][*poscol]='.';
                printf("%c",mat[*poslig][*poscol]);
                *direction=2;
                if (*poslig>20)
                {
                    *poslig=1;
                    score(mat,poslig,poscol,points);
                    pacman(mat,*poslig,*poscol);
                }
                else pacman(mat,*poslig,*poscol);
                break;
            case 's' : // Le Pacman se deplace vers la gauche d'une case
                mat[*poslig][*poscol]='.';
                printf("%c",mat[*poslig][*poscol]);
                *direction=1;
                if (*poscol<1)
                {
                    *poscol=50;
                    score(mat,poslig,poscol,points);
                    pacman(mat,*poslig,*poscol);
                }
                else pacman(mat,*poslig,*poscol);
                break;
            case 'f' : // Le Pacman se deplace vers la droite d'une case
                mat[*poslig][*poscol]='.';
                printf("%c",mat[*poslig][*poscol]);
                *direction=3;
                if (*poscol>50)
                {
                    *poscol=1;
                    score(mat,poslig,poscol,points);
                    pacman(mat,*poslig,*poscol);
                }
                else pacman(mat,*poslig,*poscol);
                break;
            case 'e' : // Le Pacman se deplace vers le haut d'une case
                //Sleep(500);
                mat[*poslig][*poscol]='.'; //position actuelle du pacman transformee
                printf("%c",mat[*poslig][*poscol]);
                *direction=0;
                if (*poslig<1)
                {   //effet de bord : si le pacman tente d'avancer
                    //il sort au cote oppose
                    *poslig=20;
                    score(mat,poslig,poscol,points);
                    pacman(mat,*poslig,*poscol);
                }
                else pacman(mat,*poslig,*poscol);
                break; //arret du cas
            }
        }
}


void niveau_1(char mat[22][52],char touche,int *poslig,int *poscol,int *points,int *vitesse,int *mode,int *pt_vie,data *donnees,int* trouve,int *index_joueur)
{ //tableau 1
    int lig=7; //ligne du curseur du menu
    int col=49; //colonne du curseur du menu
    int coord_M[4][2]; //tableau contenant les coordonnees des 4 fantomes
    long double compteur=0; //compteur de temps utiliser lorsque la touche p est appuye pour mettre le jeu en pause.
    //voir la fin de cette procedure pour comprendre son utilisation

    gotoligcol(22,0);
    printf("\t\t\tNiveau 1");
    gotoligcol(24,8); //curseur sur le score
    printf("%d", *points);
    gotoligcol(*poslig,*poscol); //curseur sur pacman
    int direction=rand()%4; //direction vaut entre 0 et 3

    do
    {
        direction_pacman(mat,&direction,poslig,poscol,points,mode,pt_vie); //fait avancer le pacman
        pacman(mat,*poslig,*poscol); //place le Pacman
        Sleep(*vitesse); //arrete le programme de qq ms
        //100 rapide  200 moyen 350 lent
        touche_directionnelle(mat,&touche,poslig,poscol,points,&direction); //pacman change de direction selon la touche

        if (*points==50)
        { //passer au tableau 2
            *pt_vie=5;
            donnees[*index_joueur].tableau=2; //pre-enregistrement des donnees du joueur
            donnees[*index_joueur].score=50;  //pre-enregistrement des donnees du joueur
            system("cls"); //supprimer l'ecran de la console
            printf("Bravo ! Vous avez gagne\n");
            getch(); //attend que le joueur appuie sur une touche pour continuer la suite
            system("cls"); //supprime l'ecran de la console
            terrain_vide(mat);
            bordure(mat,mode);
            diamants(mat);
            afficher(mat); //affiche laire de jeu avec les diamants
            random_pacman(mat,poslig,poscol); //place au hasard le pacman
            random_ghost(mat,coord_M);
            niveau_2(mat,touche,poslig,poscol,points,vitesse,mode,pt_vie,donnees,trouve,index_joueur,coord_M);
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

    if (touche=='q') //le joueur quitte la partie et retourne au menu
    {
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
        niveau_1(mat,touche,poslig,poscol,points,vitesse,mode,pt_vie,donnees,trouve,index_joueur);
    }
}




