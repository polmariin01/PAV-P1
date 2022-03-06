#include <math.h>
#include "pav_analysis.h"

float compute_power(const float *x, unsigned int N) {
    float suma = 1.e-12;
    for(int i = 0;i<N;i++){
        suma = suma + x[i]*x[i];
    }
    return 10*log10((1./N)*suma);
}

float compute_am(const float *x, unsigned int N) {
    float suma = 0;
    for(int i = 0; i<N;i++){
        suma = suma + fabs(x[i]);
    }
    return (1./N)*suma;
}

float compute_zcr(const float *x, unsigned int N, float fm) {
    int passos = 0;
    for(int i = 1; i<N;i++){
        if(x[i]*x[i-1]<0){
            passos++;
        }
    }
    return (fm/(2*(N-1))*passos);
}
