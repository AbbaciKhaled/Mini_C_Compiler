%{

#include<stdio.h>
#include "ts.h"
#include "quad.h"
#include "optimisation.h"
#include "machine.h"

extern FILE* yyin;
extern FILE* yyout;
extern int src;
extern int ligne;
extern int colonne;

int type;


int numQuad=1;

int numT=0;

pile *pileAffectation=NULL;


%}

%union
{char *chaine;
int entier;
float reel;
char caractere;

struct eng_list_idf *def_liste_idf;
struct str2{int vrai;int faux;}str2;
struct elementRacFus *liste_next;
struct str1{char *val;int type;}str1;
}

%token If Elif Else For In Range While Int Float Char And Or InfEg SupEg Eg NonEg '=' '(' ')' '[' ']' ',' '+' '-' '*' '/' '<' '>' Saut Tab NonTab Suite
%token <chaine> Idf
%token <entier> Entier
%token <reel> Reel
%token <caractere> Caractere



%type <str1> Affectation
%type <str1> Operation1
%type <str1> Operation2
%type <str1> Operation3
%type <str1> Valeur
%type <chaine> OperateurComp
%type <def_liste_idf> ListeVar
%type <entier> Type
%type <str2> Condition
%type <entier> M
%type <liste_next> ConditionIf
%type <liste_next> N
%type <liste_next> InstructionTab
%type <liste_next> BlocInst
%type <liste_next> Instruction
%type <liste_next> BlocElif

%start Z

%%

Z : S Sauts S {printf("\nFIN DU PROGRAMME\n"); return;} | Sauts {printf("\nFIN DU PROGRAMME\n"); return;} | S {printf("\nFIN DU PROGRAMME\n"); return;} | Sauts S {printf("\nFIN DU PROGRAMME\n"); return;}
;

Sauts : Sauts Saut | Saut
;

S:  S M Instruction { Raccorder($<liste_next>1,$2); Raccorder($<liste_next>3,numQuad);} | S M Declaration { Raccorder($<liste_next>1,$2);} | Instruction   { Raccorder($<liste_next>1,numQuad); }  | Declaration
;


Declaration : DeclarationType Saut
| Affectation {if (rechercher($1.val)==-1) {inserer($1.val,0,$1.type,1);} else{{printf ("ERREUR SEMANTIQUE : idf déja déclaré , Ligne : , %d Colonne : %d , Entité : %s \n",ligne,colonne,$1.val); return;}}} DeclarationInit Saut
| DeclarationStr Saut
;

DeclarationType : Type ListeVar {majPos($2,$1);}
;

Instruction : Affectation  Saut {if (rechercher($1.val)==-1) {inserer($1.val,0,$1.type,1);} else {if (getType($1.val) != $1.type) {printf ("ERREUR SEMANTIQUE : incompatibilité des types , Ligne : , %d Colonne : %d , Type 1 : %s , Type 2 : %s \n",ligne,colonne,typeToString(getType($1.val)),typeToString($1.type)); return;}} $$=NULL; }
| ConditionIf { $$=$1;}
;

Type : Int {type = 0; $$=0;} | Float {type = 1; $$=1;} | Char {type = 2; $$=2;}
;



ListeVar : Idf {if (rechercher($1)!=-1) {printf ("ERREUR SEMANTIQUE : idf déja déclaré , Ligne : , %d Colonne : %d , Entité : %s \n",ligne,colonne,$1); return;} 
else {$$=NULL; inserer($1,0,-1,1);} $$=insererPos($$,rechercher($1));} 

| Affectation {if (rechercher($1.val)==-1) {if (type == $1.type) {$$=NULL; inserer($1.val,0,-1,1); $$=insererPos($$,rechercher($1.val));} else {printf ("ERREUR SYNTAXIQUE : incompatibilité des types , Ligne : , %d Colonne : %d , Type 1 : %s , Type 2 : %s \n",ligne,colonne,typeToString(type),typeToString($1.type)); return;}} else{printf ("ERREUR SYNTAXIQUE : idf déja déclaré , Ligne : , %d Colonne : %d , Entité : %s \n",ligne,colonne,$1.val); return;}}

