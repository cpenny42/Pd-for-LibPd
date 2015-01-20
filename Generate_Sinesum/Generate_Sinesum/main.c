//
//  main.c
//  Generate_Sinesum
//
//  Created by Chris Penny on 10/4/14.
//  Copyright (c) 2014 Intrinsic Audio. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef enum {
    sine,
    triangle,
    sawtooth,
    inverse_sawtooth,
    square,
    hann
} Waveform_ID;

static const double TRIANGLE_AMPLITUDE = 0.810569469138702;
static const double SAWTOOTH_AMPLITUDE = 0.63661977236758;
static const double ISAWTOOTH_AMPLITUDE = -0.63661977236758;
static const double SQUARE_AMPLITUDE   = 1; //1.273239544735163;

int print_sinesum(Waveform_ID type, int n_harmonics);

int main(int argc, const char * argv[]) {
    
    int n_harmonics = 41;
    Waveform_ID type = triangle;

    if (argc == 1) {
        printf("\nSinesum Generator: Prints sine summation coefficients from the wave type and number of harmonics.\nWave Types:  \"-sine\" \"-triangle\" \"-sawtooth\" \"-inverse-sawtooth\" \"-square\" \"-hann\"\nExample:\n    ./sinesum -triangle 40\n\n");
    }
    
    for(int i = 0; i < argc; i++) {
        
        if (strcmp(argv[i], "-help") == 0) {
            printf("Sinesum Generator: Prints sine summation coefficients from the wave type and number of harmonics.\n\nWave Types:  \"-sine\" \"-triangle\" \"-sawtooth\" \"-inverse-sawtooth\" \"-square\" \"-hann\"\nExample:\n    ./sinesum -triangle 40\n\n");
        }
        else if (strcmp(argv[i], "-sine") == 0) {
            type = sine;
        }
        else if (!strcmp(argv[i], "-triangle")) {
            type = triangle;
        }
        else if (!strcmp(argv[i], "-sawtooth")) { // Foreward: "/"
            type = sawtooth;
        }
        else if (!strcmp(argv[i], "-inverse-sawtooth")) { // Backward: "\"
            type = inverse_sawtooth;
        }
        else if (!strcmp(argv[i], "-square")) {
            type = square;
        }
        else if (!strcmp(argv[i], "-hann")) {
            type = hann;
        }
        else if (*argv[i] == '-') {
            fprintf(stderr, "Warning: Invalid Arguments. ");
        }
        else {
            n_harmonics = (int)strtol(argv[i], NULL, 10);
        }
    }
    
    return print_sinesum(type, n_harmonics);
}

int print_sinesum(Waveform_ID type, int n_harmonics)
{
    double a = 0.0;
    double harmonic_index = 1;
    if(type == sine) {
        printf("\n1\n\n");
    }
    else if(type == triangle) {
        //http://mathworld.wolfram.com/FourierSeriesTriangleWave.html
        printf("\n");
        for(int i = 0; i < n_harmonics; i++) {
            harmonic_index = 2.0 * i + 1.0;
            if(i % 2 == 0) {
                a = TRIANGLE_AMPLITUDE;
            }
            else {
                a = (-1.0) * TRIANGLE_AMPLITUDE;
            }
            
            printf("%0.12f ", a / (harmonic_index * harmonic_index));
            printf("0 ");
        }
        printf("\n");
    }
    else if(type == sawtooth) {
        //http://mathworld.wolfram.com/FourierSeriesSawtoothWave.html
        printf("\n");
        for(int i = 1; i <= n_harmonics; i++) {
            printf("%0.12f ", SAWTOOTH_AMPLITUDE / (double)i);
        }
        printf("\n");
    }
    else if(type == inverse_sawtooth) {
        printf("\n");
        for(int i = 1; i <= n_harmonics; i++) {
            printf("%0.12f ", ISAWTOOTH_AMPLITUDE / (double)i);
        }
        printf("\n");
    }
    else if(type == square) {
        //http://mathworld.wolfram.com/FourierSeriesSquareWave.html
        printf("\n");
        for(int i = 0; i < n_harmonics; i++) {
            harmonic_index = 2.0 * i + 1.0;
            
            printf("%0.12f ", SQUARE_AMPLITUDE / harmonic_index);
            printf("0 ");
        }
        printf("\n");
    }
    else if(type == hann) {
        printf("\n");
        fprintf(stderr, "Hann not yet supported\n");
        printf("\n");
    }
    else {
        printf("\n");
        fprintf(stderr, "Couldn't print sinesum: Invalid waveform type\n");
        printf("\n");
        return 1;
    }
    
    printf("\n");
    return 0;
}


