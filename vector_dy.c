/********************************************
 * 
 * Filename: vector_dy.c
 * Author: Bryce Vosburg
 * Date: 10/15/2025
 * 
 * Compile: gcc matlab_dy.c vector_dy.c -o matlab_dy
 * 
 **********************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vector_dy.h"

// Dynamic memory variables
vector *memory;
int vector_count = 0;

vector add(vector vect1, vector vect2){
    vector out;
    out.x = vect1.x + vect2.x;
    out.y = vect1.y + vect2.y;
    out.z = vect1.z + vect2.z;
    out.name[0] = '\0';
    return out;
}

vector sub(vector vect1, vector vect2){
    vector out;
    out.x = vect1.x - vect2.x;
    out.y = vect1.y - vect2.y;
    out.z = vect1.z - vect2.z;
    out.name[0] = '\0';
    return out;
}

vector s_mult(float scalar, vector vect){
    vector out;
    out.x = (scalar * vect.x);
    out.y = (scalar * vect.y);
    out.z = (scalar * vect.z);
    out.name[0] = '\0';
    return out;
}

vector dotprod(vector vect1, vector vect2){
    vector out;
    out.x = vect1.x * vect2.x + vect1.y * vect2.y + vect1.z * vect2.z;
    out.y = 0.0;
    out.z = 0.0;
    out.name[0] = '\0';
    return out;
}

vector crossprod(vector vect1, vector vect2){
    vector out;
    out.x = vect1.y * vect2.z - vect1.z * vect2.y;
    out.y = vect1.z * vect2.x - vect1.x * vect2.z;
    out.z = vect1.x * vect2.y - vect1.y * vect2.x;
    out.name[0] = '\0';
    return out;
}

void list(void){
    printf("Stored Vectors:\n");
    if(vector_count == 0){
        printf("  (none)\n");
        return;
    }
    for(int i = 0; i<vector_count; i++){
        printf("%s = %.2f %.2f %.2f\n", memory[i].name, memory[i].x, memory[i].y, memory[i].z);
    }
}

void clear(void){
    free(memory);
    memory = NULL;
    vector_count = 0;
}

void help(void){
    printf("Vector Calculator Help:\n");
    printf("  name = x y z       Create vector\n");
    printf("  name = a + b       Add vectors\n");
    printf("  name = a - b       Subtract vectors\n");
    printf("  name = a * scalar  Multiply vector by scalar\n");
    printf("  name = a . b       Dot product vectors\n");
    printf("  name = a X b       Cross product vectors\n");
    printf("  clear              Clear all stored vectors\n");
    printf("  list               Show all vectors\n");
    printf("  quit               Exit program\n");
    printf("  load <file name>\n");
    printf("  save <file name>\n");
}

int findvect(char *name){
    if(vector_count == 0 || memory == NULL){
        return -1;
    }
    for(int i = 0; i<vector_count; i++){
        if(memory[i].name[0] != '\0' && strcmp(memory[i].name, name) == 0){
            return i;
        }
    }
    return -1;
}


void addvect(vector vect){
    int idx = findvect(vect.name);
    if(idx != -1){
        memory[idx] = vect;
        return;
    }

    vector_count++;
    memory = realloc(memory, vector_count * sizeof(vector));
    if(!memory){
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    memory[vector_count - 1] = vect;
}

void save_vectors(char *fname){
    FILE *fp = fopen(fname, "w");
    if(!fp){
        printf("Error: cannot open file '%s'\n", fname);
        return;
    }

    for(int i = 0; i<vector_count; i++){
        fprintf(fp, "%s %.2f %.2f %.2f\n", memory[i].name, memory[i].x, memory[i].y, memory[i].z);
    }
    printf("Vectors saved in '%s'\n", fname);
    fclose(fp);
}

void load_vectors(char *fname){
    FILE *fp = fopen(fname, "r");
    if(!fp){
        printf("Error: cannot open file '%s'\n", fname);
        return;
    }

    clear();
    char input[128];
    int line = 1;
    vector temp;

    while(fgets(input, sizeof(input), fp)){
        if(input[0] == '\n' || input[0] == '\r'){
            continue;
        }
        if(sscanf(input, "%s %f %f %f", temp.name, &temp.x, &temp.y, &temp.z) == 4){
            addvect(temp);
        }else{
            printf("Row %d didn't load\n", line);
        }
        line++;
    }
    fclose(fp);
    printf("Loaded %d vectors from '%s'\n", vector_count, fname);
}

void print_vector(char *name, vector vect){
    printf("%s = %.2f %.2f %.2f\n", name, vect.x, vect.y, vect.z);
}

void run_interface(void){
    char input[128];
    char a[32];
    char b[32];
    char result[32];
    char file_name[32];
    float scalar;
    int i1;
    int i2;
    vector temp;

    memory = NULL;
    vector_count = 0;

    atexit(clear);

    while(1){
        printf("> ");

        // Read inputs
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        char *ptr = input;

        // Commands
        if(strcmp(ptr, "quit") == 0){
            break;
        }
        if(strcmp(ptr, "clear") == 0){
            clear();
            continue;
        }
        if(strcmp(ptr, "list") == 0){
            list();
            continue;
        }
        if(strcmp(ptr, "-h") == 0){
            help();
            continue;
        }
        if(sscanf(ptr, "save %31s", file_name) == 1){
            save_vectors(file_name);
            continue;
        }
        if(sscanf(ptr, "load %31s", file_name) == 1){
            load_vectors(file_name);
            continue;
        }
        // End commands

        // Vector input
        if(sscanf(ptr, "%31s = %f %f %f", result, &temp.x, &temp.y, &temp.z) == 4){
            strcpy(temp.name, result);
            temp.name[sizeof(temp.name)-1] = '\0';
            addvect(temp);
            print_vector(temp.name, temp);
            continue;
        }

        // Add
        if(sscanf(ptr, "%31s = %31s + %31s", result, a, b) == 3){
            i1 = findvect(a);
            i2 = findvect(b);
            if(i1 == -1 || i2 == -1){
                printf("Error: one or both vectors not found.\n");
            }else{
                temp = add(memory[i1], memory[i2]);
                strcpy(temp.name, result);
                temp.name[sizeof(temp.name)-1] = '\0';
                addvect(temp);
                print_vector(result, temp);
            }
            continue;
        }
        if(sscanf(ptr, "%31s + %31s", a, b) == 2){
            i1 = findvect(a);
            i2 = findvect(b);
            if(i1 == -1 || i2 == -1){
                printf("Error: one or both vectors not found.\n");
            }else{
                temp = add(memory[i1], memory[i2]);
                print_vector("ans", temp);
            }
            continue;
        }
        // End add

        // Sub
        if(sscanf(ptr, "%31s = %31s - %31s", result, a, b) == 3){
            i1 = findvect(a);
            i2 = findvect(b);
            if(i1 == -1 || i2 == -1){
                printf("Error: one or both vectors not found.\n");
            }else{
                temp = sub(memory[i1], memory[i2]);
                strcpy(temp.name, result);
                temp.name[sizeof(temp.name)-1] = '\0';
                addvect(temp);
                print_vector(result, temp);
            }
            continue;
        }
        if(sscanf(ptr, "%31s - %31s", a, b) == 2){
            i1 = findvect(a);
            i2 = findvect(b);
            if(i1 == -1 || i2 == -1){
                printf("Error: one or both vectors not found.\n");
            }else{
                temp = sub(memory[i1], memory[i2]);
                print_vector("ans", temp);
            }
            continue;
        }
        // End sub

        // Scalar Mult
        if(sscanf(ptr, "%31s = %31s * %f", result, a, &scalar) == 3){
            i1 = findvect(a);
            if(i1 == -1){
                printf("Error: vector not found.\n");
            }else{
                temp = s_mult(scalar, memory[i1]);
                strcpy(temp.name, result);
                temp.name[sizeof(temp.name)-1] = '\0';
                addvect(temp);
                print_vector(result, temp);
            }
            continue;
        }
        if(sscanf(ptr, "%31s = %f * %31s", result, &scalar, a) == 3){
            i1 = findvect(a);
            if(i1 == -1){
                printf("Error: vector not found.\n");
            }else{
                temp = s_mult(scalar, memory[i1]);
                strcpy(temp.name, result);
                temp.name[sizeof(temp.name)-1] = '\0';
                addvect(temp);
                print_vector(result, temp);
            }
            continue;
        }
        if(sscanf(ptr, "%f * %31s", &scalar, a) == 2){
            i1 = findvect(a);
            if(i1 == -1){
                printf("Error: vector not found.\n");
            }else{
                temp = s_mult(scalar, memory[i1]);
                print_vector("ans", temp);
            }
            continue;
        }
        if(sscanf(ptr, "%31s * %f", a, &scalar) == 2){
            i1 = findvect(a);
            if(i1 == -1){
                printf("Error: vector not found.\n");
            }else{
                temp = s_mult(scalar, memory[i1]);
                print_vector("ans", temp);
            }
            continue;
        }
        // End Mult

        // Cross Product
        if(sscanf(ptr, "%31s = %31s X %31s", result, a, b) == 3){
            i1 = findvect(a);
            i2 = findvect(b);
            if(i1 == -1 || i2 == -1){
                printf("Error: one or both vectors not found.\n");
            }else{
                temp = crossprod(memory[i1], memory[i2]);
                strcpy(temp.name, result);
                temp.name[sizeof(temp.name)-1] = '\0';
                addvect(temp);
                print_vector(result, temp);
            }
            continue;
        }
        if(sscanf(ptr, "%31s X %31s", a, b) == 2){
            i1 = findvect(a);
            i2 = findvect(b);
            if(i1 == -1){
                printf("Error: vector not found.\n");
            }else{
                temp = crossprod(memory[i1], memory[i2]);
                print_vector("ans", temp);
            }
            continue;
        }
        // End cross

        // Dot Product
        if(sscanf(ptr, "%31s = %31s . %31s", result, a, b) == 3){
            i1 = findvect(a);
            i2 = findvect(b);
            if(i1 == -1 || i2 == -1){
                printf("Error: one or both vectors not found.\n");
            }else{
                temp = dotprod(memory[i1], memory[i2]);
                strcpy(temp.name, result);
                temp.name[sizeof(temp.name)-1] = '\0';
                addvect(temp);
                print_vector(result, temp);
            }
            continue;
        }
        if(sscanf(ptr, "%31s . %31s", a, b) == 2){
            i1 = findvect(a);
            i2 = findvect(b);
            if(i1 == -1){
                printf("Error: vector not found.\n");
            }else{
                temp = dotprod(memory[i1], memory[i2]);
                print_vector("ans", temp);
            }
            continue;
        }
        // End Dot

        // Single Vector
        if(sscanf(ptr, "%31s", result) == 1){
            i1 = findvect(result);
            if(i1 == -1){
                printf("Error: vector not found.\n");
            }else{
                print_vector(memory[i1].name, memory[i1]);
            }
            continue;
        }
        printf("Invalid command.\n");
    }
}