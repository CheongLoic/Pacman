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

void map(char mat[22][52])
{//Creation des murs de la map
    //INUTILIE DE LIRE
    int i=0, j=0;    //initialisation des indices de boucles

    //creation d'un quart des murs de la map en haut a gauche
    for (i=5;i<=8;i++)
    {
        for (j=7;j<=8;j++) mat[i][j]='#'; //remplissage du tableau
    }
    for (i=4;i<=5;i++)
    {
        for (j=21;j<=25;j++) mat[i][j]='#'; //remplissage du tableau
    }
    for (i=3;i<=5;i++)
    {
        for (j=17;j<=18;j++) mat[i][j]='#'; //remplissage du tableau
    }
    for(i=12;i<=22;i++) mat[2][i]='#';
    for(i=8;i<=10;i++) mat[i][11]='#';
    for(i=8;i<=9;i++) mat[i][20]='#';
    for(i=8;i<=10;i++) mat[i][17]='#';
    for(i=3;i<=4;i++) mat[7][i]='#';
    for(i=4;i<=14;i++) mat[4][i]='#';
    for(i=20;i<=24;i++) mat[8][i]='#';
    for(i=1;i<=4;i++) mat[10][i]='#';
    for(i=14;i<=16;i++) mat[10][i]='#';
    for(i=11;i<=14;i++) mat[7][i]='#';

    //replication
    for (i=1;i<11;i++)
    {// copiage symetrique de la map par rapport a l'axe verticale au centre de mat
        for (j=1;j<26;j++) mat[i][51-j]=mat[i][j];
    }
    for (i=1;i<11;i++)
    { //copiage symetrique de la map par rapport a l'axe horizontale au centre de mat
        for (j=1;j<51;j++) mat[21-i][j]=mat[i][j];
    }
}

float distance(int *poslig,int *poscol,t_cell *maillon)
{//calcule la distance L separant le pacman et le fantome
	int vecteur[2]; //coordonnee du vecteur entre Pacman et le fantome
	vecteur[0]=*poslig-maillon->X;	//selon l'abscisse
	vecteur[1]=*poscol-maillon->Y;	//selon l'ordonn¨¦e
	float longueur=sqrt(vecteur[0]*vecteur[0]+vecteur[1]*vecteur[1]);
	return longueur;
}

void immobile(char mat[22][52],int *lig,int *col,int *direction)
{//le pacman ou le fantome revient au position de depart lorsqu'ils rencontrent un mur
    if (mat[*lig][*col]=='#') {
        switch(*direction)
        { //selon la direction remet le pacman au position de depart
        case 0 :
            *lig+=1;
            break;
        case 1 :
            *col+=1;
            break;
        case 2 :
            *lig-=1;
            break;
        case 3 :
            *col-=1;
            break;
        }
    }
}

void wall_collision(char mat[22][52],int coord_M[4][2],int i,int *ghost_direction)
{//incite aux fantomes betes a changer de direction lorsqu'ils rencontrent un mur
    //DECLARATION VARIABLE
    int sens=*ghost_direction; //affectation de la direction du fantome bete a sens

    while(mat[coord_M[i][0]][coord_M[i][1]]=='#')
    {//boucle qui change les coordonnees du fantome tant que ses coordonnees sont sur un mur
        immobile(mat,&coord_M[i][0],&coord_M[i][1],ghost_direction);//le fantome revient au position de depart
        while (*ghost_direction==sens) *ghost_direction=rand()%4;//fantome change de direction
        //evite que le fantome continue dans la meme direction
        sens=*ghost_direction; //reaffectation de la direction du fantome a sens si le programme fait plus d'un tour de boucle
        switch(*ghost_direction)
        { //selon les direction du fantomes, on change ses coordonnees
        case 0 : //direction du haut
            coord_M[i][0]-=1; //fantome monte d'une case
            break;
        case 1 : //direction de gauche
            coord_M[i][1]-=1; //fantome se deplace d'une case a gauche
            break;
        case 2 : //direction du bas
            coord_M[i][0]+=1; //fantome descend d'une case en bas
            break;
        case 3 : //direction de droite
            coord_M[i][1]+=1; //fantome se deplace d'une case a droite
            break;
        }
    }
}

