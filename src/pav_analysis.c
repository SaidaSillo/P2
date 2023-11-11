<<<<<<< HEAD
#include <stdlib.h>
#include <string.h>
#include <math.h>
=======
#include <math.h>
#include <stdlib.h>
#include <string.h>
>>>>>>> b936122c94bcbac406ae34705f4776fcdcb1b8f1
#include "pav_analysis.h"
#define signo(x) (x>0 ? 1 : x<0 ? -1:0)

float compute_power(const float *x, unsigned int N) {
<<<<<<< HEAD
    float sum = 1e-12;
    for(unsigned int n=0; n<N; n++){
        sum += x[n]*x[n];
    }
    return 10*log10(sum/N);
}

float compute_am(const float *x, unsigned int N) {
    float sum = 0;
    for(unsigned int n=0; n<N; n++){
        sum += fabs(x[n]);
    }
    return sum/N;
}

float compute_zcr(const float *x, unsigned int N, float fm) {
    float sum = 0;
    for(unsigned int n=1; n<N; n++){
        if(signo(x[n])!=signo(x[n-1])){
            sum ++;
        }
    }
    return fm/(2*(N-1))*sum; 
=======
    float pot=1e-12;
    for(unsigned int n=0; n<N; n++){
        pot+=x[n]*x[n];
    }
    return 10*log10(pot/N);
}

float compute_am(const float *x, unsigned int N) {
    float am =0;
    for(unsigned int i=0; i<N;i++){
        am += fabs(x[i]);

    }
    return am/N;
}

float compute_zcr(const float *x, unsigned int N, float fm) {
    float zcr=0;
    for(unsigned int n=1;n<N;n++){
        if(signo(x[n])!=signo(x[n-1])){
            zcr++;
        }
    }
    return fm/(2*(N-1))*zcr;
>>>>>>> b936122c94bcbac406ae34705f4776fcdcb1b8f1
}