| ListeVar ',' Idf {if (rechercher($3)!=-1) {printf("ERREUR SEMANTIQUE : IDF DEJA DECLARE : %s\n",$3); return;} else {inserer($3,0,-1,1); $$=insererPos($$,rechercher($3));}}

 
| ListeVar ',' Affectation {if (rechercher($3.val)==-1) {if (type == $3.type) {inserer($3.val,0,-1,1); $$=insererPos($$,rechercher($3.val));} else {printf ("ERREUR SEMANTIQUE : INCOMPATIBILITE DES TYPES\n"); return;}} else{printf("ERREUR SEMANTIQUE : IDF DEJA DECLARE : %s\n",$3.val); return;}}
;



DeclarationInit : ',' Affectation {if (rechercher($2.val)==-1) {inserer($2.val,0,$2.type,1);} else{ {printf ("ERREUR SEMANTIQUE : Idf non déclaré , Ligne : , %d Colonne : %d , Entité : %s \n",ligne,colonne,$2.val); return;}}} 
| DeclarationInit ',' Affectation {if (rechercher($3.val)==-1) {inserer($3.val,0,$3.type,1);} else{ {printf ("ERREUR SEMANTIQUE : Idf non déclaré , Ligne : , %d Colonne : %d , Entité : %s \n",ligne,colonne,$3.val); return;}}}
;

DeclarationStr : Type Idf '[' Entier ']' {if (rechercher($2)!=-1) {printf ("ERREUR SEMANTIQUE : idf déja déclaré , Ligne : , %d Colonne : %d , Entité : %s \n",ligne,colonne,$2); return;} else {inserer($2,2,type,$4);}} 
;


Valeur : Entier {$$.type=0;  char t[10]; sprintf(t,"%d",$1); $$.val=strdup(t);}
| Reel  {$$.type=1;  char t[10]; sprintf(t,"%d",$1); $$.val=strdup(t);}
| Caractere {$$.type=2;  char t[10]; sprintf(t,"%d",$1); $$.val=strdup(t);}
;




Affectation : Idf '=' Operation1 {$$.val = $1; $$.type = $3.type;
				teteQuad=insererQuad(teteQuad,numQuad,"=",$3.val," ",$1);
           			numQuad++;}
;



Operation1: Operation1 '+' Operation2 {if ($1.type!=$3.type) {printf ("ERREUR SEMANTIQUE : incompatibilité des types , Ligne : , %d Colonne : %d , Type 1 : %s , Type 2 : %s \n",ligne,colonne,typeToString($1.type),typeToString($3.type)); return;} 
                           		else {$$.type=$1.type;}
						char t[10]; sprintf(t,"T%d",numT); 
						numT++;
						teteQuad=insererQuad(teteQuad,numQuad,"+",$1.val,$3.val,t);
                                		numQuad++;
						$$.val=strdup(t);}

| Operation1 '-' Operation2 {if ($1.type!=$3.type) {printf ("ERREUR SEMANTIQUE : incompatibilité des types , Ligne : , %d Colonne : %d , Type 1 : %s , Type 2 : %s \n",ligne,colonne,typeToString($1.type),typeToString($3.type)); return;} 
                           else {$$.type=$1.type;}
				char t[10]; sprintf(t,"T%d",numT); 
				numT++;
				teteQuad=insererQuad(teteQuad,numQuad,"-",$1.val,$3.val,t);
				numQuad++;
				$$.val=strdup(t);}

| Operation2 {$$.val=$1.val;}
;

Operation2: Operation2 '*' Operation3 {if ($1.type!=$3.type) {printf ("ERREUR SEMANTIQUE : incompatibilité des types , Ligne : , %d Colonne : %d , Type 1 : %s , Type 2 : %s \n",ligne,colonne,typeToString($1.type),typeToString($3.type)); return;} 
                           		else {$$.type=$1.type;}
						char t[10]; sprintf(t,"T%d",numT); 
						numT++;
						teteQuad=insererQuad(teteQuad,numQuad,"*",$1.val,$3.val,t);
						numQuad++;
						$$.val=strdup(t);}

| Operation2 '/' Operation3 {if ($1.type!=$3.type) {printf ("ERREUR SEMANTIQUE : incompatibilité des types , Ligne : , %d Colonne : %d , Type 1 : %s , Type 2 : %s \n",ligne,colonne,typeToString($1.type),typeToString($3.type)); return;} 
                           else {$$.type=$1.type;}
				char t[10]; sprintf(t,"T%d",numT); 
				numT++;
				teteQuad=insererQuad(teteQuad,numQuad,"/",$1.val,$3.val,t);
				numQuad++;
				$$.val=strdup(t);}