void enfile(char mat[22][52],int *poslig,int *poscol,int coord_M[4][2],int i,t_cell *ancre,int x,int y)
{ //cette procedure utilise les notions de liste chainee en file
//cree une nouvelle cellule et le met a la fin de la liste chainee
//x et y seront des entiers compris entre -1 et 1 pour decaler d'une case par rapport a la position de depart du fantome
    t_cell *nouveau=(t_cell*)malloc(sizeof(t_cell)); //adressage de la cellule nouveau

	nouveau->X = coord_M[i][0]+x;
	nouveau->Y = coord_M[i][1]+y;
	if (mat[nouveau->X][nouveau->Y]=='#'||mat[nouveau->X][nouveau->Y]==' ') free(nouveau);
	//si les coordonnees sont ceux du mur ou de l'exterieur du terrain, efface la cellule nouveau
	else {
		nouveau->L = distance(poslig,poscol,nouveau);
		nouveau->suiv=NULL;
		t_cell *actuel=ancre;
		while (actuel->suiv!=NULL) actuel=actuel->suiv;
		actuel->suiv=nouveau;
		actuel=ancre;
		ancre=actuel;
	}
}

void smart_ghost(char mat[22][52],int *poslig,int *poscol,int coord_M[4][2],int i)
{//cette procedure est utiliee dans la procedure stupid_ghost dans tableau_2.c
    //les coordonnees des fantomes sont changes de sorte qu'ils se rapprochent du pacman
    //printf("pacman %d %d\n",poslig,poscol);
    //printf("monstre %d %d\n",coord_M[i][0],coord_M[i][1]);
    t_cell *ancre;
	t_cell *maillon=(t_cell*)malloc(sizeof(t_cell)); //creation 1er maillon
    ancre=maillon; //ancre pointe sur le maillon

	//initialisation des valeurs du 1er maillon
	maillon->X = coord_M[i][0]; //abscisse
	maillon->Y = coord_M[i][1]-1; //ordonnee
	maillon->L = distance(poslig,poscol,maillon);
	maillon->suiv = NULL; //fin de maillon


	enfile(mat,poslig,poscol,coord_M,i,ancre,1,0); //case de droite
	enfile(mat,poslig,poscol,coord_M,i,ancre,0,1); //case du bas
	enfile(mat,poslig,poscol,coord_M,i,ancre,-1,0); //case de gauche

	t_cell *copie=ancre; //copie de la liste chainee;
	if (mat[copie->X][copie->Y]=='#') ancre=copie->suiv;

    t_cell *pt=ancre;

    float min=pt->L;
	coord_M[i][0]=pt->X;
	coord_M[i][1]=pt->Y;
    while (pt!=NULL)
     {
        //printf("X=%d\t", pt->X);
        //printf("Y=%d\t", pt->Y);
        //printf("L=%f\n", pt->L);
        pt= pt->suiv;
        if (pt!=NULL)
		{
			if (min>pt->L)
			{	min=pt->L; //reaffectation de la valeur min
				coord_M[i][0]=pt->X; //reaffectation des coordonnees
				coord_M[i][1]=pt->Y;
			}
		}
    }
    free(ancre);
    //printf("monstre %d %d %f\n",coord_M[i][0],coord_M[i][1],min);
}

void super_ghost(char mat[22][52],int *poslig,int *poscol,int coord_M[4][2],int i,int *mode,int *pt_vie)
{//affichage du fantome intelligent
    gotoligcol(coord_M[i][0],coord_M[i][1]); //curseur sur fantome
    protect_diamond(mat,coord_M,i);
    smart_ghost(mat,poslig,poscol,coord_M,i);
    fantomes(mat,coord_M,i,pt_vie,13); // affiche le fantome et protege les diamants
}

