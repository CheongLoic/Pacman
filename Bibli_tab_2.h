#ifndef BIBLI_TAB_2_H_INCLUDED
#define BIBLI_TAB_2_H_INCLUDED

#include "Bibli_sauv.h"

void protect_diamond(char mat[22][52],int coord_M[4][2],int i);
void fantomes(char mat[22][52],int coord_M[4][2],int i,int *pt_vie,int couleur_monstre);
void random_ghost(char mat[22][52],int coord_M[4][2]);
void stupid_ghost(char mat[22][52],int coord_M[4][2],int i,int *ghost_direction,int *compteur,int *mode,int *pt_vie,int linear_disp);
void niveau_2(char mat[22][52],char touche,int *poslig,int *poscol,int *points, int *vitesse,int *mode,int *pt_vie,data *donnees,int *trouve,int *index_joueur,int coord_M[4][2]);

#endif // BIBLI_TAB_2_H_INCLUDED
