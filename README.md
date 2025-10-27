# README

## a. Program Description

This program implements an interactive vector calculator that supports dynamic creation, storage, and manipulation of 3D vectors.  
Users can enter commands to define new vectors, perform operations such as addition, subtraction, scalar multiplication, dot and cross products, and save or load data from text files.  

The program uses dynamic memory allocation to manage an expandable list of user-defined vectors, allowing any number of vectors to be created during runtime. It provides a command-line interface where users can type operations directly and view results instantly.

---

## b. How to Build the Program

gcc matlab_dy.c vector_dy.c -o matlab_dy

Make sure the header file `vector_dy.h` is in the same directory as the source files.  
No external libraries are required beyond the C standard library.

---

## c. How to Run the Program

Run the program in a terminal:

./matlab_dy

Once running, the program opens an interactive prompt (`>`) where you can enter commands.  
To exit, type:

quit

There are no additional command-line options at startup; all interaction occurs through text commands entered after launch.



## d. List of Commands

 `name = x y z`  Create a vector with name `name` and components `(x, y, z)` 

 `name = a + b`  Create vector `name` as the sum of vectors `a` and `b` 

 `name = a - b`  Create vector `name` as the difference of `a` and `b` 

 `name = a * scalar`  Multiply vector `a` by a scalar value 

 `name = a . b`  Compute the dot product of `a` and `b` (result stored as a vector with only an x-component) 

 `name = a X b`  Compute the cross product of `a` and `b` 

 `list`  Display all currently stored vectors 

 `clear`  Free all allocated memory and remove all vectors 

 `save <filename>`  Save all stored vectors to a text file 

 `load <filename>`  Load vectors from a text file (overwrites current memory) 

 `quit`  Exit the program 

 `-h`  Display help information 


## e. Dynamic Memory Usage

Unlike static programs that predefine array sizes, this program dynamically allocates memory for each new vector entered by the user.  

- A global pointer `vector *memory` stores all active vectors.  
- Each time a new vector is added, the program uses `realloc()` to expand the memory block:
  
  memory = realloc(memory, vector_count * sizeof(vector));
  
- This allows the list of vectors to grow as needed without setting a fixed limit.
- When the user issues the `clear` command or when the program exits, all allocated memory is freed using:
  
  free(memory);
  
- The cleanup function `clear()` is automatically registered with `atexit()` to ensure memory is released even if the user quits unexpectedly.

This design demonstrates safe, flexible use of dynamic memory management for storing an unknown quantity of user-defined data at runtime.