int acceleration(int *points)
{//selon les points acquis au tableau 3, retourne le nombre d'iteration pour faire avancer le pacman "iteration" fois
    int iteration=1;
    switch(*points)
    {
    case 110:
        iteration=2;
        break;
    case 120:
        iteration=3;
        break;
    case 130:
        iteration=4;
        break;
    case 140:
        iteration=5;
        break;
    }
    return iteration;
}

void niveau_3(char mat[22][52],char touche,int *poslig,int *poscol,int *points, int *vitesse,int *mode,int *pt_vie,data *donnees,int *trouve,int *index_joueur,int coord_M[4][2])
{//tableau 3
    int lig=7; //ligne du curseur du menu
    int col=49; //colonne du curseur du menu
    long double compteur=0;  //compteur de temps utiliser lorsque la touche p est appuye pour mettre le jeu en pause.
    //voir la fin de cette procedure pour comprendre son utilisation

    gotoligcol(22,0);
    printf("\t\t\tNiveau 3");
    gotoligcol(24,8); //curseur sur le score
    printf("%d",*points);
    gotoligcol(*poslig,*poscol); //curseur sur pacman

    int direction=rand()%4;
    int compteur0=0,compteur1=2;
    int ghost0_direction=rand()%4;
    int ghost1_direction=rand()%4;
    int ralenti=2; //un genre de compteur pour ralentir les fantomes betes
    int n=0; //variable du second boucle do while
    int iteration=1; //variable du second boucle do while
    do
    {
        do
        { //le pacman avance iteration fois
            direction_pacman(mat,&direction,poslig,poscol,points,mode,pt_vie); //fait avancer le pacman
            pacman(mat,*poslig,*poscol); //affiche le Pacman
            n+=1;
            Sleep(*vitesse); //arrete le programme de qq ms
            //100 rapide  200 moyen 350 lent
            touche_directionnelle(mat,&touche,poslig,poscol,points,&direction); //pacman change de direction selon la touche
            iteration=acceleration(points);//affecte le nombre d'iteration a faire selon les points cumules
        } while (n<iteration);
        n=0;//reinitialisation

        //deplacement des fantomes intelligents
        super_ghost(mat,poslig,poscol,coord_M,0,mode,pt_vie);
        super_ghost(mat,poslig,poscol,coord_M,1,mode,pt_vie);

        if (ralenti==2)
        { //ralentit les fantomes betes
            //les fantomes betes ne bougent que si le compteur "ralenti" est egale a 2
            stupid_ghost(mat,coord_M,2,&ghost0_direction,&compteur0,mode,pt_vie,15);
            stupid_ghost(mat,coord_M,3,&ghost1_direction,&compteur1,mode,pt_vie,15);
            compteur0+=1;
            compteur1+=1;
            ralenti=0;
        }
        ralenti+=1;
        gotoligcol(*poslig,*poscol); //remet le curseur sur le Pacman

        if (*points==100) Sleep(50); //ralentit le programme pour que les fantomes se deplacent un peu moins vite

        if (*points==150)
        { //passer au tableau 4
                donnees[*index_joueur].tableau=4;  //pre-enregistrement des donnees du joueur
                donnees[*index_joueur].score=150;  //pre-enregistrement des donnees du joueur
                system("cls"); //supprimer l'ecran de la console
                printf("Bravo ! Vous avez gagne\n");
                getch(); //attend que le joueur appuie sur une touche pour continuer la suite
                system("cls"); //supprimer l'ecran de la console
                terrain_vide(mat);
                bordure(mat,&mode);
                diamants(mat);
                afficher(mat);
                random_pacman(mat,poslig,poscol); //place au hasard le pacman
                t_serpent *ancre=init_snake(poslig,poscol,ancre); //initialise la position du snake
                niveau_4(mat,touche,poslig,poscol,points,vitesse,mode,pt_vie,donnees,trouve,index_joueur,ancre);
        }

        if (*pt_vie==0)
        { //retour au menu
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
        niveau_3(mat,touche,poslig,poscol,points,vitesse,mode,pt_vie,donnees,trouve,index_joueur,coord_M);
    }
}






