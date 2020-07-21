#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include"pile.h"

elementQuad *teteQuadOpt=NULL;
int numQuadOpt;
elementQuad *quadsUtil=NULL;


// ================================================
int quadsEgaux (elementQuad *quad1, elementQuad *quad2)
{
	if (quad1 == NULL && quad2 == NULL ) return 1;
	else if (quad1 == NULL) return 0;
	else if (quad2 == NULL) return 0;
	else if (quad1->numQuad == quad2->numQuad && strcmp(quad1->op,quad2->op)==0 && strcmp(quad1->op1,quad2->op1)==0 && strcmp(quad1->op2,quad2->op2)==0 && strcmp(quad1->res,quad2->res)==0) return 1;
	else return 0;
}

int listesEgaux (elementQuad *tete1, elementQuad *tete2)
{
	elementQuad *PtrInt1=tete1;
	elementQuad *PtrInt2=tete2;

	while (PtrInt1 != NULL && PtrInt2 != NULL)
	{
		if (quadsEgaux (PtrInt1, PtrInt2) == 0) return 0;
		else
		{
			PtrInt1 = PtrInt1 -> suivant;
			PtrInt2 = PtrInt2 -> suivant;
		}
	}
	
	if (PtrInt1 != NULL || PtrInt2 != NULL) return 0;
	return 1;
}

elementQuad *copierListe(elementQuad *tete)
{
	elementQuad *teteCopie=NULL;
	
	elementQuad *PtrInt=tete;

	while (PtrInt!=NULL)
	{
		teteCopie = insererQuad(teteCopie,PtrInt->numQuad,PtrInt->op,PtrInt->op1,PtrInt->op2,PtrInt->res);
		PtrInt=PtrInt->suivant;
	}
	
	return teteCopie;
}
//=====================================================================================



//====================== PROPAGATION DE COPIE ========================
pile* quadBloc (elementQuad *teteOpt, int numQuad)
{
	pile *p = NULL;
	p = empiler(p,0);
	elementQuad *PtrInt=teteOpt;
	//printf ("------------%d\n",numQuad);


	while (PtrInt != NULL)
	{ 
		
		if (PtrInt->numQuad == numQuad)
			return p;
		if (PtrInt->numQuad == sommet(p))
			while (PtrInt->numQuad == sommet(p)) depiler(&p);

		if (strcmp(PtrInt->op,"BR")==0 && atoi(PtrInt->res)-1>sommet(p))
			p = empiler(p,atoi(PtrInt->res)-1);

		PtrInt = PtrInt -> suivant;
	}
	//while (numQuad == sommet(p)) depiler(&p);	
	return p;
}

int blocCompatible (pile* p1, pile* p2)
{

	//afficherPile(p1);
	//afficherPile(p2);
	if (sommet(p1) == sommet(p2))
		return 1;
	else if (sommet(p1) < sommet(p2))
		while (p1 != NULL && p2 != NULL) {if (sommet(p1) == sommet(p2)) return 1; depiler(&p2);}
	/*else if (sommet(p1) > sommet(p2))
		while (p1 != NULL && p2 != NULL) {if (sommet(p1) == sommet(p2)) return 1; depiler(&p1);}*/
	//printf("non compatibles .....................\n");
	return 0;
}


int pasDAffectation(elementQuad *ptr1, elementQuad *ptr2)
{

	elementQuad *PtrInt=ptr1->suivant;
	while (PtrInt!=NULL && PtrInt->numQuad!=ptr2->numQuad)
	{


		if (strcmp(PtrInt->op,"=")==0 && strcmp(PtrInt->res,ptr1->res)==0)
			return 0;
		PtrInt = PtrInt->suivant;
	}
	return 1;
}





elementQuad* propagationCopie(elementQuad *teteOpt, elementQuad *quad) 
{
	elementQuad *PtrInt=teteOpt;
	int boolean = 0;
	elementQuad *ptr=NULL;
	//if (strcmp(quad->op,"==")!=0 && strcmp(quad->op,"<")!=0 && strcmp(quad->op,"<=")!=0 && strcmp(quad->op,">")!=0 && strcmp(quad->op,">=")!=0 && strcmp(quad->op,"!=")!=0)
	while (PtrInt!=NULL && PtrInt->numQuad != quad->numQuad)
	{	
		if(strcmp(PtrInt->op,"=")==0 && (strcmp(PtrInt->res,quad->op1)==0 || strcmp(PtrInt->res,quad->op2)==0) 
		&& blocCompatible (quadBloc(teteOpt,PtrInt->numQuad),quadBloc(teteOpt,quad->numQuad))==1)
		{		
			if (strcmp(PtrInt->res,quad->op1)==0)
				boolean = 1;
			else
				boolean = 2;
			ptr = PtrInt;
		}
		PtrInt = PtrInt->suivant;
	}
	if (boolean == 1) teteOpt=insererQuad(teteOpt,numQuadOpt,quad->op,ptr->op1,quad->op2,quad->res);
	else if (boolean == 2) teteOpt=insererQuad(teteOpt,numQuadOpt,quad->op,quad->op1,ptr->op1,quad->res);
	else teteOpt=insererQuad(teteOpt,numQuadOpt,quad->op,quad->op1,quad->op2,quad->res);
	numQuadOpt++;
	return teteOpt;
}
//=====================================================================================


