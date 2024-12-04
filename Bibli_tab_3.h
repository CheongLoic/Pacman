#ifndef BIBLI_TAB_3_H_INCLUDED
#define BIBLI_TAB_3_H_INCLUDED

#include "Bibli_sauv.h"

typedef struct cellule
{//structure contenant les coordonnees du fantome et la distance entre le pacman
    //et lui selon ses coordonnees
	int X; //abscisse potentielle du fantome
	int Y; //ordonnee potentielle du fantome
	float L; //distance entre Pacman et fantome
	struct cellule *suiv;
} t_cell; //alias de struct cellule

int acceleration(int *points);
void map(char mat[22][52]);
float distance(int *poslig,int *poscol,t_cell *maillon);
void immobile(char mat[22][52],int *poslig,int *poscol,int *direction);
void wall_collision(char mat[22][52],int coord_M[4][2],int i,int *ghost_direction);
void enfile(char mat[22][52],int *poslig,int *poscol,int coord_M[4][2],int i,t_cell *ancre,int x,int y);
void smart_ghost(char mat[22][52],int *poslig,int *poscol,int coord_M[4][2],int i);
void super_ghost(char mat[22][52],int *poslig,int *poscol,int coord_M[4][2],int i,int *mode,int *pt_vie);
void niveau_3(char mat[22][52],char touche,int *poslig,int *poscol,int *points, int *vitesse,int *mode,int *pt_vie,data *donnees,int *trouve,int *index_joueur,int coord_M[4][2]);


#endif // BIBLI_TAB_3_H_INCLUDED
