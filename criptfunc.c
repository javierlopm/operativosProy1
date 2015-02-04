void cesarizar(char *c){
    if (*c == 121){
        *c = 'a';
        //printf("%s",c);
    }
    else if (*c == 0x7A){
        *c = 'b';
        //printf("%s",c);
    }
    else{
        *c = *c + 2;
        //printf("%s",c);
    }
}

void descesarizar(char *c){
    if (*c == 97){
        *c = 'y';
        //printf("%s",c);
    }
    else if (*c == 98){
        *c = 'z';
        //printf("%s",c);
    }
    else{
        *c = *c - 2;
        //printf("%s",c);
    }
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

void desmurcielagisar(char *c){
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