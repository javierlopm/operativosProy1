#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "criptfunc.h" // Biblioteca de funciones propias

// Implementacion secuencial para el encriptado/desencriptado de archivos 

/*  Funcion principal
 *   Parametros:
 *       argc: entero que indica numero de comandos en la linea de argumentos
 *       argv: arreglo que contiene los argumentos
 */
int main(int argc, char const *argv[])
{
    int tic,toc;        // contadores del tiempo de ejecucion
    FILE *archivo,*salida; // archivo de entrada y salida
    char *entrada;     // arreglo que contiene los caracteres del archivo de ent
    char *iterador;    // iterador sobre el arreglo de los caracteres de la ent.
    int i;             // contador
    int largoArchivo;

    tic =Tomar_Tiempo(); // calculo tiempo inicial


 
    archivo = fopen(argv[2],"r");



    fseek(archivo, 0, SEEK_END);   // busca el final del archivo
    largoArchivo = ftell(archivo); // obtiene el puntero actual en el archivo
    fseek(archivo, 0, SEEK_SET);   // regresa al inicio del archivo

    // reserva espacio para el texto de entrada
    entrada = (char*) malloc(sizeof(char)*(largoArchivo+1));

    salida = fopen(argv[3],"w");

    // Para encriptar el archivo se llama a cesarizar y luego murcielagisar
    if(strcmp(argv[1],"-c")==0){
        while(fscanf(archivo,"%s",entrada)==1){
            i = 0;
            while(entrada[i]!='\0'){
                cesarizar(&entrada[i]);
                murcielagisar(&entrada[i]);
                fprintf(salida,"%c",entrada[i]);
                i++;
            }

        }
    }
 
    // Para desencriptar llama a descesarizar y luego desmurcielagisar
    if(strcmp(argv[1],"-d")==0){
        while(fscanf(archivo,"%s",entrada)==1){
            i = 0;
            while(entrada[i]!='\0'){
                desmurcielagisar(&entrada[i]);
                descesarizar(&entrada[i]);
                fprintf(salida,"%c",entrada[i]);
                i++;
            }
        }
    }
 
    fclose(archivo);
    fclose(salida);

    free(entrada);

    toc = Tomar_Tiempo(); // calculo del tiempo de finalizacion 
    printf("Tiempo de ejecucion: %f segundos\n",(double)(toc-tic)/ CLOCKS_PER_SEC);
    return 0;
}