//====================== ELIMINATION CODE INUTILE ========================

void majBranchments(elementQuad *tete1,elementQuad *ptr)
{
	elementQuad *PtrInt=tete1;
	while (PtrInt!=NULL)
	{
		if ((strcmp(PtrInt->op,"BR")==0 || strcmp(PtrInt->op,"<")==0 || strcmp(PtrInt->op,"<=")== 0|| strcmp(PtrInt->op,">")== 0 || strcmp(PtrInt->op,">=")==0  || 			strcmp(PtrInt->op,"==")==0  || strcmp(PtrInt->op,"!=")==0))
		{
			if(atoi(PtrInt->res)>ptr->numQuad)
			{
				char s[10];
				sprintf(s,"%d",atoi(PtrInt->res)-1);
				strcpy(PtrInt->res,s);  
			}
		}
		PtrInt=PtrInt->suivant;
	}

	PtrInt=ptr->suivant;
	while (PtrInt!=NULL)
	{			
		if (strcmp(PtrInt->op,"BR")==0 || strcmp(PtrInt->op,"<")==0 || strcmp(PtrInt->op,"<=")== 0|| strcmp(PtrInt->op,">")== 0 || strcmp(PtrInt->op,">=")==0  || 			strcmp(PtrInt->op,"==")==0  || strcmp(PtrInt->op,"!=")==0)
		{
			char s[10];
			sprintf(s,"%d",atoi(PtrInt->res)-1);
			strcpy(PtrInt->res,s);
		}
		PtrInt->numQuad--;  
		PtrInt=PtrInt->suivant;
	}

}

/*
int estUtileIfElse(elementQuad *ptr1, char *idf, int numQuad)
{
	elementQuad *PtrInt=ptr1->suivant;
	while (PtrInt!=NULL && PtrInt->numQuad!=numQuad)
	{
		if (strcmp(PtrInt->op,"BR")==0)
			numQuad = atoi(PtrInt->res)-1;
		else if (strcmp(PtrInt->op,"==")==0 || strcmp(PtrInt->op,"<")==0 || strcmp(PtrInt->op,"<=")==0 || strcmp(PtrInt->op,">")==0 || strcmp(PtrInt->op,">=")==0 
		|| strcmp(PtrInt->op,"!=")==0)
			if (estUtileIfElse(PtrInt,idf,atoi(PtrInt->suivant->res)-1)==1)
				return 1;
		else if (strcmp(PtrInt->op,"=")==0 && strcmp(PtrInt->res,idf)==0) return 0;
		else if ((strcmp(PtrInt->op,"=")==0 && strcmp(PtrInt->op1,idf)==0) || (strcmp(PtrInt->op,"=")!=0) && (strcmp(PtrInt->op2,idf)==0 || strcmp(PtrInt->op1,idf)==0))  				return 1;
		PtrInt = PtrInt->suivant;
	}
	return 1;
}
*/

// Elimination de double = seulement
int estUtile(elementQuad *tete,elementQuad *teteTemp,elementQuad *ptrTemp)
{

	elementQuad *PtrInt=tete->suivant;
	elementQuad *PtrInt2=ptrTemp->suivant;

	if (PtrInt==NULL)
		return 1;

	if (strcmp(tete->op1,tete->res)==0)
		return 0;

	while (PtrInt!=NULL)
	{
		//if(sommet(quadBloc(teteTemp,PtrInt2->numQuad))==sommet(quadBloc (teteTemp,ptrTemp->numQuad))){
		//if (strcmp(PtrInt->op,"==")==0 || strcmp(PtrInt->op,"<")==0 || strcmp(PtrInt->op,"<=")==0 || strcmp(PtrInt->op,">")==0 || strcmp(PtrInt->op,">=")==0 
		//|| strcmp(PtrInt->op,"!=")==0)
			//if (estUtileIfElse(PtrInt,idf,atoi(PtrInt->suivant->res)-1)==1)
				//return 1;
		if ((strcmp(PtrInt->op,"=")==0 && strcmp(PtrInt->op1,tete->res)==0) || (strcmp(PtrInt->op,"=")!=0) && (strcmp(PtrInt->op2,tete->res)==0 || strcmp(PtrInt->op1,tete->res)==0))  				return 1;
		if ((strcmp(PtrInt->op,"=")==0 && strcmp(PtrInt->res,tete->res)==0)) {return 0;}
		PtrInt=PtrInt->suivant;
		PtrInt2=PtrInt2->suivant;
	}

	return 1;	
}


