%{
	#include <stdlib.h>  
	#include "calc.h"
	#include "mfcalc.tab.h"
	#include <string.h>
	#include "dsa.h"
	#define B 0
%}

NUM [0-9]
ID [A-Za-z_][A-Za-z0-9_]*

%%

{NUM}+"."*{NUM}*	{ yylval.dval = atof(yytext); return NUM;	}

"exit"				{ printf("Hasta la proxima. Se cierra la calculadora\n"); exit(0);           	}

{ID}				{ yylval.idval=strdup(yytext); return ID;}

[+-/*=:\n()^] 		return yytext[0];
     
[ \t]+

%%
