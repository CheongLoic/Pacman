#ifndef BIBLI_TAB_1_H_INCLUDED
#define BIBLI_TAB_1_H_INCLUDED

#include "Bibli_sauv.h" // on l'inclut pcq'on a un probleme avec la reconnaissance de la structure data

void Color(int couleurDuTexte,int couleurDeFond);
void gotoligcol( int lig, int col );
void terrain_vide(char mat[22][52]);
void bordure(char mat[22][52], int *mode);
void diamants(char mat[22][52]);
void afficher(char mat[22][52]);
void random_pacman(char mat[22][52],int *poslig,int *poscol);
void pacman(char mat[22][52],int poslig,int poscol);
void score(char mat[22][52],int *poslig,int *poscol,int *points);
void vie(int* pt_vie);
void direction_pacman(char mat[22][52],int *direction, int *poslig, int *poscol,int *points,int *mode,int *pt_vie);
void touche_directionnelle(char mat[22][52],char *touche,int *poslig,int *poscol,int *points,int *direction);
void niveau_1(char mat[22][52],char touche,int *poslig,int *poscol,int *points,int *vitesse,int *mode,int *pt_vie,data *donnees,int *trouve,int *index_joueur);

#endif // BIBLI_TAB_1_H_INCLUDED
