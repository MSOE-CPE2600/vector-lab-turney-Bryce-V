/********************************************
 * 
 * Filename: vector_dy.h
 * Author: Bryce Vosburg
 * Date: 10/15/2025
 * 
 * Compile: gcc matlab_dy.c vector_dy.c -o matlab_dy
 * 
 **********************************************/

#ifndef VECTOR_H
#define VECTOR_H

typedef struct{
    char name[10];
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

void addvect(vector vect);

void save_vectors(char *fname);

void load_vectors(char *fname);

void print_vector(char *name, vector vect);
