#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *uab;
    int numero;
    long int pos;
    uab = fopen(argv[1], "r");

    fseek(uab, 24, SEEK_SET);

    pos = ftell(uab);
    fread(&numero, 1, 4, uab);
    printf("%li - %8.x - %i\n", pos, numero, numero);
    
    fclose(uab);
}


