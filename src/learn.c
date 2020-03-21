#include <stdlib.h>
#include <stdio.h>

//declare functions here //
void printMatrix(double** matrix, int row, int column);
double** transpose(double** transpose, double** matrix, int row, int column);
double** multiply(int column1, int row1,int column2, int row2, double** matrixA, double** matrixB);
double** inverse(double** matrix, int size);

int main(int argc, char *argv[]){

	//variables
	char fileHead[10];
	int numHousesT, numAttributesT, i, j, numHousesD, numAttributesD;

	//checks number of arguements
	if(argc != 3){
		printf("Wrong number of arguements\n");
		return 0;
	}

	//opens the file then checks if it exists
	FILE* data = fopen(argv[2], "r");
	FILE* train = fopen(argv[1], "r");
	if(data == NULL || train == NULL){
		printf("error\n");
		exit(0);
	}

	//checks if file is empty
	if(feof(data) || feof(train)){
		printf("file empty!\n");
		exit(0);
	}
	
	//Scans the first 3 lines and saves the 
	//number of houses(rows) and attributes(column)
	//Data.txt
	fscanf(data, "%s", &fileHead[1]); //skips first line
	fscanf(data, "%d", &numAttributesD); //saves number of attributes
	fscanf(data, "%d", &numHousesD); //saves number of houses

	//Scans the first 3 lines and saves the 
	//number of houses(rows) and attributes(column)
	//Train.txt
	fscanf(train, "%s", &fileHead[1]); //skips first line
	fscanf(train, "%d", &numAttributesT); //saves number of attributes
	fscanf(train, "%d", &numHousesT); //saves number of houses
	
	//Create x matrix from Train
	numAttributesT = numAttributesT + 1;
	double** trainX = malloc(numHousesT  * sizeof(double*));
	for(i = 0; i < numHousesT; ++i){
			trainX[i] = malloc((numAttributesT)  * sizeof(double));
	}

	//initialize x matrix from Train
	for(i=0;i<numHousesT; i++){
		for(j = 0;j < numAttributesT; j++){
			trainX[i][j] = 1;
		}
	}

	for(i = 0; i < numHousesT; i++){
		for(j = 1; j < numAttributesT; j++){
			trainX[i][j] = 0;
		}
	}

	//create y matrix from train
	double** trainY = malloc(numHousesT * sizeof(double*));
	for(i = 0;i < numHousesT; i++){
		trainY[i] = malloc(sizeof(double));
	}

	//initialize y matrix from Train
	for(i = 0; i < 1; i++){
		for(j = 0; j < numHousesT; j++){
			trainY[i][0] = 0;
		}
	}

	//read x matrix from Train
	for(i = 0; i < numHousesT; i++){
		for(j = 1; j < numAttributesT +1; j++){
			if(j != numAttributesT){
				fscanf(train, "%lf", &trainX[i][j]);
			}
			else {
				fscanf(train,"%lf", &trainY[i][0]);
			}
		}
	}

	//Create Train Transpose matrix
	double** trainXTranspose = malloc(numAttributesT * sizeof(double*));
	for(i = 0; i < numAttributesT; ++i){
			trainXTranspose[i] = malloc(numHousesT * sizeof(double));
	}

	//initialize Train transpose matrix
	for(i = 0; i < numAttributesT; i++){
		for(j = 0; j < numHousesT; j++){
			trainXTranspose[i][j] = 0;
		}
	}

	//create matrixA 
	double** matrixA = malloc(numAttributesT  * sizeof(double*));
	for(i = 0; i < numAttributesT; ++i){
			matrixA[i] = malloc(numAttributesT* sizeof(double));
	}

	//initialize matrixA 
	for(i = 0; i < numAttributesT; i++){
		for(j = 0; j < numAttributesT; j++){
			matrixA[i][j] = 0;
		}
	}

	//Create matrix X from data file
	double** inputX = malloc(numHousesD  * sizeof(double*));
	for(i = 0; i < numHousesD; ++i){
			inputX[i] = malloc((numAttributesD+1)  * sizeof(double));
	}

	//initialize matrix X from data file
	for(i = 0; i < numHousesD; i++){
		for(j = 0; j < numAttributesD; j++){
			inputX[i][j] = 1;
		}
	}

	for(i = 0; i < numHousesD; i++){
		for(j = 1; j < numAttributesD; j++){
			inputX[i][j] = 0;
		}
	}

	//read matrix X from data file
	for(i = 0; i < numHousesD; i++)
	{
		for(j = 1; j < numAttributesD+1; j++)
			fscanf(data, "%lf", &inputX[i][j]);
	}	
	
	//create matrixB
	double** matrixB = malloc(numHousesT  * sizeof(double*));
	for(i = 0; i < numHousesT; ++i){
			matrixB[i] = malloc(numAttributesT *sizeof(double));
	}

	//initialize matrixB
	for(i = 0; i < numHousesT; i++){
		for(j = 0; j < numAttributesT; j++){
			matrixB[i][j] = 0;
		}
	}

	//create matrixW
	double** matrixW = malloc(numAttributesT  * sizeof(double*));
	for(i = 0; i < 1; i++){
			matrixW[i] = malloc(sizeof(double));
	}

	//initialize matrixW
	for(i = 0; i < 1; i++){
		for(j = 0; j < numAttributesT; j++){
			matrixW[i][0] = 0;
		}
	}

	//create data matrixY
	double** matrixY = malloc(numHousesD  * sizeof(double*));
	for(i = 0; i < numHousesD; i++){
			matrixY[i] = malloc(sizeof(double));
	}

	//initialize data matrixY
	for(i = 0; i < 1; i++){
		for(j = 0; j < numHousesD; j++){
			matrixY[i][0] = 0;
		}
	}

	//1-Compute matrix transpose 	
		trainXTranspose = transpose(trainXTranspose,trainX, numHousesT, numAttributesT); 
	//2-Compute matrix Multiplication 
		matrixA = multiply(numHousesT,numAttributesT,numAttributesT,numHousesT,trainX,trainXTranspose);//5x5
	//3-Compute matrix Inverse	
		matrixA = inverse( matrixA, numAttributesT);//5x5
	//4-Compute Y' = X'W		
		//W = (matrixA * trainXTranspose) * trainY
		matrixB = multiply(numAttributesT,numAttributesT,numHousesT,numAttributesT,trainXTranspose,matrixA);//5x7
		matrixW = multiply(numHousesT,numAttributesT,1,numHousesT,trainY,matrixB);//5x1
		//y' = inputX * W  //2x1
		matrixY = multiply(numAttributesD+1,numHousesD,1,numAttributesT,matrixW,inputX);
	//5-print Y'			
		printMatrix(matrixY, numHousesD, 1);

	//free memory
	fclose(data);
	fclose(train);
	free(trainX);
	free(trainY);
	free(trainXTranspose);
	free(matrixA);
	free(inputX);
	free(matrixW);
	free(matrixB);
	free(matrixY);
	return 0;
}

