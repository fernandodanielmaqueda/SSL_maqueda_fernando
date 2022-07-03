/* Desarrollado para FLEX versión 2.5.4a */

/*
UTN - FRBA
Sintaxis y Semántica de los Lenguajes - 2022
Trabajo Práctico n° 2 - FLEX para reconocimiento de categorías léxicas de C

    Implementar en C un programa que permita reconocer en un archivo.c de entrada todas las categorías léxicas de C, comentarios. El mismo deberá dar como salida en pantalla un reporte. Para desarrollar dicho programa deberá utilizar LEX/FLEX para la generación del código C que implemente un analizador léxico que cumpla con los requisitos
    
    Categorías léxicas de C a reconocer:
        * Constantes (Entera decimal, entera octal, entera hexadecimal, real, caracter)
        * Literal cadena
        * Palabras reservadas (distinguir entre tipo de dato, estructuras de control y otros)
        * Identificadores 
        * Caracteres de puntuación/operadores de C.

    Comentarios (en realidad son reconocidos por el PREPROCESADOR)
        * Comentarios de una línea
        * Comentarios de múltiples líneas

    El reporte deberá contener lo siguiente:
    Reporte
        * Listado de identificadores encontrados indicando la cantidad de veces que aparece cada uno de ellos. El listado debe estar ordenado alfabéticamente.
        * Listado de literales cadena encontrados indicando la longitud de los mismos y ordenados por longitud ascendente. En caso de igual longitud no importa el orden. Si un literal cadena aparece más de una vez, no volverlo a agregar a la lista.
        * Listado de palabras reservadas en el orden en el que han aparecido en el archivo. Si una palabra reservada aparece de forma repetida, se debe agregar a la lista por cada vez que aparece.
        * Listado de constantes indicando según su tipo:
            -> Para las constantes octales indicar su valor entero decimal.
            -> Para las constante hexadecimales indicar u valor entero decimal.
            -> Para las constantes decimales indicar el valor de cada una y el total acumulado de sumar todas ellas.
            -> Para las constantes reales indicar el valor sde su mantisa y parte entera.
            -> Para las constantes carácter, enumerarlas según orden de aparición. 
        * Listado de operadores/caracteres de puntuación indicando cantidad de veces que aparecen.
        * Listado de comentarios encontrados distinguiendo si se trata de comentarios de una línea o múltiples líneas.
        * Listado de cadenas y/o caracteres no reconocidos indicando el número de línea donde se encontraron.

    La entrega de este trabajo práctico es obligatoria, su fecha límite para consulta, entrega y revisión es el día Domingo 17 de Julio. Luego de esa fecha, no se aceptarán más trabajos.
    El entorno de programación queda a criterio de cada grupo de trabajo (Eclipse, Dev, Codeblocks, Visual Studio Code ). Se recomienda un IDE que esté integrado con Git para poder realizar el trabajo en equipo de una forma más práctica.
    Formará parte de la entrega, como mínimo, el archivo del analizador léxico (.l), archivo de funciones desarrolladas (.h o .c), archivo fuente del analizador léxico generado por lex-flex (.c) y el archivo ejecutable (.exe). Todo debe estar en el repositorio GitHub.
    La entrega será a través del repositorio de GitHub en la carpeta correspondiente a cada TP.
    Las consultas podrán ser respondidas a través Discord. Es importante que los utilices para compartir sus dudas con el resto de los compañeros.
*/

