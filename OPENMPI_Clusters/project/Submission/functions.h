#include <iostream>

using namespace std;

// function to display array
void printArray(double **arr, int row, int col);

void printArray(int **arr, int row, int col);

// A function to read the file and return the data from the file
// Function: read_file
int **read_file(string file_name, int &TOTALARRAYS);

// printing the 3d array
void print3dArray(double ***arr, int row, int col, int depth);

// initalizing the array with random number
void initArray(double **arr, int row, int col);

// write time to the excel file
void writeTimestampToExcel(double timeinSeconds, char *processes);
