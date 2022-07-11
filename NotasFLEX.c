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
                    "*""\\n"\""{q}?[we].(r)+/$" (en este caso reconoce la ER *\n"{q}?[we].(r)+/$)
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

        Reconocer caracteres especiales
            Reconocer ' -> ' ó "'" ó \' ó ['] ó [\']
            Reconocer _ -> _ ó "_" ó \_ ó [_] ó [\_]
            Reconocer # -> # ó "#" ó \# ó [#] ó [\#]
            Reconocer & -> & ó "&" ó \& ó [&] ó [\&]
            Reconocer = -> = ó "=" ó \= ó [=] ó [\=]
            Reconocer > -> > ó ">" ó \> ó [>] ó [\>]
            Reconocer @ -> @ ó "@" ó \@ ó [@] ó [\@]
            Reconocer , -> , ó "," ó \, ó [,] ó [\,]
            Reconocer : -> : ó ":" ó \: ó [:] ó [\:]
            Reconocer ; -> ; ó ";" ó \; ó [;] ó [\;]
            Reconocer ` -> ` ó "`" ó \` ó [`] ó [\`]
            Reconocer ´ -> ´ ó "´" ó \´ ó [´] (funciona mal: acepta la regla dos veces) ó [\´] (funciona mal: acepta la regla dos veces)
            Reconocer ¿ -> ¿ ó "¿" ó \¿ ó [¿] (funciona mal: acepta la regla dos veces) ó [\¿] (funciona mal: acepta la regla dos veces)
            Reconocer ¡ -> ¡ ó "¡" ó \¡ ó [¡] (funciona mal: acepta la regla dos veces) ó [\¡] (funciona mal: acepta la regla dos veces)
            Reconocer ¬ -> ¬ ó "¬" ó \¬ ó [¬] (funciona mal: acepta la regla dos veces) ó [\¬] (funciona mal: acepta la regla dos veces)
            Reconocer ° -> ° ó "°" ó \° ó [°] (funciona mal: acepta la regla dos veces) ó [\°] (funciona mal: acepta la regla dos veces)
            Reconocer [ -> [ ó "[" ó \[ ó [\[]
            Reconocer ] -> ] ó "]" ó \] ó [\]]
            Reconocer ~ -> ~ ó "~" ó \~ ó [\~]

            Reconocer [0-9] -> 5 ó "5" ó [5]
            Reconocer [a-zA-Z] (incluídas ñ y Ñ) -> z ó "z" ó [z]

            Reconocer \ -> "\\" ó \\ ó [\\]
            Reconocer " -> "\"" ó \" ó ["] ó [\"]

            Reconocer ! -> "!" ó \! ó [!] ó [\!]
            Reconocer % -> "%" ó \% ó [%] ó [\%]
            Reconocer < -> "<" ó \< ó [<] ó [\<]
            Reconocer  (un espacio) -> " " ó \  ó [ ] ó [\ ]
            Reconocer / -> "/" ó \/ ó [/] ó [\/]
            Reconocer * -> "*" ó \* ó [*] ó [\*]
            Reconocer + -> "+" ó \+ ó [+] ó [\+]
            Reconocer ? -> "?" ó \? ó [?] ó [\?]
            Reconocer $ -> "$" ó \$ ó [$] ó [\$]
            Reconocer ( -> "(" ó \( ó [(] ó [\(]
            Reconocer ) -> ")" ó \) ó [)] ó [\)]
            Reconocer { -> "{" ó \{ ó [{] ó [\{]
            Reconocer } -> "}" ó \} ó [}] ó [\}]
            Reconocer . -> "." ó \. ó [.] ó [\.]
            Reconocer | -> "|" ó \| ó [|] ó [\|]
            Reconocer ¨ -> "¨" ó \¨ ó [\¨] (funciona mal: acepta la regla dos veces)
            Reconocer ^ -> "^" ó \^ ó [\^]
            Reconocer - -> "-" ó \- ó [\-]

        Un salto de línea (\n) y un <<EOF>> tienen yyleng de 1

        return 0; es equivalente a yyterminate;

*/

/*
//Constantes enteras decimales
1
1u
1l
1ul
1lu
12
12u
12l
12ul
12lu

//Constantes enteras octales
0
0u
0l
0ul
0lu
02
02u
02l
02ul
02lu

//Constantes enteras hexadecimales
0XF
0XFu
0XFl
0XFul
0XFlu

//Constantes reales
    //Constante Fracción
.1
.12
1.1
1.12
12.1
12.12
1.
12.

    //Constante Fracción + Parte Exponente

.1e1
.12e1
1.1e1
1.12e1
12.1e1
12.12e1
1.e1
12.e1

.1e12
.12e12
1.1e12
1.12e12
12.1e12
12.12e12
1.e12
12.e12

.1e-1
.12e-1
1.1e-1
1.12e-1
12.1e-1
12.12e-1
1.e-1
12.e-1

.1e-12
.12e-12
1.1e-12
1.12e-12
12.1e-12
12.12e-12
1.e-12
12.e-12

    //Constante Fracción + Sufijo Real

.1f
.12f
1.1f
1.12f
12.1f
12.12f
1.f
12.f

    //Constante Fracción + Parte Exponente + Sufijo Real

.1e1f
.12e1f
1.1e1f
1.12e1f
12.1e1f
12.12e1f
1.e1f
12.e1f

.1e12f
.12e12f
1.1e12f
1.12e12f
12.1e12f
12.12e12f
1.e12f
12.e12f

.1e-1f
.12e-1f
1.1e-1f
1.12e-1f
12.1e-1f
12.12e-1f
1.e-1f
12.e-1f

.1e-12f
.12e-12f
1.1e-12f
1.12e-12f
12.1e-12f
12.12e-12f
1.e-12f
12.e-12f

    //Secuencia dígitos + Parte Exponente

1e1
12e1
1e12
12e12
1e-1
12e-1
1e-12
12e-12

    //Secuencia dígitos + Parte Exponente + Sufijo real

1e1f
12e1f
1e12f
12e12f
1e-1f
12e-1f
1e-12f
12e-12f

*/

/*

#include <limits.h>
#include <float.h>
#include <stdio.h>

struct test{
  char* a;
  struct test* b;
};

long test = 2147483647;

int main()
{
    printf("%d\n",INT_MIN);
    printf("%d\n",INT_MAX);
    printf("%f\n",FLT_MIN);
    printf("%f\n",FLT_MAX);
    printf("Identificador\n");
    printf("%lu\n",sizeof(test)); // Si pregunto por el tamaño de un identificador me da el tamaño del tipo de dato del identificador, no de la constante almacenada
    printf("Puntero\n");
    printf("%lu\n",sizeof(char*)); // Todos los punteros, apunten al tipo de dato que apunten, incluso a otro puntero, siempre tienen el mismo tamaño
    printf("Int\n");
    printf("%lu\n",sizeof(int));
    printf("%lu\n",sizeof(2147483647)); // Int Máximo
    printf("Long\n");
    printf("%lu\n",sizeof(long));
    printf("%lu\n",sizeof(9223372036800000000)); // De los últimos Long
    printf("Double\n");
    printf("%lu\n",sizeof(double));
    printf("%lu\n",sizeof(9223372036800000000)); // De los últimos Long
    return 0;
}

*/