#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgm.h"

int main(int argc, char **argv){

	// check the number of arguments
	if(argc == 1){ 
		printf("Usage: %s inputImage.pgm outputImage.pgm\n", argv[0]);
		return EXIT_NO_ERRORS;
	}
	
	if (argc != 3)	
	{ 
		printf("ERROR: Bad Argument Count\n" );
		return EXIT_WRONG_ARG_COUNT;
	}
	
	// read the pgm file
	PGM *pgm = malloc(sizeof(PGM));
	if(!pgm){
		printf("ERROR: Image Malloc Failed\n");
		exit(EXIT_IMAGE_MALLOC_FAILED);
	}
	readPgm(argv[1], pgm);
	
	if(strcmp(pgm->version,"P5") == 0){
		printf("ERROR: Bad Magic Number (%s)\n",argv[1]);
		return EXIT_BAD_MAGIC_NUMBER;
	}
	
	// change the format of the pgm file
	strcpy(pgm->version, "P5");
	strcpy(pgm->format, "Binary");
	
	//pgm file is saved as new
	writePGM(argv[2], pgm);
	printf("CONVERTED\n");
	
	// frees up memory space
	freeDynamicMatrix(pgm->matrix, pgm->height);
	free(pgm);
	
	return EXIT_NO_ERRORS;
}