/*
    Notas sobre el uso de FLEX:

    Prioridad/Precedencia de operadores en las ERX:
        << Mayor prioridad / Precedencia más alta >>
            \ (una secuencia de escape)
                \n (para secuencias de escape simples hace una interpretación ANSI-C (en este caso reconoce un "salto de línea"))
                \0 (para secuencias de escape octales hace una interpretación ANSI-C (en este caso reconoce un caracter NUL (código ASCII 0)))
                \x2a (para secuencias de escape hexadecimales hace una interpretación ANSI-C (en este caso reconoce el caracter con valor hexadecimal 2a)))
                \\n (en este caso reconoce la ER \n)
                \* (en este caso reconoce la ER *)
            x (caracteres/símbolos, excepto los reservados para los metacaracteres/operadores de la ERX (en este caso la ER x)) 
            [] (describe un conjunto/clase de caracteres, simplifica el uso del operador "unión" (+ en ER y | en ERX). Dentro de una clase de caracteres, todos los operadores de ERX pierden su significado especial a excepción del caracter de escape (\), el operador ^ cambia su significado y se agregan los operador - y :):
                [xyz] (una "clase de caracteres" (en este caso representa la ER x+y+z))
                [abj-oZ] (una "clase de caracteres" con un rango/intervalo (en este caso representa la ER a+b+j+k+l+m+n+o+Z)
                [^A-Z] (una "clase de caracteres" negada (en este caso cualquier caracter EXCEPTO una letra mayúscula). También emparejará una línea nueva (\n) a menos que \n (o una secuencia de escape equivalente) sea uno de los caracteres presentes explícitamente en la clase de caracteres negada. Sólo puede aparecer al principio de la "clase de caracteres", sino pierde sus propiedades especiales y es tratado como un caracter normal.)
                [:alnum:] [:alpha:] [:blank:] [:cntrl:] [:digit:] [:graph:] [:lower:] [:print:] [:punct:] [:space:] [:upper:] [:xdigit:] (expresiones de clases de caracteres, son expresiones encerradas entre los delimitadores [: y :] que también deben aparecer entre el [ y el ] de la clase de caracteres, y además pueden darse otros elementos dentro de la clase de caracteres. Por ejemplo, las siguientes ERX son equivalentes: [[:alnum:]] = [[:alpha:][:digit]] = [[:alpha]0-9] = [a-zA-Z0-9])
            "" (un literal cadena. Dentro de un literal cadena, todos los operadores de ERX pierden su significado especial (incluyendo también a la clase de caracteres []), con excepción del caracter de escape (\).)
                "*""\\n"\""qwe" (en este caso reconoce la ER *\nqwe")
            {nombre} (la expansión de la definición de "nombre" (que debe haber sido definido previamente en la sección de definiciones de FLEX). La ERX para los nombres de las definiciones de FLEX es la misma que la de identificadores de C: [_a-zA-Z][_a-zA-Z0-9]*)
            . (se corresponde con cualquier caracter (byte) excepto el "línea nueva" (\n))
            () (se utilizan para agrupar/emparejar una ER y anular la precedencia, por ejemplo: (r(s|t))+ )
            r* (operador clausura de Kleene: cero o más ocurrencias de la ER que lo precede)
            r+ (operador clausura positiva: una o más ocurrencias de la ER que lo precede)
            r? (operador que indica cero o una ocurrencia de la ER que lo precede (es decir, "una ER opcional"))
            {} (operador potencia, una repetición determinada del patrón que lo precede como operando)
                r{2,5} (es el operador potencia extendido a un intervalo cerrado (en este caso significa de dos a cinco r's, representando la ER rr+rrr+rrrr+rrrrr))
                r{2,} (es el operador potencia extendido a un intervalo infinito (en este caso significa dos o más r's, representando la ER rr+rrr+rrrr+rrrrr+...))
                r{4} (en este caso significa exactamente 4 r's, representando la ER rrrr)
            rs (patrones; se denomina "concatenación")
            r|s (es el operador unión de ERs (en este caso representa la ER r+s))
            r/s (patrón "de contexto posterior": reconoce la ER r pero sólo si va seguida por la ER s. El texto emparejado por la ER s se incluye cuando se determina si esta regla es el "emparejamiento más largo", pero se devuelve entonces a la entrada antes que se ejecute la acción, así que la acción sólo ve el texto emparejado por la ER r. (Nota: hay algunas combinaciones de r/s que FLEX no puede emparejar correctamente, ver Capitulo 21 [Deficiencias / Errores], página 50, las notas acerca del "contexto posterior peligroso", del manual de Flex, versión 2.5). Una regla puede tener sólo una instancia del contexto posterior (de los operadores / y $))
            ^r (la ER r, pero sólo al comienzo de una línea (es decir, justo al comienzo del análisis o a la derecha después de que se haya analizado una línea nueva (\n)). Sólo puede aparecer al principio del patrón, sino pierde sus propiedades especiales y es tratado como un caracter normal.)
            r$ (la ER r, pero sólo al final de una línea (es decir, justo antes de una línea nueva (\n)). Equivalente a r/\n. Sólo puede aparecer al final del patrón, sino pierde sus propiedades especiales y es tratado como un caracter normal.  Una regla puede tener sólo una instancia del contexto posterior (de los operadores / y $))
            <<EOF>> (un end-of-file (fin-de-fichero). Sólo puede combinarse con condiciones de arranque, por ejemplo r<<EOF>> y <<EOF>>r no son válidos, y además no puede agruparse dentro de paréntesis.)
                <s><<EOF>> un end-of-file en una condición de arranque s
                <s1,s2><<EOF>> un end-of-file en una condición de arranque s1 ó s2
            <condiciones,de,arranque>r (condiciones de arranque (ver Capítulo 9 [Condiciones de arranque], página 16, del manual de Flex, versión 2.5). Las condiciones de arranque sólo pueden aparecer al principio de un patrón, y no pueden agruparse dentro de paréntesis.)
                <s>r (una ER r, pero sólo en la condición de arranque s)
                <s1,s2,s3>r (una ER r, pero en cualquiera de las condiciones de arranque s1, s2, ó s3)
                <*>r (una ER r en cualquier condición de arranque, incluso en una exclusiva)
        << Menor prioridad / Precedencia más baja >>

    atof y atoi

    return 0; es equivalente a yyterminate;
*/

