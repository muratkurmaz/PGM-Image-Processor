# Module Overview

allocateDynamicMatrix(): The gray tones in the pgm file are stored in a given matrix.
This module allocates memory for the matrix. It takes the height and width information as intiger.
The module returns a 2-dimensional array of unsigned chars allocated in memory.

freeDynamicMatrix(): Releases memory allocated by the allocateDynamicMatrix() module.
It takes an intiger number containing the 2-dimensional array and the height information as arguments.

skipComments(): This module uses classesg in the comments in the pm.
Takes a pointer of type FILE as an argument. The program does not return.

readPgm(): This module reads a file by opening it.
It then passes the read pgm file information into a struct.
It takes two arguments. One of them is the name of the file to open.
The other is a PGM type address where the information will be saved.
AllocateDynamicMatrix(), freeDynamicMatrix() and skipComments() are called within this module.

writePGM(): This module prints the information in the pgm struct type data to the file.
It takes two arguments. One of them is the name of the file to open.
The other is a PGM type address where the information will be read.

reduce(): This module shrinks the pgm file to a certain extent.
It takes two arguments. One of them is a pointer of type pgm struct.
The other is the factor variable, which will specify the extent to which the image will be reduced.
AllocateDynamicMatrix() is called in this module.

isComp(): This module checks if two pgm files are the same.
It takes two arguments. Both of these are pointers of type pgm.

tile(): This module allows to write to the file by dividing a pgm file into a certain number of parts.
It takes 3 arguments. These are pointers of type pgm. the other is the char array holding the name of the output file.
The third is an integer number that holds how many parts the file will be divided into.
AllocateDynamicMatrix() and freeDynamicMatrix() are called within this module.

resize(): This module resizes a pgm file to desired height and width.
It takes 3 arguments. A pointer of type pgm. Integer holding height and width information.
AllocateDynamicMatrix() is called in this module.

emptyPGM(): This module creates a white pgm file.
It takes two arguments. These are integers that hold height and width information.
AllocateDynamicMatrix() is called in this module.

assemble(): This function combines several pgm files to create an output file.
outputPGM = holds output pgm information
assemblePGM = an array containing input files
size = determines the size of the input file array
row and col = keeps max row and col values
height and width = holds the size of the output file
emptyPGM() is called in this module.

Which Module is Used in Which Program?

pgmEcho.c:
	readPgm() , freeDynamicMatrix(), writePGM()

pgmComp.c:
	readPgm() , freeDynamicMatrix(), isComp()

pgma2b.c and pgmb2a.c:
	readPgm() , freeDynamicMatrix(), writePGM()

pgmReduce.c:
	readPgm() , freeDynamicMatrix(), writePGM(), reduce()

pgmTile.c:
	readPgm() , freeDynamicMatrix(), tile()

pgmAssemble.c:
	readPgm() , freeDynamicMatrix(), tile(), allocateDynamicMatrix(), resize()