elementQuad* eliminationCode(elementQuad *tete) 
{	
	int nbBranchementsMaj =0;
	int demVerrouIf = 0;
	int finVerrouIf = 0;
	elementQuad *teteOpt=NULL;
	int numQ = 1;
	elementQuad *PtrInt=tete;
	elementQuad *teteTemp=NULL;
	teteTemp=copierListe(tete);	
	elementQuad * ptrTemp = teteTemp;

	if (PtrInt==NULL)
		return NULL;
	else
	while (PtrInt!=NULL)
	{ 	

		if (strcmp(PtrInt->op,"=")==0 )
		{
			if (estUtile(PtrInt,teteTemp,ptrTemp)==1)
			{
				teteOpt=insererQuad(teteOpt,numQ,PtrInt->op,PtrInt->op1,PtrInt->op2,PtrInt->res);
				numQ++;
			}
			else
			{	
				majBranchments(teteOpt,PtrInt);
				nbBranchementsMaj++;
			}
		}
		else
		{
			teteOpt=insererQuad(teteOpt,numQ,PtrInt->op,PtrInt->op1,PtrInt->op2,PtrInt->res);
			numQ++;
		}
		PtrInt=PtrInt->suivant;
		ptrTemp=ptrTemp->suivant;
	}
	return teteOpt;
}

//garder uniquement la derniere =
/*
int estUtile(elementQuad *tete,elementQuad *teteTemp,elementQuad *ptrTemp)

{


	elementQuad *PtrInt=tete->suivant;
	elementQuad *PtrInt2=ptrTemp->suivant;

	if (PtrInt==NULL)
		return 1;


	else if (ptrTemp->suivant!=NULL)
		if (strcmp(ptrTemp->suivant->op,"BR")==0)
			return 1;

	elementQuad *ptr=teteTemp;
	while (ptr->numQuad!=ptrTemp->numQuad)
	{
		if (strcmp(ptr->op,"BR")==0 && atoi(ptr->res)==ptrTemp->numQuad+1)
			return 1;
		ptr = ptr ->suivant;
	}

	if (strcmp(tete->op1,tete->res)==0)
		return 0;

	while (PtrInt!=NULL)
	{
		//if(sommet(quadBloc(teteTemp,PtrInt2->numQuad))==sommet(quadBloc (teteTemp,ptrTemp->numQuad))){
		//if (strcmp(PtrInt->op,"==")==0 || strcmp(PtrInt->op,"<")==0 || strcmp(PtrInt->op,"<=")==0 || strcmp(PtrInt->op,">")==0 || strcmp(PtrInt->op,">=")==0 
		//|| strcmp(PtrInt->op,"!=")==0)
			//if (estUtileIfElse(PtrInt,idf,atoi(PtrInt->suivant->res)-1)==1)
				//return 1;
		if ((strcmp(PtrInt->op,"=")==0 && strcmp(PtrInt->op1,tete->res)==0) || (strcmp(PtrInt->op,"=")!=0) && (strcmp(PtrInt->op2,tete->res)==0 || strcmp(PtrInt->op1,tete->res)==0))  				return 1;
		if ((strcmp(PtrInt->op,"=")==0 && strcmp(PtrInt->res,tete->res)==0)) {return 0;}
		PtrInt=PtrInt->suivant;
		PtrInt2=PtrInt2->suivant;
	}

	return 0;	
}


elementQuad* eliminationCode(elementQuad *tete) 
{	
	int nbBranchementsMaj =0;
	int demVerrouIf = 0;
	int finVerrouIf = 0;
	elementQuad *teteOpt=NULL;
	int numQ = 1;
	elementQuad *PtrInt=tete;
	elementQuad *teteTemp=NULL;
	teteTemp=copierListe(tete);	
	elementQuad * ptrTemp = teteTemp;

	if (PtrInt==NULL)
		return NULL;
	else
	while (PtrInt!=NULL)
	{ 	

		if (strcmp(PtrInt->op,"=")==0 )
		{
			if (estUtile(PtrInt,teteTemp,ptrTemp)==1)
			{
				teteOpt=insererQuad(teteOpt,numQ,PtrInt->op,PtrInt->op1,PtrInt->op2,PtrInt->res);
				numQ++;
			}
			else
			{	
				majBranchments(teteOpt,PtrInt);
				nbBranchementsMaj++;
			}
		}

		else
		{
			teteOpt=insererQuad(teteOpt,numQ,PtrInt->op,PtrInt->op1,PtrInt->op2,PtrInt->res);
			numQ++;
		}
		PtrInt=PtrInt->suivant;
		ptrTemp=ptrTemp->suivant;
	}
	return teteOpt;
}
*/
// ================================================
int expUtile(elementQuad *tete, char *temp)
{
	elementQuad *PtrInt=tete;
	if (PtrInt==NULL)
		return 0;
	else
	while (PtrInt!=NULL)
	{	
		if (strcmp(temp,PtrInt->op1)==0 || strcmp(temp,PtrInt->op2)==0)  {return 1;}
		PtrInt=PtrInt->suivant;
	}
	return 0;	
}
elementQuad* eliminationExpression(elementQuad *tete) 
{
	int nbBranchementsMaj =0;
	elementQuad *teteOpt=NULL;
	int numQ = 1;
	elementQuad *PtrInt=tete;
	if (PtrInt==NULL)
		return NULL;
	else
	while (PtrInt!=NULL)
	{	
		if ((strcmp(PtrInt->op,"+")==0 || strcmp(PtrInt->op,"-")==0 || strcmp(PtrInt->op,"*")==0 || strcmp(PtrInt->op,"/")==0 || strcmp(PtrInt->op,"=")==0) && rechercher(PtrInt->res)==-1)
		{ 
			if (expUtile(PtrInt->suivant,PtrInt->res)==1)
			{	
				teteOpt=insererQuad(teteOpt,numQ,PtrInt->op,PtrInt->op1,PtrInt->op2,PtrInt->res);
				numQ++;
			}
			else
			{
				majBranchments(teteOpt,PtrInt);
				nbBranchementsMaj++;
			}
		}
		else
		{
			teteOpt=insererQuad(teteOpt,numQ,PtrInt->op,PtrInt->op1,PtrInt->op2,PtrInt->res);
			numQ++;
		}
		PtrInt=PtrInt->suivant;
	}
	return teteOpt;
}
//=====================================================================================











