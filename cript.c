#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include "criptfunc.h"

void imprimirCaracter(char *c){
    switch (*c){
        case -1:
            printf("Ñ");
            break;
        case -2:
            printf("ñ");
            break;
        default:
            putchar((int)(*c));
    }
    fflush(stdin);
}

int main(int argc, char const *argv[])
{
    FILE *archivo;
    char entrada[255];
    char *iterador;
    char *locale;
    int i;
 
    locale = setlocale(LC_ALL, "");
 
    archivo = fopen(argv[2],"r");
    if(strcmp(argv[1],"-c")==0){
        while (!feof(archivo)){
            fscanf(archivo,"%s",entrada); 
            i = 0;
            while(i<strlen(entrada)){
                *iterador = entrada[i];
                cesarizar(iterador);
                murcielagisar(iterador);
                printf("%s",iterador);
                i++;
            }
        }
        printf("\n");
    }
 
    if(strcmp(argv[1],"-d")==0){
        while (!feof(archivo)){
            fscanf(archivo,"%s",entrada); 
            i = 0;
            while(i<strlen(entrada)){
                *iterador = entrada[i];
                desmurcielagisar(iterador);
                descesarizar(iterador);
                printf("%s",iterador);
                i++;
            }
        }
        printf("\n");
    }
 
    fclose(archivo);
 
    return 0;
}