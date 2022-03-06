#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include "pav_analysis.h"
#include "fic_wave.h"

int main(int argc, char *argv[]) {
    float durTrm = 0.010;
    float fm, power, am, zcr;
    int   N, numChannels;
    int   trm;
    float *x, *y; //x - left singal, y - right signal
    short *buffer;
    FILE *fpWave;
    FILE *output;
    output = stdout;
    
    /*
    printf("argc = %d\nargv[0] - %s\nargv[1] - %s\n", argc, argv[0], argv[1]);
    
    if (argc == 3) {
        printf("argv[2] - %s\n", argv[2]);
    }*/

    if (argc != 2 && argc != 3) { // Comprueba que el número de argumentos introducidos sea el correcto
        fprintf(stderr, "Empleo: %s inputfile [outputfile]\n", argv[0]);
        return -1;
    }

    if ((fpWave = abre_wave(argv[1], &fm, &numChannels)) == NULL) { // Comprueba que el archivo .wav exista y sea correcto
        fprintf(stderr, "Error al abrir el fichero WAVE de entrada %s (%s)\n", argv[1], strerror(errno));
        return -1;
    }
   
    N = numChannels * durTrm * fm;
    if ((buffer = malloc(N * sizeof(*buffer))) == 0 ||
        (x = malloc(N * sizeof(*x))) == 0) {
        fprintf(stderr, "Error al ubicar los vectores (%s)\n", strerror(errno));
        return -1;
    }

    trm = 0;
    if (argc == 3) {
        //falta pulir errors
        if ((output = fopen(argv[2], "w")) == NULL) {
            fprintf(stderr, "Error obrint l'arxiu de text %s", argv[2]);
        } 
    }

    while (lee_wave(buffer, sizeof(*buffer), N, fpWave) == N) { 
        //printf("\nX\t\t\t\tY\t\t\t\t\tBuffer\n\n");

/*        for (int i = 0 ; i< N; i++){
            printf("%i\n",buffer[i]);
        }*/
        if (numChannels == 1) {
            for (int n = 0; n < N; n++) {
                x[n] = buffer[n] / (float) (1 << 15);
                //printf("%f\t[%i]\t\t\t\t%i\t\n", x[n], n, buffer[n]);
            }
            power = compute_power(x,N);
            am = compute_am(x,N);
            zcr = compute_zcr(x,N,fm);

        } else if (numChannels == 2) {
            for (int n = 0; n < N; n = n + 2)  {
                x[n] = buffer[n] / (float) (1 << 15);
                y[n] = buffer[n+1]/ (float) (1 << 15);
                printf("%f\t[%i]\t\t%f\t[%i]\t\t\t%i\t%i\n", x[n], n, y[n], n+1, buffer[n], buffer[n+1]);
            }
            power = 10* log(exp(compute_power(x,N),10) + exp(compute_power(y,N), 10));    //Hem definit la potencia dels dos canals com la suma dels dos
            am = ( compute_am(x,N) + compute_am(y,N) ) / 2;     //Amplitud mitjana definida com la mitja de les dues
            zcr = compute_zcr(x,N,fm) + compute_zcr(y,N,fm);    //Zeros - Suma de les dues
            printf("%d\t%.4f\t%f\t%f\t\t%f\t%f\t%f\n", trm, compute_power(x,N), compute_am(x,N), compute_zcr(x,N,fm),
                                                     compute_power(y,N), compute_am(y,N), compute_zcr(y,N,fm));
        }

        //printf("\n\n\n");
        scanf("\n");
        fprintf(output,"%d\t%f\t%f\t%f\n", trm, power, am, zcr);
        trm += 1;
    }

    if (argc == 3) {
        fclose(output);
    }

    cierra_wave(fpWave);
    free(buffer);
    free(x);

    return 0;
}
