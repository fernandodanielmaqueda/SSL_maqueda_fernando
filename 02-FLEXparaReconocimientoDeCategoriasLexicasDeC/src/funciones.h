#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Reconocido
{
    char* texto;
    struct Reconocido* siguiente;
} Reconocido;

typedef struct NoReconocido
{
    char* texto;
    long unsigned linea;
    struct NoReconocido* siguiente;
} NoReconocido;

void pausa(void)
{
    printf("Presione ENTER para continuar...");
    getchar();
}

void verSiHuboUnFalloDeAsignacionDeMemoriaDinamica(void* puntero)
{
    if(puntero == NULL)
    {
        printf("Hubo un fallo de asignacion de memoria dinamica\n");
        pausa();
        exit(1);
    }
}

void iniciarCadena(char** destino, char* fuente)
{
    *destino = (char*) malloc((strlen(fuente) + 1) * sizeof(char));
    verSiHuboUnFalloDeAsignacionDeMemoriaDinamica(*destino);
    strcpy(*destino, fuente);
}

void concatenarCadena(char** destino, char* fuente)
{
    *destino = (char*) realloc(*destino, (strlen(*destino) + strlen(fuente) + 1) * sizeof(char));
    verSiHuboUnFalloDeAsignacionDeMemoriaDinamica(*destino);
    strcat(*destino, fuente);
}

void insertarReconocidoEnListaAlFinal(Reconocido** actual, char* cadena)
{
    while((*actual) != NULL) actual = &((*actual) -> siguiente);
    
    *actual = (Reconocido*) malloc(sizeof(Reconocido));
    verSiHuboUnFalloDeAsignacionDeMemoriaDinamica(*actual);
    iniciarCadena(&((*actual) -> texto),cadena);
    (*actual) -> siguiente = NULL;
}

void insertarNoReconocidoEnListaAlFinal(NoReconocido** actual, char* cadena, long unsigned linea_actual)
{
    while((*actual) != NULL) actual = &((*actual) -> siguiente);
    
    *actual = (NoReconocido*) malloc(sizeof(NoReconocido));
    verSiHuboUnFalloDeAsignacionDeMemoriaDinamica(*actual);
    iniciarCadena(&((*actual) -> texto),cadena);
    (*actual) -> linea = linea_actual;
    (*actual) -> siguiente = NULL;
}

void insertarReconocidoEnListaSegunOrdenAlfabetico(Reconocido** actual, char* cadena)
{
    Reconocido* nuevo = (Reconocido*) malloc(sizeof(Reconocido));
    verSiHuboUnFalloDeAsignacionDeMemoriaDinamica(nuevo);
    iniciarCadena(&(nuevo -> texto),cadena);
    
    char bandera = 0;
    while(!bandera)
    {
        if((*actual) == NULL)
        {
            nuevo -> siguiente = NULL;
            *actual = nuevo;
            bandera = 1;
        }
        else
        {
            if(strcasecmp(cadena, (*actual) -> texto) < 0)
            {
                nuevo -> siguiente = *actual;
                *actual = nuevo;
                bandera = 1;
            }
            else actual = &((*actual) -> siguiente);
        }
    }
}

void insertarReconocidoEnListaPorLongitudAscendenteSiNoEsta(Reconocido** actual, char* cadena)
{
    Reconocido* nuevo = (Reconocido*) malloc(sizeof(Reconocido));
    verSiHuboUnFalloDeAsignacionDeMemoriaDinamica(nuevo);
    iniciarCadena(&(nuevo -> texto),cadena);
    
    char bandera = 0;
    while(!bandera)
    {
        if((*actual) == NULL)
        {
            nuevo -> siguiente = NULL;
            *actual = nuevo;
            bandera = 1;
        }
        else
        {
            if(strlen(cadena) < strlen((*actual) -> texto))
            {
                nuevo -> siguiente = *actual;
                *actual = nuevo;
                bandera = 1;
            }
            else
            {
                if(strlen(cadena) > strlen((*actual) -> texto)) actual = &((*actual) -> siguiente);
                else
                {
                    if(strcmp(cadena, (*actual) -> texto) != 0) actual = &((*actual) -> siguiente);
                    else
                    {
                        free(nuevo -> texto);
                        free(nuevo);
                        bandera = 1;
                    }
                }
            }
        }
    }
}

