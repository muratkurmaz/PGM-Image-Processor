#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "pgm.h"


// Making space for the matrix where the data of the PGM file is saved
unsigned char **allocateDynamicMatrix(int height, int width){
	 
	// allocate space for the first dimension of the matrix
	unsigned char **matrix = malloc(sizeof(unsigned char *) * height);
	// Do not give an error message if the malloc command does not work properly
	if(!matrix){
		printf("ERROR: Image Malloc Failed\n");
		exit(EXIT_IMAGE_MALLOC_FAILED);
	}
	
	// allocate space for the second dimension of the matrix
	for(int i = 0; i < height; i++){
		matrix[i] = malloc(sizeof(unsigned char) * width);
		if(!matrix[i]){
			printf("ERROR: Image Malloc Failed\n");
			exit(EXIT_IMAGE_MALLOC_FAILED);
		}
	}
	
	return matrix;
}

// free the matrix allocated memory
void freeDynamicMatrix(unsigned char **matrix, int height){
	for(int i = 0; i < height; i++){
		free(matrix[i]);
	}
	free(matrix);
}

// allows to skip comment lines while reading
void skipComments(FILE *fp, char *filename){
	char ch;
	char cur;
	int count = 0;
	//pass the spaces
	while((ch = fgetc(fp)) != EOF && isspace(ch));
	// if comment line is detected call skip command
	if(ch == '#'){
		//read all comment line
		while( (cur = fgetc(fp)) != EOF)
		{
			if(cur == '\n'){
				fseek(fp, -1, SEEK_CUR);
				break;
			}
			count ++;
			if(count > 128){
				printf("ERROR: Bad Comment Line (%s)\n",filename);
				exit(4);
			}
		}
		skipComments(fp,filename);
	}
	//reset cursor position if comment line is finished
	else{
		fseek(fp, -1, SEEK_CUR);
	}
}

