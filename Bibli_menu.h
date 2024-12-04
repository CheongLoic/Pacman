#ifndef BIBLI_MENU_H_INCLUDED
#define BIBLI_MENU_H_INCLUDED

#include "Bibli_sauv.h"

char *name();
int option_vitesse(int *vitesse,int col);
int option_bordure(int mode,int col);
void menu();
void curseur_menu(char touche,int *lig, int col);
void option_menu_choisi(char touche,int *lig,int col,int *vitesse,int *mode,int *pt_vie,data *donnees,int *trouve,int *index_joueur);


#endif // BIBLI_MENU_H_INCLUDED