| Operation3 {$$.val=$1.val;}
;

Operation3: Valeur {$$.type=$1.type; $$.val=$1.val;}
| Idf {if (rechercher($1)==-1) {printf ("ERREUR SEMANTIQUE : idf non déclaré , Ligne : , %d Colonne : %d , Entité : %s \n",ligne,colonne,$1); return;} $$.type=getType($1); $$.val=strdup($1);} 
| '(' Operation1 ')' {$$.type=$2.type; $$.val=$2.val;}

;
 


OperateurComp: '<' {$$=strdup("<");}  | '>' {$$=strdup(">");}  | Eg {$$=strdup("==");}  | NonEg {$$=strdup("!=");}  | InfEg {$$=strdup("<=");}  | SupEg {$$=strdup(">=");} 
;


ConditionIf : If Condition M ':'  Tab BlocInst NonTab {RaccorderUnElement ($2.vrai,$3); $$=FusionnerUnElement ($6,$2.faux);} 
| If Condition M ':' Tab BlocInst NonTab N M Else ':' Tab BlocInst NonTab {RaccorderUnElement ($2.vrai,$3); RaccorderUnElement ($2.faux,$9); $$=Fusionner($8,$13);} 
| If Condition M ':' Tab BlocInst NonTab N M BlocElif N M Else ':' Tab BlocInst NonTab {RaccorderUnElement ($2.vrai,$3); RaccorderUnElement ($2.faux,$9); $$=NULL; $$=Fusionner($$,$11); $$=Fusionner($$,$8);  $$=Fusionner($$,$10);};

BlocElif : Elif Condition M ':' Tab BlocInst NonTab {RaccorderUnElement ($2.vrai,$3); RaccorderUnElement ($2.faux,numQuad+1); $$=NULL;}
| BlocElif N M Elif Condition M ':' Tab BlocInst NonTab {RaccorderUnElement ($5.vrai,$6); RaccorderUnElement ($5.faux,numQuad+1); $$=NULL; $$=Fusionner($1,$2);}
;


Condition : '(' Operation1 OperateurComp Operation1 ')' {teteQuad=insererQuad(teteQuad,numQuad,$3,$2.val,$4.val," "); $$.vrai=numQuad; numQuad++;
							teteQuad=insererQuad(teteQuad,numQuad,"BR"," "," "," "); $$.faux=numQuad; numQuad++; }
;

M: { $$ = numQuad;}
;

N: { teteQuad=insererQuad(teteQuad,numQuad,"BR"," "," "," "); $$=NULL; $$=insererRacFus($$,numQuad); numQuad++;  }
;


BlocInst : InstructionTab {$$=$1;} | InstructionTab Suite BlocInst {$$=$1;}
; 

InstructionTab : Instruction {$$=$1; Raccorder($1,numQuad);}
| Affectation {if (rechercher($1.val)==-1) {inserer($1.val,1,$1.type,1);} else {if (getType($1.val) != $1.type) {printf ("ERREUR SEMANTIQUE : incompatibilité des types , Ligne : , %d Colonne : %d , Type 1 : %s , Type 2 : %s \n",ligne,colonne,typeToString(getType($1.val)),typeToString($1.type)); return;}} $$=NULL;}
;

%%
int yyerror(char* msg)
{msg = "ERREUR SYNTAXIQUE : ";
printf("%sLigne : %d , Colonne : %d",msg,ligne,colonne); 
return 1;
}

int yywrap(void)
{
	return 1;
}

int main()
{
src = 1;
yyin=fopen("code.txt","r");
yyout = fopen("code2.txt","w");
yylex();
fclose(yyin);
fclose(yyout);

src = 2;
initialiserTS();
yyin = fopen("code2.txt","r");
yyparse();
fclose(yyin);

printf("\n\nTABLE DES SYMBOLES :\n\n");
afficher();

printf("\n\nQUADRUPLETS :\n\n");
afficherQuad(teteQuad);

printf("\n\nCODE MACHINE :\n\n");
afficherCodeMachine(teteQuad);

printf("\n\nOPTIMISATION :\n\n");
teteQuadOpt=Optimiser(teteQuad);
afficherQuad(teteQuadOpt);



printf("\n\nCODE MACHINE OPTIMISE :\n\n");
afficherCodeMachine(teteQuadOpt);

return 0;
}