void liberarListaDeReconocidos(Reconocido* actual)
{
    Reconocido* aux;
    while(actual != NULL)
    {
        aux = actual -> siguiente;
        free(actual -> texto);
        free(actual);
        actual = aux;
    }
}

void liberarListaDeNoReconocidos(NoReconocido* actual)
{
    NoReconocido* aux;
    while(actual != NULL)
    {
        aux = actual -> siguiente;
        free(actual -> texto);
        free(actual);
        actual = aux;
    }
}

void reportarAparicionesSucesivasEnListaDeReconocidos(Reconocido* actual)
{
    char* cadena;
    long unsigned contador;
    char bandera;

    while(actual != NULL)
    {
        cadena = actual -> texto;
        contador = 1;

        bandera = 0;
        while(!bandera)
        {
            actual = actual -> siguiente;
            if(actual == NULL) bandera = 1;
            else
            {
                if(strcmp(cadena, actual -> texto) == 0) ++contador;
                else bandera = 1;
            }
        }
        if(contador == 1) printf("%s -> aparece %lu vez\n",cadena, contador);
        else printf("%s -> aparece %lu veces\n",cadena, contador);
    }
}

void reportarLongitudesEnListaDeReconocidos(Reconocido* actual)
{
    while(actual != NULL)
    {
        printf("%s -> Longitud: %lu\n",actual -> texto, strlen(actual -> texto) - 2);
        actual = actual -> siguiente;
    }
}

void reportarListaDeReconocidos(Reconocido* actual)
{
    while(actual != NULL)
    {
        printf("%s\n",actual -> texto);
        actual = actual -> siguiente;
    }
}

void reportarConversionesADecimalEnListaDeReconocidos(Reconocido* actual)
{
    char* sufijo;

    while(actual != NULL)
    {
        printf("%s -> Valor entero decimal: %lu\n",actual -> texto, strtoul(actual -> texto, &sufijo, 0));
        actual = actual -> siguiente;
    }
}

void reportarSumatoriaDeConstantesEnterasDecimalesDeListaDeReconocidos(Reconocido* actual)
{
    long unsigned constanteEnteraDecimal, totalAcumulado = 0;
    char* sufijo;

    while(actual != NULL)
    {
        constanteEnteraDecimal = strtoul(actual -> texto, &sufijo, 0);
        printf("%s -> Valor: %lu\n",actual -> texto, constanteEnteraDecimal);
        totalAcumulado += constanteEnteraDecimal;
        actual = actual -> siguiente;
    }
    printf("(Total acumulado de sumar todas: %lu)\n", totalAcumulado);
}

void reportarMantisasYParteEnterasEnListaDeReconocidos(Reconocido* actual) // FALTA
{
    double constanteReal, parteEntera, mantisa;
    char* sufijo;

    while(actual != NULL)
    {
        constanteReal = strtod(actual -> texto, &sufijo);
        mantisa = modf(constanteReal, &parteEntera);
        printf("%s -> Constante real: %f Parte entera: %lu Mantisa: %f\n",actual -> texto, constanteReal, (long unsigned) parteEntera, mantisa);
        actual = actual -> siguiente;
    }
}

void reportarEnumerandoListaDeReconocidos(Reconocido* actual)
{
    long unsigned contador = 0;

    while(actual != NULL)
    {
        ++contador;
        printf("%lu- %s\n", contador, actual -> texto);
        actual = actual -> siguiente;
    }
}

void reportarCantidadDeCadaOperadorYCaracterDePuntuacionEnListaDeReconocidos(Reconocido* actual) // FALTA
{
    while(actual != NULL)
    {
        printf("%s\n",actual -> texto);
        actual = actual -> siguiente;
    }
}

void reportarListaDeNoReconocidos(NoReconocido* actual)
{
    while(actual != NULL)
    {
        printf("%s -> Encontrado en la linea %lu\n",actual -> texto, actual -> linea);
        actual = actual -> siguiente;
    }
}

/*
printf("%d\n",(int) strtol(buffer, &sufijo, 0)); printf("%s\n",sufijo);
printf("%f\n", strtod(buffer, &sufijo)); printf("%s\n",sufijo);

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