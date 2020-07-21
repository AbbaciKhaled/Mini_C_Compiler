#include<stdio.h>
#include <stdlib.h>
#include<string.h>



//****************************************************************************************************
typedef struct elementLabel {  
			int num;
                    	struct elementLabel* suivant;
                    }elementLabel;


typedef struct elementVAR {  
			char* elt;
                    	struct elementVAR* suivant;
                    }elementVAR;

int nbEltPile = 0;

//****************************************************************************************************

elementLabel *insererLabel(elementLabel *tete, int label)
{
        elementLabel *PtrInt=(elementLabel*) malloc(sizeof(elementLabel));
	if (PtrInt==NULL)
	printf ("Pas d'espace pour l'allocation\n");
	PtrInt->num = label;
	PtrInt->suivant=tete;
	tete=PtrInt;
	return tete;
}

int existeLabel(elementLabel *tete, int label)
{
	elementLabel *PtrInt=(elementLabel*) malloc(sizeof(elementLabel));
	PtrInt = tete;
	while (PtrInt != NULL)
	{
		if(PtrInt->num == label)
			return 1;
		PtrInt = PtrInt->suivant;
	}
	return 0;
}


//****************************************************************************************************

elementVAR *insererVAR(elementVAR *tete, char* elt)
{
        elementVAR *PtrInt=(elementVAR*) malloc(sizeof(elementVAR));
	if (PtrInt==NULL)
	printf ("Pas d'espace pour l'allocation\n");
	PtrInt->elt=malloc (strlen(&elt)+1);
	strcpy(PtrInt->elt,elt);
	PtrInt->suivant=tete;
	tete=PtrInt;
	return tete;
}

int existeVAR(elementVAR *tete, char* elt)
{
	elementVAR *PtrInt=(elementVAR*) malloc(sizeof(elementVAR));
	PtrInt = tete;
	while (PtrInt != NULL)
	{
		if(strcmp(PtrInt->elt,elt)==0)
			return 1;
		PtrInt = PtrInt->suivant;
	}
	return 0;
}

//****************************************************************************************************
int nbOccurances (elementQuad* tete, char* temp)
{
	elementQuad *PtrInt=tete;
	int nbOcc=0;
	while (PtrInt!=NULL)
	{
		if (strcmp(PtrInt->op1,temp)==0 || strcmp(PtrInt->op2,temp)==0)
			nbOcc++;
		PtrInt=PtrInt->suivant;
	}
	return nbOcc;
}
int tempToInt (char s[])
{

	int x=0;
	int y=1;

	
	for (int i=1; i<strlen(s); i++)
			{y = y * 10;}

	y = y /10;
	
	for (int i=1; i<strlen(s); i++)
	{
		x = x + ( ( s[i] - 48 ) * y );
		y = y / 10;
	}
	return x;
}
//****************************************************************************************************


