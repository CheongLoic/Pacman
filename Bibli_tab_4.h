#ifndef BIBLI_TAB_4_H_INCLUDED
#define BIBLI_TAB_4_H_INCLUDED

typedef struct Serpent
{
    int new_X; //position actuelle du serpent en abscisse
    int new_Y; //position actuelle du serpent en ordonnee
    int last_X; //ancienne position serpent en abscisse
    int last_Y; ////ancienne position serpent en ordonnee
    int numero; //le enieme X du serpent affiche
    struct Serpent *suiv;
}t_serpent; //alias de la structure struct Serpent

void direction_serpent(char mat[22][52],int *direction, int *poslig, int *poscol,int *points,int *mode,int *pt_vie,t_serpent *pt);
void pos_queue(char mat[22][52],t_serpent *nouveau);
void deplacement(char mat[22][52],int *poslig,int *poscol,int *points,int *mode,int *pt_vie,int *direction,t_serpent *ancre);
t_serpent *init_snake(int *poslig,int *poscol,t_serpent *ancre);
void niveau_4(char mat[22][52],char touche,int *poslig,int *poscol,int *points, int *vitesse,int *mode,int *pt_vie,data *donnees,int *trouve,int *index_joueur,t_serpent *ancre);


#endif // BIBLI_TAB_4_H_INCLUDED
