# Israk Ayasin
# COS 331 Assignment 3

# Multithreaded Program with Semaphore Protection

This program demonstrates the use of semaphores to protect critical sections in a multithreaded environment. The program generates a 1024x1024 array of random integers and counts the occurrences of each integer value, updating a global array accordingly. Three different semaphore solutions are implemented:

1. 0 Semaphore Solution: Each thread accesses the shared data without protecting the critical section.
2. 1 Semaphore Solution: A single semaphore protects the entire shared array.
3. 256 Semaphore Solution: There is one semaphore per index of the shared array.

## Prerequisites

- GCC compiler
- pthread library

## Compilation

To compile the program, simply run the following command in the terminal:

"make"

This will generate an executable named `hw`.

## Execution

To run the program, use the following command:

./hw <num_threads>

Replace `<num_threads>` with the number of threads you want to create. This parameter specifies the number of threads that will be used to count the occurrences of integers in the array.

## Output

After execution, the program will print the total counts of each integer value in the array for each semaphore solution. Additionally, it will display the execution time for each solution.

## Cleaning Up

To clean up the compiled files, you can run:

make clean

## Files
- `hw2.c`: The main C source code file containing the implementation of the shell.
- `Makefile`: This file contains the makefile. 
- `README.txt`: This file, providing information about the assignment and instructions.
- `sample_output.txt`: This file contains a sample output of 64 threads

## Bugs
	
1. More testing necessary