void afficherEnTete(elementQuad* tete)
{
	printf("TITLE prog.asm: Code Objet\n");	
	printf("Pile segment stack;\n");
	printf("\tdw 100 dup(?)\n");	
	printf("Pile ends\n");	
	printf("\nDATA segment\n");
	
	printf("\tTAB DW dup(?)\n");

	elementVAR *teteVAR = NULL;

	elementQuad *PtrInt=tete;
	while (PtrInt!=NULL)
	{
		if ((strcmp(PtrInt->op,"+")==0 || strcmp(PtrInt->op,"-")==0 || strcmp(PtrInt->op,"*")==0 || strcmp(PtrInt->op,"/")==0 || strcmp(PtrInt->op,"=")==0) && 
		rechercher(PtrInt->op1)!=-1 && existeVAR(teteVAR,PtrInt->op1)==0)
		{
			teteVAR=insererVAR(teteVAR,PtrInt->op1);
			printf("\t%s DW ?\n",PtrInt->op1);
		}
		if ((strcmp(PtrInt->op,"+")==0 || strcmp(PtrInt->op,"-")==0 || strcmp(PtrInt->op,"*")==0 || strcmp(PtrInt->op,"/")==0) && 
		rechercher(PtrInt->op2)!=-1 && existeVAR(teteVAR,PtrInt->op2)==0)
		{
			teteVAR=insererVAR(teteVAR,PtrInt->op2);
			printf("\t%s DW ?\n",PtrInt->op2);
		}
		if ((strcmp(PtrInt->op,"+")==0 || strcmp(PtrInt->op,"-")==0 || strcmp(PtrInt->op,"*")==0 || strcmp(PtrInt->op,"/")==0 || strcmp(PtrInt->op,"=")==0) && 
		rechercher(PtrInt->res)!=-1 && existeVAR(teteVAR,PtrInt->res)==0)
		{
			teteVAR=insererVAR(teteVAR,PtrInt->res);
			printf("\t%s DW ?\n",PtrInt->res);
		}
		PtrInt=PtrInt->suivant;
	}



	printf("DATA ends\n");	
	printf("\nCODE segment\n");
	printf("\tMAIN:\n");	
	printf("\t\tASSUME CS:CODE, DS:DATE:\n");	
	printf("\t\tMOV AX , DATA\n");
	printf("\t\tMOV DX , AX\n\n");
}