//====================== SIMPLIFICATION ALGEBRIQUE ========================
int estUnNobmre (char *s)
{
	for (int i=0;i<strlen(s);i++)
		if(isdigit(s[i])==0 && s[i]!='.' && s[i]!='-' && s[i]!='+'  && s[i]!='(' && s[i]!=')')  {return 0;}
	return 1;
}

int estUnZero (char *s)
{
	if (estUnNobmre(s)==0) return 0;
	if(atoi(s)!=0)  return 0;
	return 1;
}

int estUnUn (char *s)
{
	if (estUnNobmre(s)==0) return 0;
	if(atoi(s)!=1)  return 0;
	return 1;
}

int operationArithmetique (int x,int y, char* op)
{
	if (strcmp(op,"+")==0) return x+y;
	else if (strcmp(op,"-")==0) return x-y;
	else if (strcmp(op,"*")==0) return x*y;
	else if (strcmp(op,"/")==0) return x/y;
}


elementQuad* simplificationAlgebrique(elementQuad *teteOpt, elementQuad *quad) 
{
	int demVerrouIf = 0;
	int finVerrouIf = 0;
	if (strcmp(quad->op,"*")==0 && (strcmp(quad->op1,"2")==0 || strcmp(quad->op2,"2")==0))
	{

		if (strcmp(quad->op1,"2")==0)
			teteOpt=insererQuad(teteOpt,numQuadOpt,"+",quad->op2,quad->op2,quad->res);
		else
			teteOpt=insererQuad(teteOpt,numQuadOpt,"+",quad->op1,quad->op1,quad->res);
		numQuadOpt++;
		return teteOpt;
	}
else if (strcmp(quad->op,"==")!=0 && strcmp(quad->op,"<")!=0 && strcmp(quad->op,"<=")!=0 && strcmp(quad->op,">")!=0 && strcmp(quad->op,">=")!=0 && strcmp(quad->op,"!=")!=0)
	if (strcmp(quad->op,"+")==0 || strcmp(quad->op,"-")==0 || strcmp(quad->op,"*")==0 || strcmp(quad->op,"/")==0 || strcmp(quad->op,"=")==0) 
	{
		if (rechercher(quad->op1)==-1 && estUnNobmre(quad->op1)==0)
		{ 
			elementQuad *PtrInt=teteOpt;
			while (PtrInt!=NULL)
			{
				/*if (strcmp(PtrInt->op,"==")==0 || strcmp(PtrInt->op,"!=")==0 || strcmp(PtrInt->op,"<=")==0 || strcmp(PtrInt->op,">=")==0 
				|| strcmp(PtrInt->op,"<")==0 || strcmp(PtrInt->op,">")==0 || strcmp(PtrInt->op,"BR")==0)
				{
					demVerrouIf=1;
					finVerrouIf=atoi(PtrInt->res);
				}
		
				if(finVerrouIf == PtrInt->numQuad)
					demVerrouIf=0; */

				if (strcmp(PtrInt->res,quad->op1)==0) // demVerrouIf==0 && 
				{
					if (estUnNobmre(PtrInt->op1)==1 && estUnNobmre(PtrInt->op2)==1)
					{
						char s[10];
	           				sprintf(s,"%d",operationArithmetique(numberOf(PtrInt->op1),numberOf(PtrInt->op2),PtrInt->op)); 
						teteOpt=insererQuad(teteOpt,numQuadOpt,quad->op,s,quad->op2,quad->res);	
						numQuadOpt ++;
						return teteOpt;
					}
					else if (estUnNobmre(PtrInt->op1)==1 && estUnNobmre(PtrInt->op2)==0)
					{
						if (estUnZero(PtrInt->op1)==1)
						{
							if(strcmp(PtrInt->op,"+")==0)
							{
								teteOpt=insererQuad(teteOpt,numQuadOpt,quad->op,PtrInt->op2,quad->op2,quad->res);	
								numQuadOpt ++;
								return teteOpt;
							}
							if(strcmp(PtrInt->op,"*")==0 || strcmp(PtrInt->op,"/")==0)
							{
								teteOpt=insererQuad(teteOpt,numQuadOpt,quad->op,"0",quad->op2,quad->res);	
								numQuadOpt ++;
								return teteOpt;
							}							
						}
						else if (estUnUn(PtrInt->op1)==1)
						{
							if(strcmp(PtrInt->op,"*")==0)
							{
								teteOpt=insererQuad(teteOpt,numQuadOpt,quad->op,PtrInt->op1,quad->op2,quad->res);	
								numQuadOpt ++;
								return teteOpt;
							}							
						}
						else if (estUnNobmre(quad->op2) && (strcmp(quad->op,"=")==0||((strcmp(PtrInt->op,"+")==0||strcmp(PtrInt->op,"-")==0)
						&&(strcmp(quad->op,"+")==0||strcmp(quad->op,"-")==0))||((strcmp(PtrInt->op,"*")==0) && (strcmp(quad->op,"*")==0))||
						((strcmp(PtrInt->op,"/")==0)&&(strcmp(quad->op,"/")==0))))
						{ 
							char s[10];
	           					sprintf(s,"%d",operationArithmetique(numberOf(PtrInt->op1),numberOf(quad->op2),quad->op)); 
							teteOpt=insererQuad(teteOpt,numQuadOpt,PtrInt->op,s,PtrInt->op2,quad->res);	
							numQuadOpt ++;
							return teteOpt;
						}
					}
					else if (estUnNobmre(PtrInt->op1)==0 && estUnNobmre(PtrInt->op2)==1)
					{
						if (estUnZero(PtrInt->op2)==1)
						{
							if(strcmp(PtrInt->op,"+")==0 || strcmp(PtrInt->op,"-")==0)
							{
								teteOpt=insererQuad(teteOpt,numQuadOpt,quad->op,PtrInt->op1,quad->op2,quad->res);	
								numQuadOpt ++;
								return teteOpt;
							}
							if(strcmp(PtrInt->op,"*")==0)
							{
								teteOpt=insererQuad(teteOpt,numQuadOpt,quad->op,"0",quad->op2,quad->res);	
								numQuadOpt ++;
								return teteOpt;
							}
						}
						else if (estUnUn(PtrInt->op2)==1)
						{
							if(strcmp(PtrInt->op,"*")==0 || strcmp(PtrInt->op,"/")==0)
							{
								teteOpt=insererQuad(teteOpt,numQuadOpt,quad->op,PtrInt->op1,quad->op2,quad->res);	
								numQuadOpt ++;
								return teteOpt;
							}
						}
						else if (estUnNobmre(quad->op2) && ((strcmp(PtrInt->op,"+")==0||strcmp(PtrInt->op,"-")==0)
						&&(strcmp(quad->op,"+")==0||strcmp(quad->op,"-")==0))||((strcmp(PtrInt->op,"*")==0) && (strcmp(quad->op,"*")==0))||
						((strcmp(PtrInt->op,"/")==0)&&(strcmp(quad->op,"/")==0)))
						{ 
							if ((strcmp(PtrInt->op,"+")==0 || strcmp(PtrInt->op,"-")==0) && (strcmp(quad->op,"+")==0 || strcmp(quad->op,"-")==0))
							{
								int x;
								if (strcmp(PtrInt->op,"-")==0) x=-1; else x=1;
								char s[10];
	           						sprintf(s,"%d",operationArithmetique(numberOf(PtrInt->op2) * x,numberOf(quad->op2),quad->op)); 
								teteOpt=insererQuad(teteOpt,numQuadOpt,"+",PtrInt->op1,s,quad->res);	
								numQuadOpt ++;
								return teteOpt;
							}
							else
							{
								char s[10];
	           						sprintf(s,"%d",operationArithmetique(numberOf(PtrInt->op2),numberOf(quad->op2),quad->op)); 
								teteOpt=insererQuad(teteOpt,numQuadOpt,PtrInt->op,PtrInt->op1,s,quad->res);	
								numQuadOpt ++;
								return teteOpt;
							}
						}
					}
				}
				PtrInt = PtrInt -> suivant;
			}
		}
		else if (rechercher(quad->op2)==-1 && estUnNobmre(quad->op2)==0)
		{ 
			elementQuad *PtrInt=teteOpt;
			while (PtrInt!=NULL)
			{
				/*if (strcmp(PtrInt->op,"==")==0 || strcmp(PtrInt->op,"!=")==0 || strcmp(PtrInt->op,"<=")==0 || strcmp(PtrInt->op,">=")==0 
				|| strcmp(PtrInt->op,"<")==0 || strcmp(PtrInt->op,">")==0 || strcmp(PtrInt->op,"BR")==0)
				{
					demVerrouIf=1;
					finVerrouIf=atoi(PtrInt->res);
				}
		
				if(finVerrouIf == PtrInt->numQuad)
					demVerrouIf=0;*/

				if (  strcmp(PtrInt->res,quad->op2)==0) //demVerrouIf==0 &&
				{
					if (estUnNobmre(PtrInt->op1)==1 && estUnNobmre(PtrInt->op2)==1)
					{
						char s[10];
	           				sprintf(s,"%d",operationArithmetique(numberOf(PtrInt->op1),numberOf(PtrInt->op2),PtrInt->op)); 
						teteOpt=insererQuad(teteOpt,numQuadOpt,quad->op,quad->op1,s,quad->res);	
						numQuadOpt ++;
						return teteOpt;
					}
					else if (estUnNobmre(PtrInt->op1)==1 && estUnNobmre(PtrInt->op2)==0)
					{
						if (estUnZero(PtrInt->op1)==1)
						{
							if(strcmp(PtrInt->op,"+")==0)
							{
								teteOpt=insererQuad(teteOpt,numQuadOpt,quad->op,quad->op1,PtrInt->op2,quad->res);	
								numQuadOpt ++;
								return teteOpt;
							}
							if(strcmp(PtrInt->op,"*")==0 || strcmp(PtrInt->op,"/")==0)
							{
								teteOpt=insererQuad(teteOpt,numQuadOpt,quad->op,quad->op1,"0",quad->res);	
								numQuadOpt ++;
								return teteOpt;
							}
						}
						else if (estUnUn(PtrInt->op1)==1)
						{
							if(strcmp(PtrInt->op,"*")==0)
							{
								teteOpt=insererQuad(teteOpt,numQuadOpt,quad->op,quad->op1,PtrInt->op2,quad->res);	
								numQuadOpt ++;
								return teteOpt;
							}
						}
						else if (estUnNobmre(quad->op1) && (strcmp(quad->op,"=")==0||((strcmp(PtrInt->op,"+")==0||strcmp(PtrInt->op,"-")==0)
						&&(strcmp(quad->op,"+")==0||strcmp(quad->op,"-")==0))||((strcmp(PtrInt->op,"*")==0) && (strcmp(quad->op,"*")==0))||
						((strcmp(PtrInt->op,"/")==0)&&(strcmp(quad->op,"/")==0))))
						{ 
							char s[10];
	           					sprintf(s,"%d",operationArithmetique(numberOf(PtrInt->op1),numberOf(quad->op1),quad->op)); 
							teteOpt=insererQuad(teteOpt,numQuadOpt,PtrInt->op,s,PtrInt->op2,quad->res);	
							numQuadOpt ++;
							return teteOpt;
						}
					}
					else if (estUnNobmre(PtrInt->op1)==0 && estUnNobmre(PtrInt->op2)==1)
					{
						if (estUnZero(PtrInt->op2)==1)
						{
							if(strcmp(PtrInt->op,"+")==0 || strcmp(PtrInt->op,"-")==0)
							{
								teteOpt=insererQuad(teteOpt,numQuadOpt,quad->op,quad->op1,PtrInt->op1,quad->res);	
								numQuadOpt ++;
								return teteOpt;
							}
							if(strcmp(PtrInt->op,"*")==0)
							{
								teteOpt=insererQuad(teteOpt,numQuadOpt,quad->op,quad->op1,"0",quad->res);	
								numQuadOpt ++;
								return teteOpt;
							}
						}
						else if (estUnUn(PtrInt->op2)==1)
						{
							if(strcmp(PtrInt->op,"*")==0 || strcmp(PtrInt->op,"/")==0)
							{
								teteOpt=insererQuad(teteOpt,numQuadOpt,quad->op,quad->op1,PtrInt->op1,quad->res);	
								numQuadOpt ++;
								return teteOpt;
							}
						}
						else if (estUnNobmre(quad->op1) && ((strcmp(PtrInt->op,"+")==0||strcmp(PtrInt->op,"-")==0)
						&&(strcmp(quad->op,"+")==0||strcmp(quad->op,"-")==0))||((strcmp(PtrInt->op,"*")==0) && (strcmp(quad->op,"*")==0))||
						((strcmp(PtrInt->op,"/")==0)&&(strcmp(quad->op,"/")==0)))
						{ 
							if ((strcmp(PtrInt->op,"+")==0 || strcmp(PtrInt->op,"-")==0) && (strcmp(quad->op,"+")==0 || strcmp(quad->op,"-")==0))
							{
								int x;
								if (strcmp(PtrInt->op,"-")==0) x=-1; else x=1;
								char s[10];
	           						sprintf(s,"%d",operationArithmetique(numberOf(PtrInt->op2) * x,numberOf(quad->op1),quad->op)); 
								teteOpt=insererQuad(teteOpt,numQuadOpt,"+",PtrInt->op1,s,quad->res);	
								numQuadOpt ++;
								return teteOpt;
							}
							else
							{
								char s[10];
	           						sprintf(s,"%d",operationArithmetique(numberOf(PtrInt->op2),numberOf(quad->op1),quad->op)); 
								teteOpt=insererQuad(teteOpt,numQuadOpt,PtrInt->op,PtrInt->op1,s,quad->res);	
								numQuadOpt ++;
								return teteOpt;
							}
						}
					}
				}
				PtrInt = PtrInt -> suivant;
			}
		}


	} 
	teteOpt=insererQuad(teteOpt,numQuadOpt,quad->op,quad->op1,quad->op2,quad->res);
	numQuadOpt++;
	return teteOpt;
}