//gets inverse of a matrix
double** inverse(double** matrix, int size)
{
	int i, j,k;
	double l;
	
	//create augmented matrix
	double** augM = malloc(size * sizeof(double*));
	for(i = 0; i < size; i++){
			augM[i] = malloc(size*2 *sizeof(double));
	}
	
	//initialize augmented matrix
	for(i = 0; i < size; i++){
		for(j = 0; j < size; j++){
			augM[i][j] = matrix[i][j];
		}
	}
	//makes columns 5-9 have a diaganal of 1s for gauss jordan
	for(i = 0; i < size; i++){
		for(j = size; j < size*2; j++){
			if((i+size) ==j){
				augM[i][j] = 1;
			}
			else{
				augM[i][j] = 0;
			}
		}
	}
	//Gauss jordan 
	for( j = 0; j < size; j++){//row
		for(i = 0; i < size; i++){//column
			if(i != j){
				l = augM[i][j];	
				//subtracts rows
				for(k = 0; k < size*2; k++){
					augM[i][k] -= (augM[j][k]/augM[j][j])*l;
				}
			}
			else{
				l = augM[i][j];
				//divides rows
				for(k = 0; k < size*2; k++){
					augM[i][k] /= l;
				}
			}
		}
	}
	//puts augM 5-9 into matrix 0-4
	for(i = 0; i < size; i++){
		for(j = 0; j < size;j++){
			matrix[i][j] = augM[i][j+size];
		}
	}
	
	return matrix;
}

//multiplies 2 matrixes together
double** multiply(int column1, int row1, int column2, int row2, double** matrixA, double** matrixB)
{
	int i,j,l;

	//Create Results matrix
	double** results = malloc(row1 * sizeof(double*));
	for(i = 0; i < row1; ++i){
			results[i] = malloc(column2* sizeof(double));
	}

	//initialize results matrix
	for(i = 0; i < row1; i++){
		for(j = 0; j < column2; j++){
			results[i][j] = 0;
		}
	}

	for (i = 0; i < row1; i++)
	{//row
		for(j = 0; j < column2; j++)
		{//column
			for(l = 0; l < column1; l++)
			{//multiplies elements
				results[i][j] += matrixA[l][j] * matrixB[i][l];
			}
		}
	}
	return results;
}

//finds transpose
//takes all columns and makes them rows
double** transpose(double** transpose, double** matrix, int row, int column)
{
	int i, j;

	for(i = 0; i < column; ++i){
		for(j = 0; j < row; ++j){
			transpose[i][j] = matrix[j][i];	
		}
	}
	return transpose;
}

//prints matrix
void printMatrix(double** matrix, int row, int column)
{
	int i,j;
	for(i = 0; i < row; i++)
	{
		for(j = 0; j < column; j++)
			printf("%.0f", matrix[i][j]);
		printf("\n");
	}
}
