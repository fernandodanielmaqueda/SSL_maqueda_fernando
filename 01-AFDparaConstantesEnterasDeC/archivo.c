/*
UTN - FRBA

Sintaxis y Semántica de los Lenguajes - 2022
Trabajo Práctico n° 1 - Autómata Finito Determinístico para Constantes Enteras de C 

Implementar en C un programa que permita reconocer y clasificar las distintas de constantes enteras de C (Decimal, Octal, Hexadecimal). El mismo deberá leer palabras separadas por “,” (carácter centinela) de un archivo de entrada (entrada.txt) y generar un archivo de salida (salida.txt) donde indique qué tipo de constante entera es cada palabra leída o si no fue reconocida. El programa debe contemplar la implementación de un autómata finito determinístico (AFD) para el reconocimiento y clasificación de palabras según estado final. 

Ejemplo del archivo de entrada (secuencia de caracteres separadas por coma): 0xFF,127,0159,0xaBb1,0Xx,0,010,09,127A,120

Ejemplo del archivo de salida:

0xFF		HEXADECIMAL
127		DECIMAL
0159		NO RECONOCIDA
0xaBb1	HEXADECIMAL
0Xx		NO RECONOCIDA
0		OCTAL
010		OCTAL
09		NO RECONOCIDA
127A		NO RECONOCIDA
120		DECIMAL

La entrega de este trabajo práctico no es obligatoria, su fecha límite para consulta, entrega y revisión es el día domingo 15 de mayo. Luego de esa fecha, no se aceptarán más trabajos, y toda consulta referida quedará para la defensa final que será en el mes de noviembre.

El entorno de programación queda a criterio de cada grupo de trabajo (Eclipse, Dev, Codeblocks, Visual Studio Code ). Se recomienda un IDE que esté integrado con Git para poder realizar el trabajo en equipo de una forma más práctica.

Formará parte de la entrega el archivo fuente (.c) y el archivo ejecutable (.exe). Todo debe estar en el repositorio GitHub

La entrega será a través del repositorio de GitHub en la carpeta correspondiente a cada TP

Las consultas podrán ser respondidas a través Discord. Es importante que los utilices para compartir sus dudas con el resto de los compañeros.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <memory.h>
#include <errno.h>

FILE* abrirArchivo(char*,char*);
void cerrarArchivo(FILE*);

int main()
{
    enum Estados
    {
        Q0,
        Q1,
        Q2,
        Q3,
        Q4,
        Q5,
        Q6
    };

    FILE* archivoEntrada = abrirArchivo("entrada.txt","r");
    FILE* archivoSalida = abrirArchivo("salida.txt","w");

    bool banderaEOF = false;
    char caracter = fgetc(archivoEntrada);

    unsigned short int estadoActual = Q0;

    do
    {
        if(caracter != ',' && caracter != EOF)
        {
            putchar(caracter);
            fputc(caracter,archivoSalida);

            switch(estadoActual)
            {
                case Q0:
                    switch(caracter)
                    {
                        case '0':
                            estadoActual = Q1;
                        break;
                        case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                            estadoActual = Q2;
                        break;
                        default:
                            estadoActual = Q6;
                        break;
                    }
                break;
                case Q1:
                    switch(caracter)
                    {
                        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
                            estadoActual = Q5;
                        break;
                        case 'x': case 'X':
                            estadoActual = Q3;
                        break;
                        default:
                            estadoActual = Q6;
                        break;
                    }
                break;
                case Q2:
                    switch(caracter)
                    {
                        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                            //estadoActual = Q2;
                        break;
                        default:
                            estadoActual = Q6;
                        break;
                    }
                break;
                case Q3:
                    switch(caracter)
                    {
                        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case 'A': case 'a': case 'B': case 'b': case 'C': case 'c': case 'D': case 'd': case 'E': case 'e': case 'F': case 'f':
                            estadoActual = Q4;
                        break;
                        default:
                            estadoActual = Q6;
                        break;
                    }
                break;
                case Q4:
                    switch(caracter)
                    {
                        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case 'A': case 'a': case 'B': case 'b': case 'C': case 'c': case 'D': case 'd': case 'E': case 'e': case 'F': case 'f':
                            //estadoActual = Q4;
                        break;
                        default:
                            estadoActual = Q6;
                        break;
                    }
                break;
                case Q5:
                    switch(caracter)
                    {
                        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
                            //estadoActual = Q5;
                        break;
                        default:
                            estadoActual = Q6;
                        break;
                    }
                break;
            }
        }
        else
        {
            switch(estadoActual)
            {
                case Q0: case Q3: case Q6:
                    printf("\tNO RECONOCIDA\n");
                    fprintf(archivoSalida,"\tNO RECONOCIDA\n");
                break;
                case Q1: case Q5:
                    printf("\tOCTAL\n");
                    fprintf(archivoSalida,"\tOCTAL\n");
                break;
                case Q2:
                    printf("\tDECIMAL\n");
                    fprintf(archivoSalida,"\tDECIMAL\n");
                break;
                case Q4:
                    printf("\tHEXADECIMAL\n");
                    fprintf(archivoSalida,"\tHEXADECIMAL\n");
                break;
            }

            estadoActual = Q0;
        }

        if(caracter != EOF) caracter = fgetc(archivoEntrada);
        else banderaEOF = true;
    }
    while(!banderaEOF);

    cerrarArchivo(archivoEntrada);
    cerrarArchivo(archivoSalida);

    system("pause");
    return 0;
}

FILE* abrirArchivo(char* nombreArchivo, char* modo)
{
    FILE* archivo = fopen(nombreArchivo, modo);

    if (errno != 0)
    {
        char* tipoError = "Error al abrir el archivo ";
        char* mensajeError = (char*) malloc(1 + sizeof(char*) * (strlen(nombreArchivo) + strlen(tipoError)));
        strcpy(mensajeError, tipoError);
        strcat(mensajeError, nombreArchivo);
        perror(mensajeError);
        system("pause");
        exit(1);
    }

    return archivo;
}

void cerrarArchivo(FILE* archivo)
{
    fclose(archivo);
}