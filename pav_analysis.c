#include <math.h>
#include <stdio.h>
#include "pav_analysis.h"

float compute_power(const float *x, unsigned int N, int nch) {
    float suma = 1.e-12;
    for(int i = 0; i < N ; i++) {
        suma = suma + x[i]*x[i];
    }
    //printf("Suma pot: %f", suma);
    return 10 * log10(nch * (1./N)*suma);
}

float compute_am(const float *x, unsigned int N) {
    float suma = 0;
    for(int i = 0; i < N; i++){
        suma = suma + fabs(x[i]);
    }
    //printf("\tSuma amp: %f\n", suma);
    return (1./N) * suma;
}

float compute_zcr(const float *x, unsigned int N, float fm) {
    int passos = 0, delay=0;
    for(int i = 1; i <= N; i++){
        delay = 0;
        //printf("\n%i: %f", i, x[i]);
        //if( x[i] *x[i-1] <0){
        if (x[i] == 0 && i<N) delay = 1; //per evitar els passos per 0 quan el valor es exactament 0

        if( (float)(1<<8) * x[i+delay] *x[i-1] <0){    
            //printf("\tzero!");
            passos++;
        }
    }
    //printf("\n\n%i zeros\n\n", passos);
    return (fm/(2*(N-1))*passos);
}
