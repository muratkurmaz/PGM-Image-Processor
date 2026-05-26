#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pgm.h"
  
int main(int argc, char **argv){
	
	// check the number of arguments
	if(argc == 1){ 
		printf("Usage: %s inputImage.pgm inputImage.pgm\n", argv[0]);
		return EXIT_NO_ERRORS;
	}
	if (argc != 3)	
	{ 
		printf("ERROR: Bad Argument Count\n");
		return EXIT_WRONG_ARG_COUNT;
	}
	
	//files are read and saved in a struct
	PGM *pgm1 = malloc(sizeof(PGM));
	PGM *pgm2 = malloc(sizeof(PGM));
	if(!pgm1){
		printf("ERROR: Image Malloc Failed\n");
		exit(EXIT_IMAGE_MALLOC_FAILED);
	}
	if(!pgm2){
		printf("ERROR: Image Malloc Failed\n");
		exit(EXIT_IMAGE_MALLOC_FAILED);
	}
	readPgm(argv[1], pgm1);
	readPgm(argv[2], pgm2);
	
	if(isComp(pgm1, pgm2) == 1){
		printf("IDENTICAL\n");
	}
	
	else{
		printf("DIFFERENT\n");
	}
	
	// frees up memory space
	freeDynamicMatrix(pgm1->matrix, pgm1->height);
	free(pgm1);
	freeDynamicMatrix(pgm2->matrix, pgm2->height);
	free(pgm2);
	
	return EXIT_NO_ERRORS;
}