//====================== ELIMINATION D'EXPRESSIONS COMMUNES ========================
int pasDeChangements(elementQuad *ptr1, elementQuad *ptr2)
{
	elementQuad *PtrInt=ptr1->suivant;
	while (PtrInt!=NULL && PtrInt->numQuad!=ptr2->numQuad)
	{
		if (strcmp(PtrInt->op,"==")==0 || strcmp(PtrInt->op,"<")==0 || strcmp(PtrInt->op,"<=")==0 || strcmp(PtrInt->op,">")==0 || strcmp(PtrInt->op,">=")==0 
		|| strcmp(PtrInt->op,"!=")==0)
			if (pasDeChangementsIfElse(PtrInt,ptr2,atoi(PtrInt->suivant->res)-1)==0)
				return 0;
		if (strcmp(PtrInt->op,"=")==0 && (strcmp(PtrInt->res,ptr2->op1)==0 || strcmp(PtrInt->res,ptr2->op2)==0))
			return 0;
		PtrInt = PtrInt->suivant;
	}
	return 1;
}

int pasDeChangementsIfElse(elementQuad *ptr1, elementQuad *ptr2, int numQuad)
{
	elementQuad *PtrInt=ptr1->suivant;
	while (PtrInt!=NULL && PtrInt->numQuad!=numQuad)
	{
		if (strcmp(PtrInt->op,"BR")==0)
			numQuad = atoi(PtrInt->res)-1;
		else if (strcmp(PtrInt->op,"==")==0 || strcmp(PtrInt->op,"<")==0 || strcmp(PtrInt->op,"<=")==0 || strcmp(PtrInt->op,">")==0 || strcmp(PtrInt->op,">=")==0 
		|| strcmp(PtrInt->op,"!=")==0)
			if (pasDeChangementsIfElse(PtrInt,ptr2,atoi(PtrInt->suivant->res)-1)==0)
				return 0;
		else if (strcmp(PtrInt->op,"=")==0 && (strcmp(PtrInt->res,ptr2->op1)==0 || strcmp(PtrInt->res,ptr2->op2)==0))
			return 0;
		PtrInt = PtrInt->suivant;
	}
	return 1;
}



