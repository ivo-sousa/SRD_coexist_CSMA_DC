#ifndef MATRIXES_H_INCLUDED
#define MATRIXES_H_INCLUDED

// create matrixes
double** createMatrixdouble(int m, int n);
double* createArraydouble(int m);
int** createMatrixInt(int m, int n);
double* createArrayInt(int m);

// print matrixes
void printMatrixLongDouble(long double** matrix, int m, int n);
void printMatrix(double** matrix, int m, int n);
void printMatrixWidth(double** matrix, int m, int n);
void printMatrixdB(double** matrix, int m, int n);
void printArray(double* matrix, int n);
void printMatrixInt(int** matrix, int m, int n);
void printArrayInt(int* matrix, int n);


// operations with matrixes
double averageInt(int* array, int n);
double averagedouble(double* array, int n);
double averagedoublePartial(double* array, int startPosition, int size);
double stdDevDouble(double* array, double av, int size);
void collumn(double** matrix, double* result, int size, int collumn);
void sumInt(int*array1, int*array2, int*result, int n, int coef);
void divInt(int*array1, int*array2, double* result, int n);
void setValueInt(int* array, int size, int val);
void setValuedouble(double* array, int size, double val);
void endOfArray(int size, int n, int* array, int* result);



outputMatrix(FILE* file, double** matrix, int m, int n);

#endif // MATRIXES_H_INCLUDED
