%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "projet.tab.h"

int colonne = 1;
int ligne =1;
extern YYSTYPE yylval;

int last = 1;
int current = 1;
int src;


int numberOf(char s[])
{
	int negatif=1;
	int x=0;
	int y=1;
int z=0;
	
	for (int i=0; i<strlen(s); i++)
		if (s[i] != '(' && s[i] != ')' && s[i] != '-')
			{y = y * 10; z++;}

	y = y /10;
	
	for (int i=0; i<strlen(s); i++)
		if (s[i] == '-') negatif=-1;
		else if (s[i] != '(' && s[i] != ')')
		{
			x = x + ( ( s[i] - 48 ) * y );
			y = y / 10;
		}
	return negatif * x;
}



%}

entier (0|[1-9][0-9]*)|"(-"[1-9][0-9]*")"
reel {entier}"."[0-9]*|"(-"[1-9][0-9]*"."[0-9]*")"
caractere "'"(.)"'"
idf [A-Z][a-zA-Z0-9_]*[a-zA-Z0-9]
commentaire "#"(.)*




%%
"int" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("int type %s\n",yytext); colonne=colonne+yyleng; return Int;}}
"float" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("float type %s\n",yytext); colonne=colonne+yyleng; return Float;}}
"char" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("char type %s\n",yytext); colonne=colonne+yyleng; return Char;}}
"if" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("if %s\n",yytext); colonne=colonne+yyleng; return If;}}
"elif" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("elif %s\n",yytext); colonne=colonne+yyleng; return Elif;}}
"else" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("else %s\n",yytext); colonne=colonne+yyleng; return Else;}}
"for" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("for %s\n",yytext); colonne=colonne+yyleng; return For;}}
"in" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("in %s\n",yytext); colonne=colonne+yyleng; return In;}}
"range" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("range %s\n",yytext); colonne=colonne+yyleng; return Range;}}
"while" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("while %s\n",yytext); colonne=colonne+yyleng; return While;}}


"," {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("virgule %s\n",yytext); colonne=colonne+yyleng; return ',';}}
"=" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("affectation %s\n",yytext); colonne=colonne+yyleng; return '=';}}
"[" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("crochet %s\n",yytext); colonne=colonne+yyleng; return '[';}}
"]" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("crochet %s\n",yytext); colonne=colonne+yyleng; return ']';}}
"(" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("parenthese %s\n",yytext); colonne=colonne+yyleng; return '(';}}
")" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("parenthese %s\n",yytext); colonne=colonne+yyleng; return ')';}}
":" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("deux points %s\n",yytext); colonne=colonne+yyleng; return ':';}}
"+" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("addition %s\n",yytext); colonne=colonne+yyleng; return '+';}}
"-" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("soustraction %s\n",yytext); colonne=colonne+yyleng; return '-';}}
"*" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("multiplication %s\n",yytext); colonne=colonne+yyleng; return '*';}}
"/" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("division %s\n",yytext); colonne=colonne+yyleng; return '/';}}
"<" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("comparaison %s\n",yytext); colonne=colonne+yyleng; return '<';}}
">" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("comparaison %s\n",yytext); colonne=colonne+yyleng; return '>';}}
"==" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("comparaison %s\n",yytext); colonne=colonne+yyleng; return Eg;}}
"!=" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("comparaison %s\n",yytext); colonne=colonne+yyleng; return NonEg;}}
"<=" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("comparaison %s\n",yytext); colonne=colonne+yyleng; return InfEg;}}
">=" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("comparaison %s\n",yytext); colonne=colonne+yyleng; return SupEg;}}
"and" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("et %s\n",yytext); colonne=colonne+yyleng; return And;}}
"or" {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("ou %s\n",yytext); colonne=colonne+yyleng; return Or;}}

{idf} {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {if (yyleng<=8) {printf ("idf %s\n",yytext); yylval.chaine=strdup(yytext); return Idf;} else {printf ("ERREUR LEXICALE : taille IDF trop longue , Ligne : %d , Colonne : %d , Entité : %s \n",ligne,colonne,yytext); colonne=colonne+yyleng; return;}}}
{entier} {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {if (numberOf(yytext)>=-32768 && numberOf(yytext)<=32767) {printf ("Entier %s\n",yytext);  yylval.entier=numberOf(yytext); return Entier;} else {printf ("ERREUR LEXICALE : valeure d'entier trop longue , Ligne : %d , Colonne : %d , Entité : %s \n",ligne,colonne,yytext); return;}colonne=colonne+yyleng;}}
{reel} {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("Reel %s\n",yytext); colonne=colonne+yyleng; yylval.entier=atof(yytext); return Reel;}}
{caractere} {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("Caractère %s\n",yytext); colonne=colonne+yyleng; yylval.chaine=strdup(yytext); return Caractere;}}
{commentaire} {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("commentaire\n"); colonne=colonne+yyleng;}}


"\n"+"\t"+ 	{if (src==1)
		{	char s[10]; sprintf(s,"\n"); for(int i=0;i<strlen(yytext);i++) if(yytext[i]=='\t') sprintf(s,"%s%c",s,yytext[i]); yyleng=strlen(s);
			if (current == 1) {if(yyleng-current>1) {for(int i=0;i<yyleng-current;i++) fprintf(yyout,"{");} current=yyleng; fprintf(yyout,"\n{");}
			else
			{
				last=current;
				current=yyleng;
				if (current == last) {fprintf(yyout,"\n|");}
				if (current < last) {while(current<last) {last--; fprintf(yyout,"}");}  fprintf(yyout,"%s","\n");}
				if (current > last) {if(current-last>1) {for(int i=0;i<yyleng-current;i++) fprintf(yyout,"{");} fprintf(yyout,"\n{");}
			} 
		}else{ligne++; colonne=yyleng;}}


"\n"+ {if (src==1) {if(current>1){while(current>1){fprintf(yyout,"}"); current--;} last=1;}  fprintf(yyout,"%s","\n");} else {ligne++; colonne=0; printf ("newligne \n"); return Saut;}}

" " {if (src==1) fprintf(yyout,"%s"," "); else {colonne=colonne+yyleng;}}


"{"|"\n""{" {if (src==2) {printf ("Indentation\n"); return Tab;} else {printf ("ERREUR LEXICALE : entité non roconnue , Ligne : %d , Colonne : %d , Entité : %s \n",ligne,colonne,yytext); ligne++; colonne=yyleng; return;}}
"}"|"}""\n" {if (src==2) {printf ("Indentation supprimée\n"); return NonTab;} else {printf ("ERREUR LEXICALE : entité non roconnue , Ligne : %d , Colonne : %d , Entité : %s \n",ligne,colonne,yytext); colonne=colonne+yyleng; return;}}
"|" {if (src==2) {printf ("Suite\n"); return Suite;} else {printf ("ERREUR LEXICALE : entité non roconnue  Ligne :  %d , Colonne : %d , Entité : %s \n",ligne,colonne,yytext); colonne=colonne+yyleng; return;}}
"\t"+ {if (src==1) fprintf(yyout,"%s","{"); else {colonne=colonne+yyleng;}}
. {if (src==1) fprintf(yyout,"%s",strdup(yytext)); else {printf ("ERREUR LEXICALE : entité non roconnue  Ligne : %d , Colonne : %d , Entité : %s \n",ligne,colonne,yytext); colonne=colonne+yyleng; return;}}

%%




















