
//Funciones para encriptar y desencriptar archivos

void cesarizar(char *c){
	/*  Funcion que cambia un caracter por los dos que le siguen en el alfabeto. 
		En el caso de la 'y' cambia a 'a' y la 'z' cambia a 'b'.
		Parametros:
			c: caracter a ser cambiado 

		Devuelve nada
	*/
    if (*c == 121){
        *c = 'a';
    }
    else if (*c == 0x7A){
        *c = 'b';
    }
    else{
        *c = *c + 2;
    }
}

void descesarizar(char *c){
	/*  Funcion que cambia un caracter por los dos que le anteceden en el alfabeto. 
		En el caso de la 'a' cambia a 'y' y la 'b' cambia a 'z'.
		Parametros:
			c: caracter a ser cambiado

		Devuelve nada 
	*/
    if (*c == 97){
        *c = 'y';
    }
    else if (*c == 98){
        *c = 'z';
    }
    else{
        *c = *c - 2;
    }
}

void murcielagisar(char *c){
	/*  Funcion que cambia los caracteres 'm,u,r,c,i,e,l,a,g,o' por '0,1,2,3,4,5,6,7,8,9' respectivamente.
		Parametros:
			c: caracter a ser cambiado 

		Devuelve nada
	*/
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

void desmurcielagisar(char *c){
	/*  Funcion que cambia los numeros '0,1,2,3,4,5,6,7,8,9' por 'm,u,r,c,i,e,l,a,g,o' por  respectivamente.
		Parametros:
			c: caracter a ser cambiado 

		Devuelve nada
	*/
    switch(*c){
        case 48:
            *c = 'm';
            break;
        case 49:
            *c = 'u';
            break;
        case 50:
            *c = 'r';
            break;
        case 51:
            *c = 'c';
            break;
        case 52:
            *c = 'i';
            break;
        case 53:
            *c = 'e';
            break;
        case 54:
            *c = 'l';
            break;
        case 55:
            *c = 'a';
            break;
        case 56:
            *c = 'g';
            break;
        case 57:
            *c = 'o';
            break;
    }
}

int divRoundClosest(int n, int d)
	/*	Funcion que sustituye el truncado que usa c en division entera por redondeo hacia arriba
		Parametros:
		n: entero a dividir
		d: entero divisor

		Devuelve un entero
	*/

{
  return ((n < 0) ^ (d < 0)) ? ((n - d/2)/d) : ((n + d/2)/d);
}