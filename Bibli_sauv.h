#ifndef BIBLI_SAUV_H_INCLUDED
#define BIBLI_SAUV_H_INCLUDED

typedef struct players_data
{
    char joueur[15];
    int tableau;
    int score;
    //int partie_finie;
}data;


int nb_joueur();
data* lecture_fichier();
void recherche_joueur(data* donnees, char *pseudo,int *index_joueur,int *trouve);
void sauvegarder(data *donnees,int *trouve);


#endif // BIBLI_SAUV_H_INCLUDED