elementQuad* eliminationExpCommunes(elementQuad *teteOpt, elementQuad *quad) 
{
	//printf ("---------------------- %d\n",quad->numQuad);
	int demVerrouIf = 0;
	int finVerrouIf = 0;
	elementQuad *PtrInt=teteOpt;
	if (PtrInt!=NULL)
	while (PtrInt!=NULL && PtrInt->numQuad!=quad->numQuad)
	{


			/*if (strcmp(PtrInt->op,"==")==0 || strcmp(PtrInt->op,"!=")==0 || strcmp(PtrInt->op,"<=")==0 || strcmp(PtrInt->op,">=")==0 || strcmp(PtrInt->op,"<")==0 
			|| strcmp(PtrInt->op,">")==0 || strcmp(PtrInt->op,"BR")==0)
			{
				demVerrouIf=1;
				finVerrouIf=atoi(PtrInt->res);
			}
		
			if(finVerrouIf == PtrInt->numQuad)
				demVerrouIf=0;*/

				
		if((strcmp(PtrInt->op,"+")==0 || strcmp(PtrInt->op,"-")==0 || strcmp(PtrInt->op,"*")==0 || strcmp(PtrInt->op,"/")==0) && strcmp(PtrInt->op,quad->op)==0 
			&& strcmp(PtrInt->op1,quad->op1)==0 && strcmp(PtrInt->op2,quad->op2)==0 && pasDeChangements(PtrInt,quad)==1)
		{//demVerrouIf==0 &&	
			//printf("+++++++++++++++++++++++++ %d aa %d\n",PtrInt->numQuad,quad->numQuad);
			elementQuad *ptr = quad->suivant;
			while (ptr!=NULL)
			{	
				if (blocCompatible (quadBloc(teteOpt,quad->numQuad),quadBloc(teteOpt,ptr->numQuad))==1)
				if (strcmp(ptr->op1,quad->res)==0)
					strcpy(ptr->op1,PtrInt->res);
				else if (strcmp(ptr->op2,quad->res)==0)
					strcpy(ptr->op2,PtrInt->res);
				ptr = ptr->suivant;
			}
			return teteOpt;	
		}
		PtrInt = PtrInt->suivant;
	}
	return teteOpt;
}
// ================================================

