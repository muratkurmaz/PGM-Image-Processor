#include<stdio.h>
#include <stdlib.h>
#include "pgm.h"
#include <ctype.h>
#include <string.h> 
  
int main(int argc, char **argv){

	// check the number of arguments
	if(argc == 1){ 
		printf("Usage: %s inputImage.pgm tiling_factor outputImage_<row>_<column>.pgm\n", argv[0]);
		return EXIT_NO_ERRORS;
	}
	if (argc != 4)	
	{ 
		printf("ERROR: Bad Argument Count\n");
		return EXIT_WRONG_ARG_COUNT;
	}
	
	for (int i = 0; argv[2][i]; i++) {
        if (!isdigit(argv[2][i])) {
            printf("ERROR: Miscellaneous (is not pozitif digit)\n");
            return EXIT_MISCELLANEOUS;
        }
    }
	
	if(atoi(argv[2]) < 1){
		printf("ERROR: Miscellaneous (is not pozitif digit)\n");
        return EXIT_MISCELLANEOUS;
	}
	
	// read the input pgm file
	PGM *pgm = malloc(sizeof(PGM));
	if(!pgm){
		printf("ERROR: Image Malloc Failed\n");
		exit(EXIT_IMAGE_MALLOC_FAILED);
	}
	readPgm(argv[1], pgm);
	
	// the pgm file is split into parts and saved	
	tile(pgm, atoi(argv[2]), argv[3]);
	printf("TILED\n");
	
	// frees up memory space
	freeDynamicMatrix(pgm->matrix, pgm->height);
	free(pgm);
	
	return EXIT_NO_ERRORS;
}