//reads the file with the given name and saves it in the given struct.
void readPgm(char *fileName, PGM *pgmStruct){
	
	// open file for reading
	FILE *pgmFile = fopen(fileName, "rb");
	
	// If there is a problem opening the file, it will give an error message
	if(!pgmFile){
		printf("ERROR: Bad File Name (%s)\n", fileName);
		exit(EXIT_BAD_FILE_NAME);
	}
	skipComments(pgmFile,fileName);
	// read the version of the file
	fgets( pgmStruct->version, sizeof(pgmStruct->version), pgmFile);
	
	if(strcmp(pgmStruct->version, "P2") == 0 ){
		strcpy(pgmStruct->format, "ASCII");
	}
	else if(strcmp(pgmStruct->version, "P5") == 0 ){
		strcpy(pgmStruct->format, "Binary");
	}
	// If the file version is incorrect, an error message is given.
	else{
		printf("ERROR: Bad Magic Number (%s)\n", fileName);
		exit(EXIT_BAD_MAGIC_NUMBER);
	}
	//variable to keep the correct number of file format information.
	int scanCount = 0;
	// comment lines are passed
	skipComments(pgmFile,fileName);
	//read height and width
	scanCount += fscanf(pgmFile, "%d %d", &pgmStruct->width , &pgmStruct->height);
	skipComments(pgmFile,fileName);
	//check the size of the file
	 if((pgmStruct->width 	< MIN_IMAGE_DIMENSION) 	||
		(pgmStruct->width 	>= MAX_IMAGE_DIMENSION) 	||
		(pgmStruct->height < MIN_IMAGE_DIMENSION) 	||
		(pgmStruct->height >= MAX_IMAGE_DIMENSION)   ){ 
		
		// an error message is sent if the file size is incorrect
		printf("ERROR: Bad Dimensions (%s)\n", fileName);
		fclose(pgmFile);
		exit(EXIT_BAD_DIMENSION);
	}
	// read max gray information
	scanCount += fscanf(pgmFile, "%d", &pgmStruct->maxGray);
	fgetc(pgmFile);
	skipComments(pgmFile,fileName);
	//matrix to save the data of the pgm file
	pgmStruct->matrix = allocateDynamicMatrix(pgmStruct->height , pgmStruct->width);
	
	// if there is a problem with the file data, an error message is given
	if (scanCount != 3	){	
		printf("ERROR: Bad Data (%s)\n", fileName);	
		fclose(pgmFile);
		freeDynamicMatrix(pgmStruct->matrix, pgmStruct->height);
		exit(EXIT_BAD_DATA);
	} 
	
	//Check if there is an error in the variable max gray
	else if(pgmStruct->maxGray != 255){
		printf("ERROR: Bad Max Gray Value (%s)\n", fileName);
		fclose(pgmFile);
		freeDynamicMatrix(pgmStruct->matrix, pgmStruct->height);
		exit(EXIT_BAD_MAX_GRAY);
	}
	
	
	
	int grayValue;
	
	// if the file is in ASCII format, it is read by considering the line break
	if(strcmp(pgmStruct->version, "P2") == 0){
		for(int i= 0; i < pgmStruct->height; i++){
			//variable that keeps the read value temporarily
			grayValue = -1;
			for(int j = 0; j < pgmStruct->width; j++){
				// read gray values one by one
				scanCount = fscanf(pgmFile, " %u", &grayValue);
				//if the gray value is invalid, an error message is given
				if((scanCount != 1) ){
					fclose(pgmFile);
					freeDynamicMatrix(pgmStruct->matrix, pgmStruct->height);
					
					printf("ERROR: Bad Data (%s)\n", fileName);	   
					exit(EXIT_BAD_DATA);
				}
				if ((grayValue < 0) || (grayValue > 255))
				{ 
					fclose(pgmFile);
					freeDynamicMatrix(pgmStruct->matrix, pgmStruct->height);
					
					printf("ERROR: Bad Data (%s)\n", fileName);	   
					exit(EXIT_BAD_MAX_GRAY);
				}
				// if the gray value is valid, this will be saved in the metric
				pgmStruct->matrix[i][j] = (unsigned char) grayValue;
			}
			
		}
	
		if(fscanf(pgmFile, " %u", &grayValue) == 1){
			fclose(pgmFile);
			freeDynamicMatrix(pgmStruct->matrix, pgmStruct->height);
			
			printf("ERROR: Bad Data (%s)\n", fileName);	   
			exit(EXIT_BAD_DATA);
		}
	}
	// If binary, write
	else{
		for(int i= 0; i < pgmStruct->height; i++){

			scanCount = fread(pgmStruct->matrix[i], sizeof(unsigned char), pgmStruct->width, pgmFile);
			if(scanCount != pgmStruct->width){
				
				fclose(pgmFile);
				freeDynamicMatrix(pgmStruct->matrix, pgmStruct->height);
				
				printf("ERROR: Bad Data (%s)\n", fileName);	   
				exit(EXIT_BAD_DATA);
			}
		}
		if(fscanf(pgmFile, " %u", &grayValue) == 1){
			fclose(pgmFile);
			freeDynamicMatrix(pgmStruct->matrix, pgmStruct->height);
			
			printf("ERROR: Bad Data (%s)\n", fileName);	   
			exit(EXIT_BAD_DATA);
		}
	}
	
	//file closes back
	fclose(pgmFile);
	
}

void writePGM(char *fileName, PGM *pgm){
	FILE* pgmOut;

    pgmOut = fopen(fileName, "wb");
	
	// If there is a problem opening the file, it will give an error message
	if(!pgmOut){
		printf("ERROR: Output Failed (%s)\n", fileName);
		exit(EXIT_OUTPUT_FAILED);
	}
  
    // Writing Magic Number to the File
    fprintf(pgmOut, "%s\n",pgm->version); 
  
    // Writing Width and Height
    fprintf(pgmOut, "%d %d\n", pgm->width, pgm->height); 
  
    // Writing the maximum gray value
    fprintf(pgmOut, "%d\n", pgm->maxGray); 
    int temp;
	unsigned char byte;
    for (int i = 0; i < pgm->height; i++) {
        for (int j = 0; j < pgm->width; j++) {
			// gray value is assigned to a temporary int variable
            temp = pgm->matrix[i][j];
			// if it is ascii it is written as an int
            if(strcmp(pgm->version, "P2") == 0){
				fprintf(pgmOut, "%d ", temp);
			}
			// if it will be written as binary, the byte will be written to the file as unsigned char
			else{
				byte = (unsigned char) (temp);
				fwrite(&byte, sizeof(unsigned char), 1, pgmOut);
			}
            
        }
		//if the file is encoded with ascii, a line break is placed after each line
		if(strcmp(pgm->version, "P2") == 0)
			fprintf(pgmOut, "\n");
    }
	//file is closed
    fclose(pgmOut);
}

