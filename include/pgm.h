#ifndef _PGM_H
#define _PGM_H

#include <stdio.h>

#define EXIT_NO_ERRORS 0         
#define EXIT_WRONG_ARG_COUNT 1   
#define EXIT_BAD_FILE_NAME 2     
#define EXIT_BAD_MAGIC_NUMBER 3  
#define EXIT_BAD_COMMENT_LINE 4
#define EXIT_BAD_DIMENSION 5  
#define EXIT_BAD_MAX_GRAY 6   
#define EXIT_IMAGE_MALLOC_FAILED 7  
#define EXIT_BAD_DATA 8  
#define EXIT_OUTPUT_FAILED 9  
#define EXIT_BAD_LAYOUT 10  
#define EXIT_MISCELLANEOUS 100

#define MIN_IMAGE_DIMENSION 1
#define MAX_IMAGE_DIMENSION 65536
#define MAX_COMMENT_LINE_LENGTH 128

typedef struct pgm{
	int height;
	int width;
	int maxGray;
	char version[3];
	char format[16];
	unsigned char **matrix;
}PGM;

typedef struct assemblePGM{
	PGM* pgm;
	int row;
	int col;
}assemblePGM;

unsigned char **allocateDynamicMatrix(int height, int width);
void freeDynamicMatrix(unsigned char **matrix, int height);
void skipComments(FILE *fp, char *filename);
void readPgm(char *fileName, PGM *pgmStruct);
void writePGM(char *fileName, PGM *pgm);
PGM *reduce(PGM *pgm, int factor);
int isComp(PGM *pgm1, PGM *pgm2);
void tile(PGM *pgm, int fileNumber, char *name);
PGM *resize(PGM *pgm, int newHeight, int newWidth);
PGM *emptyPGM(int height, int width);
void assemble(PGM *outputPGM, assemblePGM *inputPGM, int size ,int row, int col, int height, int width);
char* replaceWord(const char* s, const char* oldW, const char* newW);

#endif