/**
 * Connor Weldon
 * vector.c
 * CPE2600
 * Description: Contains math functions for vector operations 
 * such as add, subtract, and scalar multiply.
 */


#include "vector.h"

Vector add(Vector a, Vector b) 
{
    Vector result = { "", {0,0,0} };

    for (int i = 0; i < 3; i++) 
    {
        result.val[i] = a.val[i] + b.val[i];
    }
    return result;
}

Vector subtract(Vector a, Vector b) 
{
    Vector result = { "", {0,0,0} };

    for (int i = 0; i < 3; i++)
    {
        result.val[i] = a.val[i] - b.val[i];
    }
    return result;
}

Vector scalar_multiply(Vector a, double s) 
{
    Vector result = { "", {0,0,0} };

    for (int i = 0; i < 3; i++)
    {
        result.val[i] = a.val[i] * s;
    }
    return result;
}