// Gets the pgm file and a factor value
// shrinks the pgm file to a certain extent
// returns the pgm file
PGM *reduce(PGM *pgm, int factor){
	
	// a new file is created
	PGM *newPgm = malloc(sizeof(PGM));
	if(!newPgm){
		printf("ERROR: Image Malloc Failed\n");
		exit(EXIT_IMAGE_MALLOC_FAILED);
	}
	
	//required information is saved in the pgm struct
	strcpy(newPgm->version, pgm->version);
	strcpy(newPgm->format, pgm->format);
	newPgm->height = pgm->height / factor;
	newPgm->width = pgm->width / factor;
	if(newPgm->height < pgm->height / (factor * 1.0))
		newPgm->height ++;
	if(newPgm->width < pgm->width / (factor * 1.0))
		newPgm->width ++;
	newPgm->maxGray = pgm->maxGray ;
	newPgm->matrix = allocateDynamicMatrix(newPgm->height , newPgm->width);
	
	for(int i= 0; i < newPgm->height; i++){
		//transfer gray values to the new pgm structure
		for(int j = 0; j < newPgm->width; j++){
			newPgm->matrix[i][j] = pgm->matrix[i*factor][j*factor];			
		}
	}
	
	return newPgm;
}


// check that the gray values of the two pgm files are the same
// program returns 1 if two files are the same
// if two files are different the program returns 0
int isComp(PGM *pgm1, PGM *pgm2){
	
	// if the file sizes are different, the files are also different
	if(pgm1->height != pgm2->height || pgm1->width != pgm2->width)
		return 0;
	
	for(int i= 0; i < pgm1->height; i++){
		//check each gray value, if different the program returns 0
		for(int j = 0; j < pgm1->width; j++){
			if(pgm1->matrix[i][j] != pgm2->matrix[i][j])
				return 0;
		}
	}
	return 1;
}

// splits the pgm file into parts
// variable filenumber specifies how many parts the file will be divided into
void tile(PGM *pgm, int fileNumber, char *name){
	
	// new pgm file is created and information is saved
	PGM *newPgm = malloc(sizeof(PGM));
	if(!newPgm){
		printf("ERROR: Image Malloc Failed\n");
		exit(EXIT_IMAGE_MALLOC_FAILED);
	}
	char *fileName = NULL;
	strcpy(newPgm->version, pgm->version);
	strcpy(newPgm->format, pgm->format);
	newPgm->height = pgm->height / fileNumber;
	newPgm->width = pgm->width / fileNumber;
	newPgm->maxGray = pgm->maxGray ;
	newPgm->matrix = allocateDynamicMatrix(newPgm->height , newPgm->width);

	//transfer gray values from main pgm file to subfiles
	for(int num = 0; num < fileNumber*fileNumber; num++){
		for(int i= 0; i < newPgm->height; i++){
				
			for(int j = 0; j < newPgm->width; j++){
				newPgm->matrix[i][j] = pgm->matrix[(num/fileNumber)*(newPgm->height) + i][(num%fileNumber)*(newPgm->width) + j];			
			}
		}
		// set the name of the output file
		char row[20], column[20];
		sprintf( row, "%d" ,num/fileNumber );
		sprintf( column, "%d" ,num%fileNumber );
		fileName = replaceWord(name, "<row>", row);
		fileName = replaceWord(fileName, "<column>", column);
	
		// output file is saved
		writePGM(fileName,newPgm);
	}
	
	freeDynamicMatrix(newPgm->matrix, newPgm->height);
	free(newPgm);
}

