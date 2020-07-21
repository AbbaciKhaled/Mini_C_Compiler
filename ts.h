#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

typedef struct element{
    char *nom;
    int type;
    int nature;
    int taille;
}element;

typedef struct eng_list_idf {int pos; struct eng_list_idf *svt;} eng_list_idf;

maxSize = 1000;
element* TS;

/////////////////////////// FONCTIONS DE HACHAGE ///////////////////////////

int hash1(char* str)
{
    int hash = 0 ;
    for (int i=0; i<strlen(str); i++)
        hash = hash + str[i];
    return hash % maxSize;
}

int hash2(char* str)
{
    int hash = 0 ;
    for (int i=0; i<strlen(str); i++)
        hash = hash + str[i];
    return 1 + (hash % (maxSize-1));
}

int hash(char* str, int i)
{
    return hash1(str) + hash2(str) * i;
}

///////////////////////////// INITIALISER TS /////////////////////////////

void initialiserTS()
{
    TS = (element *)malloc((maxSize+1)*sizeof(element));
    for (int i=0;i<maxSize;i++)
        TS[i].nom = " ";
}

//////////////////////////// INSERER DANS TS ////////////////////////////

void inserer(char* nom, int nature, int type, int taille)
{
    element elt;
    int insert = 0;
    int i = 0;

    while (insert == 0)
    {
        int cle = hash(nom, i);

        if (TS[cle].nom == " ")
        {
            elt.nom = nom;
            elt.nature = nature;
            elt.type = type;
            elt.taille = taille;
            TS[cle] = elt;
            insert = 1;
        }
        else
            i++;
    }
}

/////////////////////////// RECHERCHER DANS TS ///////////////////////////

int rechercher(char* nom)
{
    char* str = "";
    int i = 0;

    while (str != " ")
    {
        int cle = hash(nom, i);
        if (strcmp(TS[cle].nom,nom) == 0)
            return cle;
        else
        {
            str = TS[cle].nom;
            i++;
        }
    }
    return -1;
}

////////////////////////// RECUPERER INFOS IDF //////////////////////////


int getNature(char* nom)
{
    return TS[rechercher(nom)].nature;
}

int getType(char* nom)
{
    return TS[rechercher(nom)].type;
}

int getTaille(char* nom)
{
    return TS[rechercher(nom)].taille;
}

//////////////////////////// TS EST VIDE ? ////////////////////////////

int estVide()
{
    for (int i=0; i<maxSize; i++)
        if (TS[i].nom != " ")
            return 0;
    return 1;
}

//////////////////////////// AFFICHER TS /////////////////////////////


char* natureToString(int nature)
{

    switch(nature)
    {
        case 0 : return "Variable"; break;
        case 1 : return "Constante"; break;
        case 2 : return "Tableau"; break;
    }
}

char* typeToString(int type)
{
    switch(type)
    {
        case 0 : return "Entier"; break;
        case 1 : return "Reel"; break;
        case 2 : return "Caractere"; break;
	default : return "NULL"; break;
    }
}

void afficher()
{
    if (estVide() == 1)
        printf("La TS est vide\n");
    else
        for (int i=0;i<maxSize; i++)
        {
            if (TS[i].nom != " ")
                printf("Nom : %s | Nature : %s | Type : %s | Taille : %d\n",TS[i].nom,natureToString(TS[i].nature),typeToString(TS[i].type),TS[i].taille);
        }

}

//////////////////////////// MAJ TYPE /////////////////////////////


eng_list_idf *insererPos(eng_list_idf *tete,int pos)
{
        eng_list_idf *PtrInt=(eng_list_idf*) malloc(sizeof(eng_list_idf));
	if (PtrInt==NULL)
	printf ("Pas d'espace pour l'allocation\n");
	PtrInt->pos=pos;
	PtrInt->svt=tete;
	tete=PtrInt;
	return tete;
}

void majPos(eng_list_idf* tete, int type)
{
	eng_list_idf *PtrInt=tete;
	if (PtrInt==NULL)
	printf ("La liste est vide\n");
	else
	while (PtrInt!=NULL)
	{
		majType(PtrInt->pos,type);	
		PtrInt=PtrInt->svt;
	}
}




void majType(int pos, int type)
{
	TS[pos].type = type;
}




