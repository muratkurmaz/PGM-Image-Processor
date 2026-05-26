#include<stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "pgm.h"

int main(int argc, char **argv){

	// check the number of arguments
	if(argc == 1){ 
		printf("Usage: %s inputImage.pgm reduction_factor outputImage.pgm\n", argv[0]);
		return EXIT_NO_ERRORS;
	}
	if (argc != 4)	
	{ 
		printf("ERROR: Bad Argument Count\n");
		return EXIT_WRONG_ARG_COUNT;
	}
	
	for (int i = 0; argv[2][i]; i++) {
        if (!isdigit(argv[2][i])) {
            printf("ERROR: Miscellaneous is not pozitif digit\n");
            return EXIT_MISCELLANEOUS;
        }
    }
	
	if(atoi(argv[2]) < 1){
		printf("ERROR: Miscellaneous is not pozitif digit\n");
        return EXIT_MISCELLANEOUS;
	}
	
	
	// allocate memory for the pgm file to be read
	PGM *pgm = malloc(sizeof(PGM));
	if(!pgm){
		printf("ERROR: Image Malloc Failed\n");
		exit(EXIT_IMAGE_MALLOC_FAILED);
	}
	
	//create a new structure to hold the resized file
	PGM *newPgm;
	
	// read the input pgm file
	readPgm(argv[1], pgm);
	
	// the file is resized
	newPgm = reduce(pgm, atoi(argv[2]));
	
	// resized file is saved
	writePGM(argv[3], newPgm);
	printf("REDUCED\n");
	
	// frees up memory space
	freeDynamicMatrix(pgm->matrix, pgm->height);
	free(pgm);
	freeDynamicMatrix(newPgm->matrix, newPgm->height);
	free(newPgm);
	
	return EXIT_NO_ERRORS;
}

