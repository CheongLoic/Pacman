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


int nb_joueur()
{//fonction renvoyant le nombre de joueurs enregistres dans le fichier de sauvegarde
    FILE *fichier=NULL;
    int n=0; // innitialisation de l'indice du tableau donnees
    int val1; //tableau
    int val2; //score
    char chaine[40];
    fichier=fopen("Donnees joueurs.txt","r"); //ouverture du fichier en lecture
    if (fichier!=NULL)
    {
        fgets(chaine,100,fichier); //lecture de la 1ere ligne du fichier
                                    //o¨´ il ya les noms des variables
        //printf("%s",chaine);
        do
        { //On fait une boucle qui parcoure le fichier pour connaitre le nombre de joueur
            fscanf(fichier,"%s %d %d",chaine,&val1,&val2);
            n+=1; //pour savoir combien y a-t-il de joueur pout faire une allocution dynamique
        } while (feof(fichier)==0); // tant que le fichier n'est pas arriv¨¦ a la fin
     }
    else {
            printf("ERREUR ! FICHIER STOCKAGE DES DONNEES NON TROUVE");
            exit(0); //force le programme a quitter pour ne pas faire la suite du programme dans le main
        }

    return n;
}

data* lecture_fichier()
{//lecture du fichier pour exploiter les donnees et les mettre dans un tableau de structure de type data
    data *donnees;
    FILE *fichier=NULL;
    int nb_joueur=0; // innitialisation de l'indice du tableau donnees
    int i=0; //index de ligne du tableau de structure donnees
    char chaine[40];
    int val1; //tableau
    int val2; //score
    fichier=fopen("Donnees joueurs.txt","r"); //ouverture du fichier en lecture
    if (fichier!=NULL)
    {
        fgets(chaine,100,fichier); //lecture de la 1ere ligne du fichier
                                    //o¨´ il ya les noms des variables
        //printf("%s",chaine);
        do
        { //On fait une boucle qui parcour le fichier pour connaitre le nombre de joueur
            fscanf(fichier,"%s %d %d",chaine,&val1,&val2);
            nb_joueur+=1; //pour savoir combien y a-t-il de joueur pout faire une allocution dynamique
        } while (feof(fichier)==0); // tant que le fichier n'est pas arriv¨¦ a la fin

        //printf("%d",nb_joueur);

        //relecture du fichier pour faire une allocation dynamique
        rewind(fichier); //on revient au debut du fichier
        fgets(chaine,40,fichier); //relit la 1ere ligne du fichier
        donnees=(data*)malloc((nb_joueur+1)*sizeof(data)); //on cree un tableau de structure contenant les donnees du joueur
        //il y a nb_joueur+1 lignes dont les nb_joueurs permieres lignes seront copies du fichier txt
        //la ligne nb_joueur+1 de la structure ne contiendra rien apres le do while suivant, pour eventuellement enregistrer
        //les donnees d'un joueur inconnu

        do
        {
            fscanf(fichier,"%s %d %d",chaine,&val1,&val2);
            strcpy(donnees[i].joueur,chaine);
            donnees[i].tableau=val1;
            donnees[i].score=val2;
            //printf("%s\t%d\t%d\n",donnees[i].joueur,donnees[i].tableau,donnees[i].score);
            i+=1;
        } while (i<nb_joueur);

        //la derniere ligne de la structure est vide pour rentrer les futurs donnees d'un joueur inconnu
        strcpy(donnees[nb_joueur].joueur,"");
        donnees[nb_joueur].tableau=NULL;
        donnees[nb_joueur].score=NULL;

        fclose(fichier);
    }
    else {
            printf("ERREUR ! FICHIER STOCKAGE DES DONNEES NON TROUVE");
            exit(0); //force le programme a quitter pour ne pas faire la suite du programme dans le main
        }

    return donnees;
}

void recherche_joueur(data *donnees, char *pseudo,int *index_joueur,int *trouve)
{// procedure qui cherche le pseudo du joueur dans le tableau de structure donnees pour les exploiter sinon les  cree

    int n=nb_joueur(); //nombre de joueurs enregistres
    int i=0;

    for(i=0;i<n;i++)
    {   if (strcmp(donnees[i].joueur,pseudo)==0) //compare le pseudo entree par le joueur aux donnees sauvegardees
        {//execute le reste du programme si la comparaison  est identique
            *trouve=1;
            *index_joueur=i;
            printf("\nJoueur trouve");
        }
    }
    if (*trouve==0)
    {
        printf("\nNouveau joueur\n");
        //si le nom du joueur n'est pas dans le tableau de structure
        *index_joueur=n;
        strcpy(donnees[n].joueur,pseudo); //copie le pseudo du joueur inconnu
        donnees[n].tableau=1; //le joueur incooe commence au tableau 1
        donnees[n].score=0; //le joueur inconnu commence le score ?0
        //l'ancien tableau de structure donnees est remplace par celui de copie_donnees avec une ligne en plus
    }
}

void sauvegarder(data *donnees,int *trouve)
{//Sauvegarde les donnees du joueur dans le fichier txt
    int n=nb_joueur(); //nombre de joueurs enregistres
    int i=0; //variable de boucle for
    FILE *fichier=NULL;

    fichier=fopen("Donnees joueurs.txt","w+");
    // "w+" permet d'ecrire et de lire le fichier mais supprime tout ce qu'il contenait au debut
    // on aurait pu utiliser "w" et ecrire par dessus le fichier, mais c'est plus complique ...
    if (fichier==NULL)
    {
        printf("ERREUR ! PROBLEME DE SAUVEGARDE");
        exit(0);
    }
    else
    {
        fprintf(fichier,"Joueur\t\tTableau\tScore");
        if (*trouve==1)
        {   //si le joueur est trouve dans le tableau de structure, on ecrit les n premieres lignes
            for(i=0;i<n;i++) fprintf(fichier,"\n%s\t\t%d\t%d",donnees[i].joueur,donnees[i].tableau,donnees[i].score);
        }
        else
        {   //si le joueur n'est pas trouve, on ecrit dans le fichier toutes les lignes du tableau de structure
            //la difference de ce boucle for par rapport a la ligne d'au dessus est la plage ou va l'index i
            for(i=0;i<=n;i++) fprintf(fichier,"\n%s\t\t%d\t%d",donnees[i].joueur,donnees[i].tableau,donnees[i].score);
        }
        fclose(fichier);
    }
}






