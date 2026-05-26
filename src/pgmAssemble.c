#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pgm.h"

int main(int argc, char **argv){
	
	// Checking the correctness of the number of arguments.
	if(argc == 1){ 
		printf("Usage: %s outputImage.pgm width height (row column inputImage.pgm)+\n", argv[0]);
		return EXIT_NO_ERRORS;
	}
	
	if (argc%3 != 0 && argc<6)	
	{ 
		printf("ERROR: Bad Argument Count\n" );
		return EXIT_WRONG_ARG_COUNT;
	}
	
	int height, width;
	//Checking if the dimensions of the output file are numeric values
	
	for (int i = 0; argv[2][i]; i++) {
        if (!isdigit(argv[2][i])) {
            printf("ERROR: Miscellaneous is not pozitif digit\n");
            return EXIT_MISCELLANEOUS;
        }
    }
	
	for (int i = 0; argv[3][i]; i++) {
        if (!isdigit(argv[3][i])) {
            printf("ERROR: Miscellaneous is not pozitif digit\n");
            return EXIT_MISCELLANEOUS;
        }
    }
	
	height = atoi(argv[2]);
	width = atoi(argv[3]);
	
	if(height < 0 || height < 0 ){
		printf("ERROR: Miscellaneous is not pozitif digit\n");
        return EXIT_MISCELLANEOUS;
	}
	
	int maxRow = 0, maxCol = 0;
	
	for(int i = 1; i < (argc-1)/3; i++){
		
		// Is the position of input files taken as an argument a numeric value?

		for (int j = 0; argv[3*i + 1][j]; j++) {
			if (!isdigit(argv[3*i + 1][j])) {
				printf("ERROR: Miscellaneous is not pozitif digit\n");
				return EXIT_MISCELLANEOUS;
			}
		}
		
		for (int j = 0; argv[3*i + 2][j]; j++) {
			if (!isdigit(argv[3*i + 2][j])) {
				printf("ERROR: Miscellaneous is not pozitif digit\n");
				return EXIT_MISCELLANEOUS;
			}
		}
		
		// Determining the largest row and col position of input files
		if(maxRow < atoi(argv[3*i + 1])){
			maxRow = atoi(argv[3*i + 1]);
		}
		if(maxCol < atoi(argv[3*i + 2])){
			maxCol = atoi(argv[3*i + 2]);
		}
		
		// check if different location is selected for input files
		for(int j = i + 1; j < (argc-1)/3; j++)
			if(atoi(argv[3*i + 1]) ==  atoi(argv[3*j + 1]) && atoi(argv[3*i + 2]) ==  atoi(argv[3*j + 2])){
				printf("ERROR: Bad Layout\n");
				return EXIT_BAD_LAYOUT;
			}
	}
	
	int newHeight = height/(maxCol+1) , newWidth = width/(maxRow+1);
	assemblePGM *pgms = malloc(sizeof(assemblePGM)*((argc-4)/3));
	
	//Save the data, row and col information of the input files into an array.
	for(int i = 0; i < (argc-4)/3; i++){
		pgms[i].row = atoi(argv[3*(i+1) + 1]);
		pgms[i].col = atoi(argv[3*(i+1) + 2]);
		pgms[i].pgm = malloc(sizeof(PGM));
		readPgm(argv[3*(i+1) + 3], pgms[i].pgm);

		//input files are resized to fit the output file.
		pgms[i].pgm = resize(pgms[i].pgm, newHeight,  newWidth);
	}
	
	// The necessary information for the input file is saved in the struct and space is allocated
	PGM *outputPGM = malloc(sizeof(PGM));
	strcpy(outputPGM->version, "P2");
	strcpy(outputPGM->format, "ASCII");
	outputPGM->height = height;
	outputPGM->width = width;
	outputPGM->maxGray = 255 ;
	outputPGM->matrix = allocateDynamicMatrix(outputPGM->height , outputPGM->width);
	
	// the files are merged
	assemble(outputPGM, pgms, (argc-4)/3, maxRow, maxCol, height, width);
	// output file is saved
	writePGM(argv[1], outputPGM);
	
	printf("ASSEMBLED\n");
	
	
	// freeing allocated memory
	for(int i = 0; i < (argc-4)/3; i++){
		freeDynamicMatrix(pgms[i].pgm->matrix, pgms[i].pgm->height);
		free(pgms[i].pgm);
	}
	free(pgms);
	
	freeDynamicMatrix(outputPGM->matrix, outputPGM->height);
	free(outputPGM);
	
	
	
	
	return EXIT_NO_ERRORS;
}
