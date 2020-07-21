#include<stdio.h>
#include <stdlib.h>
#include<string.h>

typedef struct elementQuad {  int numQuad;
                      char *op;
                      char *op1; 
                      char *op2;
                      char *res;
                      struct elementQuad* suivant;
                    }elementQuad;
//****************************************************************************************************
typedef struct elementRacFus {  
			int val;
                    	struct elementRacFus* suivant;
                    }elementRacFus;

typedef struct strElementRacFus{elementRacFus *vrai;elementRacFus *faux;}strElementRacFus;


//****************************************************************************************************
elementQuad *teteQuad=NULL;



// Insérer à la fin
elementQuad *insererQuad(elementQuad *tete,int numQuad,char*op,char *op1,char *op2,char *res)
{


        elementQuad *PtrInt=(elementQuad*) malloc(sizeof(elementQuad));
	if (PtrInt==NULL)
        printf ("Pas d'espace pour l'allocation\n");
	PtrInt->numQuad=numQuad;
	PtrInt->op=malloc (strlen(&op)+1);
	strcpy(PtrInt->op,op);
        PtrInt->op1=malloc (strlen(&op1)+1);
	strcpy(PtrInt->op1,op1);
	PtrInt->op2=malloc (strlen(&op2)+1);
	strcpy(PtrInt->op2,op2);
	PtrInt->res=malloc (strlen(&res)+1);
	strcpy(PtrInt->res,res);
	PtrInt->suivant=NULL;

	if(tete == NULL) return PtrInt;	

	elementQuad *teteInt=(elementQuad*) malloc(sizeof(elementQuad));
	teteInt=tete;
	while (teteInt->suivant!=NULL)
		teteInt=teteInt->suivant;

	teteInt -> suivant = PtrInt;


	return tete;
}
//******************************************************************************************************

void afficherQuad(elementQuad* tete)
{
	elementQuad *PtrInt=tete;
	if (PtrInt==NULL)
	printf ("La liste est vide\n");
	else
	while (PtrInt!=NULL)
	{
		printf ("%2d - ( %3s , %5s , %5s , %5s )\n",PtrInt->numQuad,PtrInt->op,PtrInt->op1,PtrInt->op2,PtrInt->res);
		PtrInt=PtrInt->suivant;
	}
}

//******************************************************************************************************
void modifierPositionQuad(elementQuad* tete,int numQuad,int position)
{

	elementQuad *PtrInt=tete;
	if (PtrInt==NULL)
	printf ("La liste est vide\n");
	else
	while (PtrInt!=NULL)
	{
                if(PtrInt->numQuad==numQuad)
                {
                 char t[10]; 
                 sprintf(t,"%d",position); 
                PtrInt->res=malloc (strlen(&t)+1);
	        strcpy(PtrInt->res,t);
                }
		PtrInt=PtrInt->suivant;
	}
}

//******************************************************************************************************
char *branchement(char *chaine)
{
if (strcmp(chaine,"==")==0) return "BNE";
if (strcmp(chaine,"!=")==0) return "BE";
if (strcmp(chaine,"<")==0) return "BGE";
if (strcmp(chaine,"<=")==0) return "BG";
if (strcmp(chaine,">")==0) return "BLE";
if (strcmp(chaine,">=")==0) return "BL";
}


//******************************************************************************************
elementRacFus *insererRacFus(elementRacFus *tete,int val)
{	
        elementRacFus *PtrInt=(elementRacFus*) malloc(sizeof(elementRacFus));
	if (PtrInt==NULL)
	printf ("Pas d'espace pour l'allocation\n");
	PtrInt->val=val;
	PtrInt->suivant=tete;
	tete=PtrInt;
	return tete;
}


void Raccorder (elementRacFus* tete, int numQuad)
{

	elementRacFus *PtrInt=tete;
	if (PtrInt==NULL)
	printf ("La liste est vide\n");
	else
	while (PtrInt!=NULL)
	{
	     	modifierPositionQuad(teteQuad,PtrInt->val,numQuad);
		PtrInt=PtrInt->suivant;
	}
}

void RaccorderUnElement (int elt, int numQuad)
{
	modifierPositionQuad(teteQuad,elt,numQuad);
}



elementRacFus *Fusionner (elementRacFus* tete1, elementRacFus* tete2)
{

	if (tete1 == NULL && tete2 == NULL ) return NULL;
	if (tete1 == NULL) {printf("yes++++++++\n");return tete2;}
	if (tete2 == NULL) return tete1;

        elementRacFus *PtrInt=(elementRacFus*) malloc(sizeof(elementRacFus));
	PtrInt=tete1;

	while (PtrInt->suivant!=NULL)
		PtrInt=PtrInt->suivant;
	PtrInt->suivant = tete2;
	return tete1;
}

elementRacFus *FusionnerUnElement (elementRacFus* tete1, int elt)
{
	elementRacFus *tete2 = (elementRacFus*) malloc(sizeof(elementRacFus));
	tete2 = NULL;
	tete2 = insererRacFus(tete2,elt);

	tete2->suivant = tete1;
	return tete2;
}




strElementRacFus *insererStrRecFus(elementRacFus *tete,int quad1, int quad2)
{
        strElementRacFus *PtrInt=(strElementRacFus*) malloc(sizeof(strElementRacFus));
	if (PtrInt==NULL)
	printf ("Pas d'espace pour l'allocation\n");
	PtrInt->vrai = (elementRacFus*) malloc(sizeof(elementRacFus));
	PtrInt->vrai = insererRacFus(PtrInt->vrai,quad1);
	PtrInt->faux = (elementRacFus*) malloc(sizeof(elementRacFus));
	PtrInt->faux = insererRacFus(PtrInt->faux,quad2);
	tete=PtrInt;
	return tete;
}

void afficherERF(elementRacFus* tete)
{
elementRacFus *PtrInt=tete;
	if (PtrInt==NULL)
	printf ("La liste est vide\n");
	else
	while (PtrInt!=NULL)
	{
		printf ("%d ELT\n",PtrInt->val);
		PtrInt=PtrInt->suivant;
	}
}

