#include <stdio.h>
#include <windows.h>
#include <math.h>

#include "list.h"
#include "matrixes.h"

/*
Dynamically allocates m x n matrix
*/
double** createMatrixdouble(int m, int n){

    double** matrix = malloc(m*sizeof(double*));
    if(matrix == NULL){
        printf("ERROR - NOT ENOUGH MEMORY\n");
        exit(0);
    }

    int i;
    for(i=0;i<m;i++){
        matrix[i] = calloc(1,n*sizeof(double));
        if(matrix[i] == NULL){
            printf("ERROR - NOT ENOUGH MEMORY\n");
            exit(0);
        }
    }
    return matrix;
}

double* createArraydouble(int m){
    double* array = calloc(1,m*sizeof(double));
    if(array == NULL){
        printf("ERROR - NOT ENOUGH MEMORY\n");
        exit(0);
    }
    return array;
}

int** createMatrixInt(int m, int n){
    int** matrix = malloc(n*sizeof(int*));
    if(matrix == NULL){
        printf("ERROR - NOT ENOUGH MEMORY\n");
        exit(0);
    }
    int i;
    for(i=0;i<m;i++){
        matrix[i] = calloc(1,n*sizeof(int));
        if(matrix[i] == NULL){
            printf("ERROR - NOT ENOUGH MEMORY\n");
            exit(0);
        }
    }


    return matrix;
}

double* createArrayInt(int m){
    int* array = calloc(1,m*sizeof(int));
    if(array == NULL){
        printf("ERROR - NOT ENOUGH MEMORY\n");
        exit(0);
    }
    return array;
}

void printMatrixLongDouble(long double** matrix, int m, int n){
    int i,j;

    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            printf("%.15lf    ", (double) matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    return;
}


void printMatrix(double** matrix, int m, int n){
    int i,j;

    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            printf("%.15lf    ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    return;
}

void printMatrixWidth(double** matrix, int m, int n){
    int i,j;

    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            printf("%.15f    ",matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    return;
}

void printMatrixdB(double** matrix, int m, int n){
    int i,j;

    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            printf("%.1f    ",10*log10(matrix[i][j]));
        }
        printf("\n");
    }
    printf("\n");
    return;
}

void printArray(double* matrix, int n){
    int i,j;
    for(i=0;i<n;i++)
        printf("%.10f   ",matrix[i]);
    return;

}

void printMatrixInt(int** matrix, int m, int n){
    int i,j;

    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            printf("%d    ",matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    return;
}

void printArrayInt(int* matrix, int n){
    int i;

    for(i=0;i<n;i++)
        printf("%d   ",matrix[i]);
    return;

}

double averageInt(int* array, int n){
    int i;
    double sum = 0;
    for (i=0;i<n;i++){
            sum += (double) array[i];
    }
    return (double)sum/((double)n);
}

double averagedouble(double* array, int n){
    int i;
    double sum = 0;
    for (i=0;i<n;i++){
            sum += array[i];
    }
    return sum/((double)n);
}

double averagedoublePartial(double* array, int startPosition, int size){
    int i;
    double sum = 0;
    for(i=startPosition; i<size; i++)
        sum+= array[i];

    return sum/((double)size - (double)startPosition);

}

double stdDevDouble(double* array, double av, int size){
    int i;
    double var = 0;

    for(i=0; i<size; i++){
        var += pow(array[i] - av, 2);
    }
    var = sqrt(var/(double)size);

    return var;
}

void collumn(double** matrix, double* result, int size, int collumn){
    int i;

    for(i=0; i<size; i++)
        result[i] = matrix[i][collumn];

    return;
}


void sumInt(int*array1, int*array2, int* result, int n, int coef){

    int i;
    for (i=0;i<n;i++){
        result[i] = array1[i] + coef*array2[i];
    }
    return;
}

void divInt(int*array1, int*array2, double*result, int n){
    int i;
    for(i=0;i<n;i++)
        if(array2[i] != 0)
            result[i] = (double)array1[i]/(double)array2[i];

    return;
}

void setValueInt(int* array, int size, int val){
    int i;
    for(i = 0; i<size; i++)
        array[i] = val;
    return;
}

void setValuedouble(double* array, int size, double val){
    int i;
    for(i = 0; i<size; i++)
        array[i] = val;
    return;
}

void outputArray(FILE* file, double* array, int n){

    int i;
    for(i=0;i<n;i++){
        fprintf(file,"%.15f;",array[i]);

    }
    fprintf(file,"\n");

    return;
}

void outputArrayInt(FILE* file, int* array, int n){

    int i;
    for(i=0;i<n;i++){
        fprintf(file,"%d;",array[i]);

    }
    fprintf(file,"\n");

    return;
}

outputMatrix(FILE* file, double** matrix, int m, int n){
    int i,j;
    for(i=0; i<m; i++){
        fprintf(file, ";");
        for(j=0; j<n; j++){
                fprintf(file, "%.15f;",matrix[i][j]);
        }
        fprintf(file, "\n");
    }
    return;
}

void endOfArray(int sizeTotal, int sizeEnd, int* array, int* result){
    int i;

    for(i=0; i<sizeEnd; i++){
        result[i] = array[sizeTotal-sizeEnd+i];

    }


    return;

}

