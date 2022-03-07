#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include "pav_analysis.h"
#include "fic_wave.h"

int main(int argc, char *argv[]) {
    float durTrm = 0.010, durWindow = 0.002;
    float fm, power, am, zcr;
    int   N, L, numChannels;
    int   trm;
    float *x, *y; //x - left singal (just singal if mono), y - right signal
    short *buffer;
    FILE *fpWave;
    FILE *output;
    output = stdout;

    if (argc != 2 && argc != 3) { // Comprueba que el número de argumentos introducidos sea el correcto
        fprintf(stderr, "Empleo: %s inputfile [outputfile]\n", argv[0]);
        return -1;
    }

    if ((fpWave = abre_wave(argv[1], &fm, &numChannels)) == NULL) { // Comprueba que el archivo .wav exista y sea correcto
        fprintf(stderr, "Error al abrir el fichero WAVE de entrada %s (%s)\n", argv[1], strerror(errno));
        return -1;
    }

    L = durTrm * fm;
    N = numChannels * L;

    //printf("N - %i (datos que cojer del archivo wav)\nL - %i (duración de la ventana)\n\n", N, L);

    if ((buffer = malloc(N * sizeof(*buffer))) == 0 ||
        (x = malloc(N * sizeof(*x))) == 0) {
        fprintf(stderr, "Error al ubicar los vectores (%s)\n", strerror(errno));
        return -1;
    }

    trm = 0;
    if (argc == 3) {
        if ((output = fopen(argv[2], "w")) == NULL) {
            fprintf(stderr, "Error obrint l'arxiu de text %s", argv[2]);
            return -1;
        } 
    }

    fprintf(output,"Trm\t\tPower\t\tAmp\t\t\tZeros\n");
    while (lee_wave(buffer, sizeof(*buffer), N, fpWave) == N) { 

        for (int n = 0; n < N; n++) {
            x[n] = buffer[n] / (float) (1 << 15);
            //printf("%f\t[%i]\t\t\t\t%i\t\n", x[n], n, buffer[n]);
        }
        /*
        Definim la potencia com la suma de les dues potencies en lineal
        Per tant, farem la potencia de una senyal de 2*N mostres sencera
        Equival a fer la potencia de dues senyals de N mostres i sumarles
        Ja que a la codificació de wav tenim 16 bits (2 bytes) per codificar un canal
        i 16 per l'altre alternativament
        N equival al nombre de mostres de la finestra temporal (L) quan hi ha un canal
        i serà el doble de les mostres quan hi ha 2
        */

        power = compute_power(x, N, numChannels);
        am = compute_am(x, N);
        
        //printf("Suma pow: %.4f\tSuma amp: %.4f", power, am);

        if (numChannels == 1) {
            zcr = compute_zcr(x,N,fm);

        } else if (numChannels == 2) {
            for (int n = 0; n < N/2; n++)  {
                x[n] = buffer[2*n] / (float) (1 << 15);
                y[n] = buffer[2*n+1]/ (float) (1 << 15);
                //printf("%i/%i\t", n, N/2);
                //printf("%f\t%i-%i\t%f\t%i-%i\t%i\t%i\t", x[n], n, 2*n, y[n], n, 2*n+1, buffer[2*n], buffer[2*n+1]);
                //printf("\n");
            }            
            //El nombre de zeros està definit fent la mitjana de les dues
            zcr = (compute_zcr(x,L,fm) + compute_zcr(y,L,fm) ) / 2;
            //printf("\nPower: %f\tMitjana - %f\n", power, am);
            //scanf("\n");
            //printf("%d\t%.4f\t%f\t%f\t\t%f\t%f\t%f\n", trm, compute_power(x,N), compute_am(x,N), compute_zcr(x,N,fm),
            //                                         compute_power(y,N), compute_am(y,N), compute_zcr(y,N,fm));
        }

        fprintf(output,"%i\t\t%.4f\t%.5f\t\t%.2f\n", trm, power, am, zcr);
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
