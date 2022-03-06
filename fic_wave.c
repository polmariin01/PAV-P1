#include <stdio.h>
#include <stdlib.h>
#include "fic_wave.h"

FILE    *abre_wave(const char *ficWave, float *fm, int *nch) {
    FILE    *fpWave;
    int     audioFormat, numChannels, sampleRate, bitsPerSample;

    if ((fpWave = fopen(ficWave, "r")) == NULL) return NULL;

    /*fseek(fpWave, 24, SEEK_SET);    // Puntero posición 24 - fm (4 bytes)
    fread(&sampleRate, 1, 4, fpWave);   // Leemos el valor de la fm
*/
   
    fseek(fpWave, 20, SEEK_SET);
    fread(&audioFormat, 1, 2, fpWave);
    if (audioFormat != 1) fprintf(stderr,"Format d'audio incorrecte\n");
    fread(&numChannels, 1, 2, fpWave);
    //if (numChannels != 1) fprintf(stderr,"Numero de canals incorrecte\n");
    if (numChannels != 1 && numChannels != 2) fprintf(stderr,"Numero de canals incorrecte\n");
    fread(&sampleRate, 1, 4, fpWave);
    fseek(fpWave, 6, SEEK_CUR);
    fread(&bitsPerSample, 1, 2, fpWave);
    if (bitsPerSample != 16) fprintf(stderr, "Numero de bits per mostra incorrecte\n");

    if (audioFormat == 1 && (numChannels == 1 || numChannels == 2) && bitsPerSample ==16) {
        printf("\nFormato correcto! Audio de %i canal(es) con codificación PCM de 16 bits.\nFrecuencia de muestreo - %i\n\n", 
                numChannels, sampleRate);
        *fm = sampleRate;   // Cojemos el valor obtenido
        *nch = numChannels;
    } else {
        printf("\nAudio Format i bits: %i de %i bits\nNum. channels: %i\nSample Rate: %i\n\n", 
            audioFormat, bitsPerSample, numChannels, sampleRate);
        exit(1);
    }

    if (fseek(fpWave, 44, SEEK_SET) < 0) return NULL; //començara a buscar informació al bit 44 (inici dades)

    return fpWave;
}

    //fseek(fpWave, 6, SEEK_CUR);

size_t   lee_wave(void *x, size_t size, size_t nmemb, FILE *fpWave) {
    //printf("Comença a llegir la merda aquesta, estem per la posició %li\n", ftell(fpWave));
    return fread(x, size, nmemb, fpWave);
}

void    cierra_wave(FILE *fpWave) {
    fclose(fpWave);
}