elementQuad *Commutative(elementQuad *tete)
{
	elementQuad *ptr=NULL;
	int numQuad=1;
	elementQuad *PtrInt=tete;
	while(PtrInt != NULL)
	{
		if (strcmp(PtrInt->op,"*")==0)
		{
			if (strcmp(PtrInt->suivant->op,"*")==0)
			{
			
				if (strcmp(PtrInt->suivant->suivant->op,"+")==0 
&& (strcmp(PtrInt->suivant->suivant->op1,PtrInt->suivant->res)==0 && strcmp(PtrInt->suivant->suivant->op2,PtrInt->res)==0)
|| (strcmp(PtrInt->suivant->suivant->op1,PtrInt->res)==0 && strcmp(PtrInt->suivant->suivant->op2,PtrInt->suivant->res)==0))
				{
					if (strcmp(PtrInt->op1,PtrInt->suivant->op1)==0)
					{
						ptr=insererQuad(ptr,numQuad,"+",PtrInt->op2,PtrInt->suivant->op2,PtrInt->suivant->res);
						numQuad++;
						ptr=insererQuad(ptr,numQuad,"*",PtrInt->op1,PtrInt->suivant->res,PtrInt->suivant->suivant->res);
						numQuad++;
						majBranchments(tete,PtrInt->suivant->suivant);

				
						PtrInt=PtrInt->suivant->suivant->suivant;
							}
					else if (strcmp(PtrInt->op1,PtrInt->suivant->op2)==0)
					{
						ptr=insererQuad(ptr,numQuad,"+",PtrInt->op2,PtrInt->suivant->op1,PtrInt->suivant->res);
						numQuad++;
						ptr=insererQuad(ptr,numQuad,"*",PtrInt->op1,PtrInt->suivant->res,PtrInt->suivant->suivant->res);
						numQuad++;
						majBranchments(tete,PtrInt->suivant->suivant);

						PtrInt=PtrInt->suivant->suivant->suivant;
					}
					else if (strcmp(PtrInt->op2,PtrInt->suivant->op1)==0)
					{
						ptr=insererQuad(ptr,numQuad,"+",PtrInt->op1,PtrInt->suivant->op2,PtrInt->suivant->res);
						numQuad++;
						ptr=insererQuad(ptr,numQuad,"*",PtrInt->op2,PtrInt->suivant->res,PtrInt->suivant->suivant->res);
						numQuad++;
						majBranchments(tete,PtrInt->suivant->suivant);

						PtrInt=PtrInt->suivant->suivant->suivant;
					}
					else if (strcmp(PtrInt->op2,PtrInt->suivant->op2)==0)
					{
						ptr=insererQuad(ptr,numQuad,"+",PtrInt->op1,PtrInt->suivant->op1,PtrInt->suivant->res);
						numQuad++;
						ptr=insererQuad(ptr,numQuad,"*",PtrInt->op2,PtrInt->suivant->res,PtrInt->suivant->suivant->res);
						numQuad++;
						majBranchments(tete,PtrInt->suivant->suivant);

						PtrInt=PtrInt->suivant->suivant->suivant;
					}
					else
					{
						ptr=insererQuad(ptr,numQuad,PtrInt->op,PtrInt->op1,PtrInt->op2,PtrInt->res);
						numQuad++;
						PtrInt=PtrInt->suivant;
					}	

				}
				else
					{
						ptr=insererQuad(ptr,numQuad,PtrInt->op,PtrInt->op1,PtrInt->op2,PtrInt->res);
						numQuad++;
						PtrInt=PtrInt->suivant;
					}	
			}
			else
					{
						ptr=insererQuad(ptr,numQuad,PtrInt->op,PtrInt->op1,PtrInt->op2,PtrInt->res);
						numQuad++;
						PtrInt=PtrInt->suivant;
					}	
		}
		else
					{
						ptr=insererQuad(ptr,numQuad,PtrInt->op,PtrInt->op1,PtrInt->op2,PtrInt->res);
						numQuad++;
						PtrInt=PtrInt->suivant;
					}	
	}
return ptr;
}




