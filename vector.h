/*********************************************
 * 
 * Filename: vector.h
 * Author: Bryce Vosburg
 * Date: 10/1/2025
 * 
 * Compile: gcc matlab.c vector.c -o matlab
 * 
 **********************************************/

#ifndef VECTOR_H
#define VECTOR_H

typedef struct{
    char name[20];
    float x;
    float y;
    float z;
}vector;

#endif

vector add(vector vect1, vector vect2);

vector sub(vector vect1, vector vect2);

vector s_mult(float scalar, vector vect);

vector dotprod(vector vect1, vector vect2);

vector crossprod(vector vect1, vector vect2);

void list(void);

void clear(void);

void help(void);

void addvect(vector vect);

int findvect(char * name);

void run_interface(void);