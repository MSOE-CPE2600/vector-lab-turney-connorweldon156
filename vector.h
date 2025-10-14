/**
 * Connor Weldon
 * Vector.h
 * Lab5
 * CPE 2600
 * Description: Header for vector.c. Defines the 
 * Vector struct and declares vector operation functions.
 */

#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    char name[20];
    double val[3];
} Vector;

Vector add(Vector a, Vector b);
Vector subtract(Vector a, Vector b);
Vector scalar_multiply(Vector a, double s);

#endif