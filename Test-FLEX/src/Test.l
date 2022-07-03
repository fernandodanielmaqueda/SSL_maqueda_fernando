/* Desarrollado para FLEX versión 2.5.4a */

/* Con esta opción la función de análisis int yylex(void); se comporta como si en la sección de usuario existiera la función int yywrap(void) return 1; la cual devuelve verdadero (no-cero), haciendo que una vez que la función de análisis int yylex(void) reciba una indicación de EOF desde YY_INPUT, asuma que no hay nada más para analizar de yyin y finalice (haciendo un return 0;) */
%option noyywrap

%{
// atof atoi
// return 0; es equivalente a yyterminate;
#include <stdio.h>
#include <math.h>


%}
/* Condiciones de arranque inclusivas */
/* %s */
/* Condiciones de arranque exclusivas */
/* %x */

/* Inicio de la sección de definiciones */

/* Fin de la sección de definiciones */

/* Inicio de la sección de reglas */
%%
    int linea_actual = 1;

    /* Reconocer ' -> ' ó "'" ó \' ó ['] ó [\'] */
    /* Reconocer _ -> _ ó "_" ó \_ ó [_] ó [\_] */
    /* Reconocer # -> # ó "#" ó \# ó [#] ó [\#] */
    /* Reconocer & -> & ó "&" ó \& ó [&] ó [\&] */
    /* Reconocer = -> = ó "=" ó \= ó [=] ó [\=] */
    /* Reconocer > -> > ó ">" ó \> ó [>] ó [\>] */
    /* Reconocer @ -> @ ó "@" ó \@ ó [@] ó [\@] */
    /* Reconocer , -> , ó "," ó \, ó [,] ó [\,] */
    /* Reconocer : -> : ó ":" ó \: ó [:] ó [\:] */
    /* Reconocer ; -> ; ó ";" ó \; ó [;] ó [\;] */
    /* Reconocer ` -> ` ó "`" ó \` ó [`] ó [\`] */
    /* Reconocer ´ -> ´ ó "´" ó \´ ó [´] (funciona mal: acepta la regla dos veces) ó [\´] (funciona mal: acepta la regla dos veces) */
    /* Reconocer ¿ -> ¿ ó "¿" ó \¿ ó [¿] (funciona mal: acepta la regla dos veces) ó [\¿] (funciona mal: acepta la regla dos veces) */
    /* Reconocer ¡ -> ¡ ó "¡" ó \¡ ó [¡] (funciona mal: acepta la regla dos veces) ó [\¡] (funciona mal: acepta la regla dos veces) */
    /* Reconocer ¬ -> ¬ ó "¬" ó \¬ ó [¬] (funciona mal: acepta la regla dos veces) ó [\¬] (funciona mal: acepta la regla dos veces) */
    /* Reconocer ° -> ° ó "°" ó \° ó [°] (funciona mal: acepta la regla dos veces) ó [\°] (funciona mal: acepta la regla dos veces) */
    /* Reconocer [ -> [ ó "[" ó \[ ó [\[] */
    /* Reconocer ] -> ] ó "]" ó \] ó [\]] */
    /* Reconocer ~ -> ~ ó "~" ó \~ ó [\~] */

    /* Reconocer [0-9] -> 5 ó "5" ó [5] */
    /* Reconocer [a-zA-Z] (incluídas ñ y Ñ) -> z ó "z" ó [z] */

    /* Reconocer \ -> "\\" ó \\ ó [\\] */
    /* Reconocer " -> "\"" ó \" ó ["] ó [\"] */

    /* Reconocer ! -> "!" ó \! ó [!] ó [\!] */
    /* Reconocer % -> "%" ó \% ó [%] ó [\%] */
    /* Reconocer < -> "<" ó \< ó [<] ó [\<] */
    /* Reconocer  (un espacio) -> " " ó \  ó [ ] ó [\ ]*/
    /* Reconocer / -> "/" ó \/ ó [/] ó [\/] */
    /* Reconocer * -> "*" ó \* ó [*] ó [\*] */
    /* Reconocer + -> "+" ó \+ ó [+] ó [\+] */
    /* Reconocer ? -> "?" ó \? ó [?] ó [\?] */
    /* Reconocer $ -> "$" ó \$ ó [$] ó [\$] */
    /* Reconocer ( -> "(" ó \( ó [(] ó [\(] */
    /* Reconocer ) -> ")" ó \) ó [)] ó [\)] */
    /* Reconocer { -> "{" ó \{ ó [{] ó [\{] */
    /* Reconocer } -> "}" ó \} ó [}] ó [\}] */
    /* Reconocer . -> "." ó \. ó [.] ó [\.] */
    /* Reconocer | -> "|" ó \| ó [|] ó [\|] */
    /* Reconocer ¨ -> "¨" ó \¨ ó [\¨] (funciona mal: acepta la regla dos veces) */
    /* Reconocer ^ -> "^" ó \^ ó [\^] */
    /* Reconocer - -> "-" ó \- ó [\-] */

    /* Reconoce saltos de línea */
[\n]+ printf("\\n \n"); linea_actual += yyleng;
[a-z]+|. {printf ("Error: token no reconocido en la linea %d: %s \n", linea_actual,yytext); return 1;}



%%
/* Fin de la sección de reglas */

/* Inicio de la sección de código de usuario */

int main() {
   
    yyin = fopen("archivo.c", "r");
    yyout = fopen("yyout.txt", "w");	
    yylex();
    fclose(yyin);
    fclose(yyout);
	system("pause");
    return 0;
}

/* Fin de la sección de código de usuario */