// ================================================
elementQuad* Optimiser(elementQuad *tete)
{

	
		elementQuad* ptr;
		elementQuad* copie;
		elementQuad* PtrInt;
		numQuadOpt=1;
		elementQuad *teteCopie = NULL;

		do{  PtrInt = tete; copie = copierListe(teteQuadOpt);  teteQuadOpt=NULL;
		
		while (PtrInt != NULL)
		{
			
			//propagation
			teteQuadOpt=propagationCopie(teteQuadOpt, PtrInt);


			//simplification

			//sauv
			int num=numQuadOpt;
			ptr = PtrInt;

			do{ 
				numQuadOpt=1;
				tete = copierListe(teteQuadOpt);
				PtrInt = tete;
				teteCopie=copierListe(teteQuadOpt);
				teteQuadOpt=NULL;

				while (PtrInt != NULL)
				{	
					teteQuadOpt=simplificationAlgebrique(teteQuadOpt, PtrInt);
					PtrInt = PtrInt -> suivant;
				}
			}while(listesEgaux(teteQuadOpt, teteCopie) == 0);


			


			//recup
			numQuadOpt=num;
			PtrInt = ptr;

			//exp communes
			teteQuadOpt=eliminationExpCommunes(teteQuadOpt,PtrInt);



			teteQuadOpt = copierListe(tete);
			PtrInt = PtrInt -> suivant; 
		}
		//afficherQuad(teteQuadOpt);


		teteQuadOpt = tete; 


		do{
			teteCopie=copierListe(teteQuadOpt);
			teteQuadOpt=eliminationCode(teteQuadOpt);
			teteQuadOpt=eliminationExpression(teteQuadOpt);
		}while(listesEgaux(teteQuadOpt, teteCopie) == 0);
		//afficherQuad(teteQuadOpt);
			tete=copierListe(teteQuadOpt);
		}while(listesEgaux(teteQuadOpt, copie) == 0);
		//teteQuadOpt = Commutative(teteQuadOpt);		
	return teteQuadOpt;
}


