#include<stdio.h>
#include <stdlib.h>
#include<string.h>

typedef struct pile {  int numero;
                      struct pile* suivant;
                    }pile;
//****************************************************************************************************


// Insérer à la fin
pile *empiler(pile *p,int numero)
{
        pile *t=(pile*) malloc(sizeof(pile));
	if (t==NULL)
        printf ("Pas d'espace pour l'allocation\n");
	t->numero=numero;
	t->suivant=p;
	p=t;
	return p;
}
//******************************************************************************************************

int *depiler(pile **p)
{
        if (p==NULL)
        	return -1;
        int numero=(*p)->numero;
        pile *t=*p;
        *p=(*p)->suivant;
        free(t);
        return numero;
}
//******************************************************************************************************

int sommet(pile *p)
{
        if (p==NULL)
        {
         return -1;
        }
        return p->numero;
}

//******************************************************************************************************

void afficherPile(pile *p)
{
	printf ("AFFICHAGE PILE : \n");
        if (p==NULL)
        	printf("Pile vide \n");
       	else
	{
		pile *t=p;
		while (t!=NULL)
		{
			printf ("%d \n",t->numero);	
			t=t->suivant;
		}
	}
        return p->numero;
}
