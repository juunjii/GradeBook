# GradeBook
A gradebook storing students' name and score with a command line interface that utilized hash tables.  
This program also supports write and read to/from text and binary. 

## Notes
- This program was coded and tested in a Linux Environment <br>
- Ensure that you are in the right directory

## Running Instructions

### Method 1: Makefile
A Makefile is provided. This program then invokes gcc and other commands on your behalf.

The Makefile supports the following commands (listed only necessary ones for running):<br>

- **make**: Compile all code, produce an executable gradbook_main program<br>
- **make clean**: Remove all compiled items, useful if you want to recompile everything from scratch

1. After typing *'make'* (without quotation marks)
2. A new file *'gradebook_main'* would be added to the file directory
3. Type *'./gradebook_main'* to run the program

### Method 2: gcc compiler 
1. In Linux terminal, type *'gcc gradebook_main.c'*
2. A new file **'./a.out'** would be added to the file directory
3. Type *'./a.out'* to run the program. 

