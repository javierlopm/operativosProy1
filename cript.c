#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

/*
void eliminarAcentos(int *caracter){

    switch( (*caracter) ){
        case 100: // a acentuada
            (*caracter) = 97;
            break;
        case 108: // e acentuada
            (*caracter) = 101;
            break;
        case 112: // i acentuada
            (*caracter) = 105;
            break;
        case 118: // o acentuada
            (*caracter) = 111;
            break;
        case 125: // u acentuada
            (*caracter) = 117;
            break;
        case 116: // enie
            (*caracter) = 110;
            break;
    }

}
*/

void cesarizar(char *c){
    if (64 < (*c & 0xDF) && (*c & 0xDF) < 91){ //Si esta en entre a y z, mayuscula o min
        /*else*/
        *c += 2;

        //Si se salio del ciclo circular lo arreglamos
        if ( ((*c & 0xDF) % 91) < 65){
            //printf("%d",(int) (*c) );
            *c = (*c % 91) + 65;
        }
        /*
        if(*c == 79){
            *c = -1;        //Codigo creados para enie
        }
        else if(*c == 111){
            *c = -2;
        }
        */
        
        if((*c & 0xDF) == 78 || (*c & 0xDF) > 79 ){ //Si la letra se encontraba despues de la enie, necesita un salto adicional
            *c += 1;
        }
        
    }
    else if (*c == -79){
        *c = '-';   //Caso especial con multicaracter enie
    }    
    else if (*c == -111){
        *c = '-';
    }
    //NOTA IMPORTANTE.... TENEMOS QUE CORRER TODOOOOOS LOS CARACTERES DESPUES DE LA N en 1
}

void murcielagisar(char *c){
    switch(*c){
        case 'm':
            *c = 0x30;
            break;
        case 'u':
            *c = 0x31;
            break;
        case 'r':
            *c = 0x32;
            break;
        case 'c':
            *c = 0x33;
            break;
        case 'i':
            *c = 0x34;
            break;
        case 'e':
            *c = 0x35;
            break;
        case 'l':
            *c = 0x36;
            break;
        case 'a':
            *c = 0x37;
            break;
        case 'g':
            *c = 0x38;
            break;
        case 'o':
            *c = 0x39;
            break;

    }
}

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

            //printf("%s",entrada);
            //printf("%s\n",entrada);
            i = 0;
            while(i<strlen(entrada)){
                
                
                if(entrada[i]< 0){
                    i++;
                    //eliminarAcentos((int*)iterador);
                }
                *iterador = entrada[i];
                
                //printf("%d\n",(int) (*iterador) );

                cesarizar(iterador);
                //murcielagisar(iterador);
                imprimirCaracter(iterador);
                i++;
            }
            //putchar(32);

        }
        printf("\n");
    }

    fclose(archivo);

    return 0;
}