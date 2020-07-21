bison -d projet.y
flex projet.l
gcc lex.yy.c projet.tab.c -lfl -ly -o exe