void afficherCodeMachine(elementQuad* tete)
{
	
	afficherEnTete(tete);
	elementQuad *PtrInt=tete;
	int lastQuad=1;
	elementLabel *teteLabel = NULL;

	if (PtrInt==NULL)
		printf ("La liste est vide\n");
	
	else
	while (PtrInt!=NULL)
	{

		if ( existeLabel(teteLabel, PtrInt->numQuad) == 1)
				printf("\t\tLabel_%d : ", PtrInt->numQuad);

		if (strcmp(PtrInt->op,"=")==0)
		{
			if (rechercher(PtrInt->op1)==-1 && estUnNobmre(PtrInt->op1)==0)
			{
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
				printf ("\t\tMOV %s , AX\n",PtrInt->res);	
			}
			else
				printf ("\t\tMOV %s , %s\n",PtrInt->res,PtrInt->op1);	
		}

		else if (strcmp(PtrInt->op,"+")==0)
		{
			if (rechercher(PtrInt->op1)==-1 && estUnNobmre(PtrInt->op1)==0 && rechercher(PtrInt->op2)==-1 && estUnNobmre(PtrInt->op2)==0)
			{
				if (nbOccurances(tete,PtrInt->op2)<=1)
					printf ("\t\tPOP BX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op2));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV BX , TAB[SI]\n");
				}
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
				printf ("\t\tADD AX , BX\n");	
			}
			else if (rechercher(PtrInt->op1)==-1 && estUnNobmre(PtrInt->op1)==0)
			{
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
				printf ("\t\tADD AX , %s\n",PtrInt->op2);
			}
			else if (rechercher(PtrInt->op2)==-1 && estUnNobmre(PtrInt->op2)==0)
			{
				if (nbOccurances(tete,PtrInt->op2)<=1)
					printf ("\t\tPOP BX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op2));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV BX , TAB[SI]\n");
				}
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);
				printf ("\t\tADD AX , BX\n");	
			}
			else
			{
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);
				printf ("\t\tADD AX , %s\n",PtrInt->op2);
			}
			if (nbOccurances(tete,PtrInt->res)<=1)
				printf ("\t\tPUSH AX\n");
			else
			{
				printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->res));
				printf ("\t\tADD SI , SI\n");
				printf ("\t\tMOV TAB[SI] , AX\n");
			}	
		}

		else if (strcmp(PtrInt->op,"-")==0)
		{
			if (rechercher(PtrInt->op1)==-1 && estUnNobmre(PtrInt->op1)==0 && rechercher(PtrInt->op2)==-1 && estUnNobmre(PtrInt->op2)==0)
			{
				if (nbOccurances(tete,PtrInt->op2)<=1)
					printf ("\t\tPOP BX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op2));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV BX , TAB[SI]\n");
				}
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
				printf ("\t\tSUB AX , BX\n");	
			}
			else if (rechercher(PtrInt->op1)==-1 && estUnNobmre(PtrInt->op1)==0)
			{
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
				printf ("\t\tSUB AX , %s\n",PtrInt->op2);
			}
			else if (rechercher(PtrInt->op2)==-1 && estUnNobmre(PtrInt->op2)==0)
			{
				if (nbOccurances(tete,PtrInt->op2)<=1)
					printf ("\t\tPOP BX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op2));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV BX , TAB[SI]\n");
				}
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);
				printf ("\t\tSUB AX , BX\n");	
			}
			else
			{
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);
				printf ("\t\tSUB AX , %s\n",PtrInt->op2);
			}
			if (nbOccurances(tete,PtrInt->res)<=1)
				printf ("\t\tPUSH AX\n");
			else
			{
				printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->res));
				printf ("\t\tADD SI , SI\n");
				printf ("\t\tMOV TAB[SI] , AX\n");
			}		
		}

		else if (strcmp(PtrInt->op,"*")==0)
		{
			if (rechercher(PtrInt->op1)==-1 && estUnNobmre(PtrInt->op1)==0 && rechercher(PtrInt->op2)==-1 && estUnNobmre(PtrInt->op2)==0)
			{
				if (nbOccurances(tete,PtrInt->op2)<=1)
					printf ("\t\tPOP BX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op2));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV BX , TAB[SI]\n");
				}
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
				printf ("\t\tMUL BX\n");	
			}
			else if (rechercher(PtrInt->op1)==-1 && estUnNobmre(PtrInt->op1)==0)
			{
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
				printf ("\t\tMUL %s\n",PtrInt->op2);
			}
			else if (rechercher(PtrInt->op2)==-1 && estUnNobmre(PtrInt->op2)==0)
			{
				if (nbOccurances(tete,PtrInt->op2)<=1)
					printf ("\t\tPOP BX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op2));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV BX , TAB[SI]\n");
				}
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);
				printf ("\t\tMUL BX\n");	
			}
			else
			{
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);
				printf ("\t\tMUL %s\n",PtrInt->op2);
			}
			if (nbOccurances(tete,PtrInt->res)<=1)
				printf ("\t\tPUSH AX\n");
			else
			{
				printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->res));
				printf ("\t\tADD SI , SI\n");
				printf ("\t\tMOV TAB[SI] , AX\n");
			}		
		}

		else if (strcmp(PtrInt->op,"/")==0)
		{
			if (rechercher(PtrInt->op1)==-1 && estUnNobmre(PtrInt->op1)==0 && rechercher(PtrInt->op2)==-1 && estUnNobmre(PtrInt->op2)==0)
			{
				if (nbOccurances(tete,PtrInt->op2)<=1)
					printf ("\t\tPOP BX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op2));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV BX , TAB[SI]\n");
				}
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
				printf ("\t\tDIV BX\n");	
			}
			else if (rechercher(PtrInt->op1)==-1 && estUnNobmre(PtrInt->op1)==0)
			{
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
				printf ("\t\tDIV %s\n",PtrInt->op2);
			}
			else if (rechercher(PtrInt->op2)==-1 && estUnNobmre(PtrInt->op2)==0)
			{
				if (nbOccurances(tete,PtrInt->op2)<=1)
					printf ("\t\tPOP BX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op2));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV BX , TAB[SI]\n");
				}
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);
				printf ("\t\tDIV BX\n");	
			}
			else
			{
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);
				printf ("\t\tDIV %s\n",PtrInt->op2);
			}
			if (nbOccurances(tete,PtrInt->res)<=1)
				printf ("\t\tPUSH AX\n");
			else
			{
				printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->res));
				printf ("\t\tADD SI , SI\n");
				printf ("\t\tMOV TAB[SI] , AX\n");
			}		
		}
		
		else if (strcmp(PtrInt->op,"<")==0)
		{
			if (rechercher(PtrInt->op1)==-1 && estUnNobmre(PtrInt->op1)==0)
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
			else
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);	
			printf ("\t\tCMP AX , %s\n",PtrInt->op2);	
			printf ("\t\tJL Label_%s\n",PtrInt->res);
			teteLabel = insererLabel(teteLabel,atoi(PtrInt->res));	
		}

		else if (strcmp(PtrInt->op,"<=")==0)
		{
			if (rechercher(PtrInt->op1)==-1 && estUnNobmre(PtrInt->op1)==0)
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
			else
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);	
			printf ("\t\tCMP AX , %s\n",PtrInt->op2);	
			printf ("\t\tJLE Label_%s\n",PtrInt->res);
			teteLabel = insererLabel(teteLabel,atoi(PtrInt->res));	
		}

		else if (strcmp(PtrInt->op,">")==0)
		{
			if (rechercher(PtrInt->op1)==-1 && estUnNobmre(PtrInt->op1)==0)
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
			else
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);	
			printf ("\t\tCMP AX , %s\n",PtrInt->op2);	
			printf ("\t\tJG Label_%s\n",PtrInt->res);
			teteLabel = insererLabel(teteLabel,atoi(PtrInt->res));	
		}

		else if (strcmp(PtrInt->op,">=")==0)
		{
			if (rechercher(PtrInt->op1)==-1 && estUnNobmre(PtrInt->op1)==0)
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
			else
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);	
			printf ("\t\tCMP AX , %s\n",PtrInt->op2);	
			printf ("\t\tJGE Label_%s\n",PtrInt->res);
			teteLabel = insererLabel(teteLabel,atoi(PtrInt->res));	
		}

		else if (strcmp(PtrInt->op,"==")==0)
		{
			if (rechercher(PtrInt->op1)==-1 && estUnNobmre(PtrInt->op1)==0)
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
			else
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);	
			printf ("\t\tCMP AX , %s\n",PtrInt->op2);	
			printf ("\t\tJE Label_%s\n",PtrInt->res);
			teteLabel = insererLabel(teteLabel,atoi(PtrInt->res));	
		}

		else if (strcmp(PtrInt->op,"!=")==0)
		{
			if (rechercher(PtrInt->op1)==-1 && estUnNobmre(PtrInt->op1)==0)
				if (nbOccurances(tete,PtrInt->op1)<=1)
					printf ("\t\tPOP AX\n");
				else
				{
					printf ("\t\tMOV SI , %d\n",tempToInt(PtrInt->op1));
					printf ("\t\tADD SI , SI\n");
					printf ("\t\tMOV AX , TAB[SI]\n");
				}
			else
				printf ("\t\tMOV AX , %s\n",PtrInt->op1);	
			printf ("\t\tCMP AX , %s\n",PtrInt->op2);	
			printf ("\t\tJNE Label_%s\n",PtrInt->res);
			teteLabel = insererLabel(teteLabel,atoi(PtrInt->res));	
		}

		else if (strcmp(PtrInt->op,"BR")==0)
		{
			printf ("\t\tJMP Label_%s\n",PtrInt->res);
			teteLabel = insererLabel(teteLabel,atoi(PtrInt->res));	
		}
		lastQuad++;
		PtrInt=PtrInt->suivant;
	}
	
	if ( existeLabel(teteLabel, lastQuad) == 1)
	printf("\t\tLabel_%d : \n", lastQuad);
	printf("\tMAIN ends\n");	
	printf("CODE ends\n");	
}


//============================================
int utiliseParLaSuite(elementQuad* tete, char* temp)
{
	elementQuad* PtrInt=tete;
	while (PtrInt != NULL)
	{
		if (strcmp(PtrInt->op1,temp)==0 || strcmp(PtrInt->op2,temp)==0)
			return 1;
		PtrInt = PtrInt ->suivant;
	}
	return 0;
}