// resize the pgm file to the desired size
PGM *resize(PGM *pgm, int newHeight, int newWidth){
	
	//space is made for the new pgm file and necessary information is saved
	PGM *newPgm = malloc(sizeof(PGM));
	if(!newPgm){
		printf("ERROR: Image Malloc Failed\n");
		exit(EXIT_IMAGE_MALLOC_FAILED);
	}
	float heightFac = (float)pgm->height / newHeight;
	float widthFac = (float)pgm->width / newWidth;
	
	strcpy(newPgm->version, pgm->version);
	strcpy(newPgm->format, pgm->format);
	newPgm->height = newHeight;
	newPgm->width = newWidth;
	newPgm->maxGray = pgm->maxGray ;
	newPgm->matrix = allocateDynamicMatrix(newPgm->height , newPgm->width);
	
	// import gray values to new pgm file
	for(int i= 0; i < newPgm->height; i++){
		for(int j = 0; j < newPgm->width; j++){
			newPgm->matrix[i][j] = pgm->matrix[(int)(i*heightFac)][(int)(j*widthFac)];
			
		}
	}	
	// program returns the new pgm file
	return newPgm;
}

// create empty white pgm file
PGM *emptyPGM(int height, int width){
	
	//space is reserved for the new pgm file and the information is saved
	PGM *newPgm = malloc(sizeof(PGM));
	if(!newPgm){
		printf("ERROR: Image Malloc Failed\n");
		exit(EXIT_IMAGE_MALLOC_FAILED);
	}
	strcpy(newPgm->version, "P2");
	strcpy(newPgm->format, "ASCII");
	newPgm->height = height;
	newPgm->width = width;
	newPgm->maxGray = 255 ;
	newPgm->matrix = allocateDynamicMatrix(newPgm->height , newPgm->width);
	
	//Assign each gray value of the pgm file to 255
	for(int i= 0; i < newPgm->height; i++){
		for(int j = 0; j < newPgm->width; j++){
			newPgm->matrix[i][j] = 255;
		}
	}
	//pgm file is returned
	return newPgm;
}

// creates a new pgm file by combining the pgm file with its parts
// outputPGM = keeps the output pgm information
// assemblePGM = an array of input files
// size = determines the size of the input file array
// row and col = keeps maximal row and col values
// height and width = holds the size of the output file

void assemble(PGM *outputPGM, assemblePGM *inputPGM, int size ,int row, int col, int height, int width){
	
	int whichRow = 0, whichCol = 0, control = 0;
	//calculating the new size of the shrinked pgm files
	int newHeight = height/(col+1) , newWidth = width/(row+1);
	//if there is no image in a certain location, a blank image is created to insert a white pgm
	PGM *empty = emptyPGM(newHeight, newWidth);
	PGM *tmpPgm;
	
	// loop continues until output file is created
	while(1){
		control = 0;
		
		//Searching for the pgm suitable for the location.
		for(int i = 0; i < size; i++){
			if(inputPGM[i].col == whichRow && inputPGM[i].row == whichCol){
				tmpPgm = inputPGM[i].pgm;
				control = 1;
			}
		}
		
		// if there is no suitable input for that location, it will be filled with the empty pgm
		if(control == 0){
			tmpPgm = empty;
		}
		
		//printing to output file
		for(int i= 0; i < newHeight; i++){	
			for(int j= 0; j < newWidth; j++){
				//printf("%i", tmpPgm->matrix[i%newHeight][j%newWidth]);
				outputPGM->matrix[i+(whichCol*newHeight)][j+(whichRow*newWidth)] = tmpPgm->matrix[i%newHeight][j%newWidth];			
			}
		}
		
		if(whichRow == row && whichCol == col)
			break;
		// increase row col
		whichCol++;
		if(whichCol > col){
			whichCol=0;
			whichRow++;
		}
	}
}


char* replaceWord(const char* s, const char* oldW,
                  const char* newW)
{
    char* result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);
	int chek = 0;
  
    // Counting the number of times old word
    // occur in the string
    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], oldW) == &s[i]) {
            cnt++;
			chek = 1;
            // Jumping to index after the old word.
            i += oldWlen - 1;
        }
    }
  
    // Making new string of enough length
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);
	if(!result){
		printf("ERROR: Image Malloc Failed\n");
		exit(EXIT_IMAGE_MALLOC_FAILED);
	}
  
    i = 0;
    while (*s) {
        // compare the substring with the result
        if (strstr(s, oldW) == s) {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }
  
    result[i] = '\0';
	if(chek != 1){
		printf("ERROR: Miscellaneous (bad template)\n");
		exit(100);
	}
    return result;
}