/* Con esta opción la función de análisis int yylex(void); se comporta como si en la sección de usuario existiera la función int yywrap(void) return 1; la cual devuelve verdadero (no-cero), haciendo que una vez que la función de análisis int yylex(void) reciba una indicación de EOF desde YY_INPUT, asuma que no hay nada más para analizar de yyin y finalice (haciendo un return 0;) */
%option noyywrap

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "funciones.h"
#define archivo_yyin "archivo.c"
#define archivo_yyout "yyout.txt"

%}
/* Condiciones de arranque inclusivas */
/* %s */
/* Condiciones de arranque exclusivas */
%x constanteEnteraDecimal constanteEnteraOctal constanteEnteraHexa constanteReal constanteCaracter literalCadena identificador comentarioDeMultiplesLineas comentarioDeUnaLinea tokenNoReconocido

/* Inicio de la sección de definiciones */

signo [+-]
digito [0-9]
digitoNoCero [1-9]
digitoOctal [0-7]
digitoHexa [0-9a-fA-F]
noDigito [_a-zA-Z]
parteExponente [eE]{signo}?{digito}+
sufijoEntero [uU]|[lL]|[uU][lL]|[lL][uU]
sufijoReal [fF]|[lL]
secuenciaEscapeHexa \\x{digitoHexa}{1,2}
secuenciaEscapeOctal \\{digitoOctal}{1,3}
secuenciaEscapeSimple \\[abefnrtv"\\"'"?"\"]
caracteresDePuntuacionYOperadores "<<="|>>=|"..."|"->"|"++"|"--"|"<<"|>>|"<="|>=|==|"!="|&&|"||"|"*="|"/="|"%="|"+="|"-="|&=|"^="|"|="|##|"["|"]"|"("|")"|"{"|"}"|"."|&|"*"|"+"|"-"|~|"!"|"/"|"%"|"<"|>|"^"|"|"|=|"?"|:|;|,|#

/* Fin de la sección de definiciones */

/* Inicio de la sección de reglas */
%%
    int linea_actual = 1; int columna_actual = 0;
    char* buffer;

    /* Constantes */
        /* Entera decimal */
{digitoNoCero}{digito}*{sufijoEntero}?/{caracteresDePuntuacionYOperadores}|"//"|"/*"|" "|\t|\n {columna_actual += yyleng; printf ("Reconocida: constante entera decimal: %s \n", yytext);}
{digitoNoCero}{digito}*{sufijoEntero}? {columna_actual += yyleng; inicializarString(&buffer, yytext); BEGIN(constanteEnteraDecimal);}
<constanteEnteraDecimal>{
    <<EOF>> {columna_actual -= strlen(buffer); printf ("Reconocida: constante entera decimal: %s \n", buffer); free(buffer); return 0;}
    [^<>.\-+=!&|*/%^#\[\]\(\)\{\}~?:;, \t\n]+ {columna_actual -= strlen(buffer); concatenarString (&buffer, yytext); printf ("%s:%d:%d: Error lexico: token no reconocido: %s \n", archivo_yyin, linea_actual, columna_actual, buffer); free(buffer); BEGIN(INITIAL);}
}
        /* Entera octal */
0{digitoOctal}*{sufijoEntero}?/{caracteresDePuntuacionYOperadores}|"//"|"/*"|" "|\t|\n {columna_actual += yyleng; printf ("Reconocida: constante entera octal: %s \n", yytext);}
0{digitoOctal}*{sufijoEntero}? {columna_actual += yyleng; inicializarString(&buffer, yytext); BEGIN(constanteEnteraOctal);}
<constanteEnteraOctal>{
    <<EOF>> {columna_actual -= strlen(buffer); printf ("Reconocida: constante entera octal: %s \n", buffer); free(buffer); return 0;}
    [^<>.\-+=!&|*/%^#\[\]\(\)\{\}~?:;, \t\n]+ {columna_actual -= strlen(buffer); concatenarString (&buffer, yytext); printf ("%s:%d:%d: Error lexico: token no reconocido: %s \n", archivo_yyin, linea_actual, columna_actual, buffer); free(buffer); BEGIN(INITIAL);}
}
        /* Entera hexadecimal */
0[xX]{digitoHexa}+{sufijoEntero}?/{caracteresDePuntuacionYOperadores}|"//"|"/*"|" "|\t|\n {columna_actual += yyleng; printf ("Reconocida: constante entera hexadecimal: %s \n", yytext);}
0[xX]{digitoHexa}+{sufijoEntero}? {columna_actual += yyleng; inicializarString(&buffer, yytext); BEGIN(constanteEnteraHexa);}
<constanteEnteraHexa>{
    <<EOF>> {columna_actual -= strlen(buffer); printf ("Reconocida: constante entera hexadecimal: %s \n", buffer); free(buffer); return 0;}
    [^<>.\-+=!&|*/%^#\[\]\(\)\{\}~?:;, \t\n]+ {columna_actual -= strlen(buffer); concatenarString (&buffer, yytext); printf ("%s:%d:%d: Error lexico: token no reconocido: %s \n", archivo_yyin, linea_actual, columna_actual, buffer); free(buffer); BEGIN(INITIAL);}
}
        /* Real */
(((({digito}*"."{digito}+)|({digito}+".")){parteExponente}?)|({digito}+{parteExponente})){sufijoReal}?/"<<="|>>=|"->"|"++"|"--"|"<<"|>>|"<="|>=|==|"!="|&&|"||"|"*="|"/="|"%="|"+="|"-="|&=|"^="|"|="|##|"["|"]"|"("|")"|"{"|"}"|&|"*"|"+"|"-"|~|"!"|"/"|"%"|"<"|>|"^"|"|"|=|"?"|:|;|,|#|"//"|"/*"|" "|\t|\n {columna_actual += yyleng; printf ("Reconocida: constante real: %s \n", yytext);}
(((({digito}*"."{digito}+)|({digito}+".")){parteExponente}?)|({digito}+{parteExponente})){sufijoReal}? {columna_actual += yyleng; printf ("Reconocida: constante real: %s \n", yytext);}
<constanteReal>{
    <<EOF>> {columna_actual -= strlen(buffer); printf ("Reconocida: constante real: %s \n", buffer); free(buffer); return 0;}
    [^<>.\-+=!&|*/%^#\[\]\(\)\{\}~?:;, \t\n]+ {columna_actual -= strlen(buffer); concatenarString (&buffer, yytext); printf ("%s:%d:%d: Error lexico: token no reconocido: %s \n", archivo_yyin, linea_actual, columna_actual, buffer); free(buffer); BEGIN(INITIAL);}
}
        /* Caracter */
' {columna_actual += yyleng; BEGIN(constanteCaracter);}
<constanteCaracter>{
    {secuenciaEscapeHexa}' {columna_actual += yyleng; printf ("Reconocida: constante caracter con secuencia de escape hexadecimal: '%s \n", yytext); BEGIN(INITIAL);}
    {secuenciaEscapeOctal}' {columna_actual += yyleng; printf ("Reconocida: constante caracter con secuencia de escape octal: '%s \n", yytext); BEGIN(INITIAL);}
    {secuenciaEscapeSimple}' {columna_actual += yyleng; printf ("Reconocida: constante caracter con secuencia de escape simple: '%s \n", yytext); BEGIN(INITIAL);}
    [^\\'\n]' {columna_actual += yyleng; printf ("Reconocida: constante caracter simple: '%s \n", yytext); BEGIN(INITIAL);}
    [^'\n]*' {printf ("%s:%d:%d: Error lexico: constante caracter invalida \n", archivo_yyin, linea_actual, columna_actual); return 1;}
    [^'\n]+ |
    \n |
    <<EOF>> {printf ("%s:%d:%d: Error sintactico: falta el caracter ' de cierre de la constante caracter \n", archivo_yyin, linea_actual, columna_actual); return 1;}
}
    /* Literal cadena */
\" {columna_actual += yyleng; BEGIN(literalCadena);}
<literalCadena>{
    ({secuenciaEscapeHexa}|{secuenciaEscapeOctal}|{secuenciaEscapeSimple}|[^\"\n\\])*\" {columna_actual += yyleng; printf ("Reconocido: literal cadena: \"%s \n", yytext); BEGIN(INITIAL);}
    [^\"\n]*\" {printf ("%s:%d:%d: Error lexico: literal cadena invalido \n", archivo_yyin, linea_actual, columna_actual); return 1;}
    [^\"\n]+ |
    \n |
    <<EOF>> {printf ("%s:%d:%d: Error sintactico: falta el caracter \" de cierre del literal cadena \n", archivo_yyin, linea_actual, columna_actual); return 1;}
}
    /* Palabras reservadas */
        /* Tipo de dato */
char|double|enum|float|int|long|short|struct|union|void {columna_actual += yyleng; printf ("Reconocida: palabra reservada (tipo de dato): %s \n", yytext);}
        /* Estructuras de control */
break|case|continue|default|do|else|for|goto|if|return|switch|while {columna_actual += yyleng; printf ("Reconocida: palabra reservada (estructuras de control): %s \n", yytext);}
        /* Otros */
auto|const|extern|register|signed|sizeof|static|typedef|unsigned|volatile {columna_actual += yyleng; printf ("Reconocida: palabra reservada (otros): %s \n", yytext);}
    /* Identificadores */
{noDigito}({noDigito}|{digito})*/{caracteresDePuntuacionYOperadores}|"//"|"/*"|" "|\t|\n {columna_actual += yyleng; printf ("Reconocido: identificador: %s \n", yytext);}
{noDigito}({noDigito}|{digito})* {columna_actual += yyleng; inicializarString(&buffer, yytext); BEGIN(identificador);}
<identificador>{
    <<EOF>> {columna_actual -= strlen(buffer); printf ("Reconocido: identificador: %s \n", buffer); free(buffer); return 0;}
    [^<>.\-+=!&|*/%^#\[\]\(\)\{\}~?:;, \t\n]+ {columna_actual -= strlen(buffer); concatenarString (&buffer, yytext); printf ("%s:%d:%d: Error lexico: token no reconocido: %s \n", archivo_yyin, linea_actual, columna_actual, buffer); free(buffer); BEGIN(INITIAL);}
}
    /* Caracteres de puntuación/operadores de C */
{caracteresDePuntuacionYOperadores} {columna_actual += yyleng; printf ("Reconocido: caracter de puntuacion/operador de C: %s \n", yytext);}

    /* Comentarios */
        /* Comentarios de una línea */
"//" {BEGIN(comentarioDeUnaLinea);}
<comentarioDeUnaLinea>{
    [^\n]+ {printf("Reconocido: comentario de una linea: //%s \n", yytext);}
    \n {++linea_actual; columna_actual = 0; BEGIN(INITIAL);}
}
        /* Comentarios de múltiples líneas */
"/*" {columna_actual += yyleng; inicializarString(&buffer, "/*"); BEGIN(comentarioDeMultiplesLineas);}
<comentarioDeMultiplesLineas>{
    [^"*/"\n]+ {columna_actual += yyleng; concatenarString(&buffer, yytext);}
    \n+ {linea_actual += yyleng; columna_actual = 0; concatenarString(&buffer, yytext);}
    "*/" {columna_actual += yyleng; concatenarString (&buffer, yytext); printf("Reconocido: comentario de multiples lineas: %s \n", buffer); free(buffer); BEGIN(INITIAL);}
    <<EOF>> {columna_actual -= strlen(buffer); printf ("%s:%d:%d: Error sintactico (para el preprocesamiento): comentario multilinea sin terminar \n", archivo_yyin, linea_actual, columna_actual); free(buffer); return 1;}
}

[^ \t\n]/({caracteresDePuntuacionYOperadores}|"//"|"/*"|" "|\t|\n) {printf ("%s:%d:%d: Error lexico: token no reconocido: %s \n", archivo_yyin, linea_actual, columna_actual, yytext); columna_actual += yyleng;}
[^ \t\n] {inicializarString(&buffer, yytext); BEGIN(tokenNoReconocido);}
<tokenNoReconocido>{
    <<EOF>> {printf ("%s:%d:%d: Error lexico: token no reconocido: %s \n", archivo_yyin, linea_actual, columna_actual, buffer); free(buffer); return 1;}
    [^<>.\-+=!&|*/%^#\[\]\(\)\{\}~?:;, \t\n]+ {concatenarString (&buffer, yytext); printf ("%s:%d:%d: Error lexico: token no reconocido: %s \n", archivo_yyin, linea_actual, columna_actual, buffer); free(buffer); BEGIN(INITIAL);}
    [^0-9'"_a-zA-Z<>.\-+=!&|*/%^#\[\]\(\)\{\}~?:;, \t\n] ;
}
[ \t]+ {columna_actual += yyleng;}
[\n]+ {linea_actual += yyleng; columna_actual = 0;}

%%
/* Fin de la sección de reglas */

/* Inicio de la sección de código de usuario */

int main() {
    printf("\n========================= [ FLEX ] =========================\n\n");
    yyin = fopen(archivo_yyin, "r");
    yyout = fopen(archivo_yyout, "w");	
    yylex();
    fclose(yyin);
    fclose(yyout);
    printf("\n========================= [ Terminado ] =========================\n\n");
    printf("\n========================= [ Reporte ] =========================\n\n");
    printf("* Listado de identificadores encontrados:\n");
    printf("* Listado de literales cadena encontrados:\n");
    printf("* Listado de palabras reservadas por orden de aparicion:\n");
    printf("* Listado de constantes enteras octales:\n");
    printf("* Listado de constantes enteras hexadecimales:\n");
    printf("* Listado de constantes enteras decimales:\n");
    printf("* Listado de constantes reales:\n");
    printf("* Listado de constantes caracter:\n");
    printf("* Listado de operadores/caracteres de puntuacion:\n");
    printf("* Listado de comentarios:\n");
    printf("* Listado de cadenas y/o caracteres no reconocidos:\n");
    printf("\n========================= [ Terminado ] =========================\n\n");
	system("pause");
    return 0;
}

/* Fin de la sección de código de usuario */