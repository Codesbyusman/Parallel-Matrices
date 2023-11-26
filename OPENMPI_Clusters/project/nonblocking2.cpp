#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <ctime>
#include <iostream>

using namespace std;
// we need to define the function to perform the strassen's matrix multiplication but for 2x2 matrix and the base case because we will decide them across mpi processing from the main
/**
 *
 *
(1x1)×(1x1)
(1x1)×(1x2)(1x1)×(1x2)
(1x1)×(2x1)(1x1)×(2x1)
(1x1)×(2x2)(1x1)×(2x2)
(1x2)×(1x1)(1x2)×(1x1)
(1x2)×(1x2)(1x2)×(1x2)
(1x2)×(2x1)(1x2)×(2x1)
(1x2)×(2x2)(1x2)×(2x2)
(2x1)×(1x1)(2x1)×(1x1)
(2x1)×(1x2)(2x1)×(1x2)
(2x1)×(2x1)(2x1)×(2x1)
(2x1)×(2x2)(2x1)×(2x2)
(2x2)×(1x1)(2x2)×(1x1)
(2x2)×(1x2)(2x2)×(1x2)
(2x2)×(2x1)(2x2)×(2x1)
(2x2)×(2x2)(2x2)×(2x2)
 *
 *
*/
void strassen(int *A, int *B, int *C, int rowsA, int colsA, int colsB)
{

    printf("rowsA = %d, colsA = %d, colsB = %d\n", rowsA, colsA, colsB);
    if (rowsA == 1 && colsA == 1 && colsB == 1)
{
    // (1x1) × (1x1)
    C[0] = A[0] * B[0];
    return;
}

if (rowsA == 1 && colsA == 1 && colsB == 2)
{
    // (1x1) × (1x2)
    C[0] = A[0] * B[0];
    C[1] = A[0] * B[1];
    return;
}

if (rowsA == 1 && colsA == 2 && colsB == 1)
{
    // (1x2) × (2x1)
    C[0] = A[0] * B[0] + A[1] * B[0];
    return;
}

if (rowsA == 1 && colsA == 2 && colsB == 2)
{
    // (1x2) × (2x2)
    C[0] = A[0] * B[0] + A[1] * B[2];
    C[1] = A[0] * B[1] + A[1] * B[3];
    return;
}

if (rowsA == 2 && colsA == 1 && colsB == 2)
{
    // (2x1) × (1x2)
    C[0] = A[0] * B[0];
    C[1] = A[0] * B[1];
    C[2] = A[1] * B[0];
    C[3] = A[1] * B[1];
    return;
}
if (rowsA == 2 && colsA == 1 && colsB == 1)
{
    // (2x1) × (1x1)
    C[0] = A[0] * B[0];
    C[1] = A[1] * B[0];
    return;
}
if (rowsA == 2 && colsA == 1 && colsB == 2)
{
    // (2x1) × (1x2)
    C[0] = A[0] * B[0];
    C[1] = A[0] * B[1];
    C[2] = A[1] * B[0];
    C[3] = A[1] * B[1];
    return;
}
if (rowsA == 2 && colsA == 2 && colsB == 1)
{
    // (2x2) × (2x1)
    C[0] = A[0] * B[0] + A[1] * B[1];
    C[1] = A[2] * B[0] + A[3] * B[1];
    return;
}

if (rowsA == 2 && colsA == 2 && colsB == 2)
{
    // (2x2) × (2x2)
    int P = A[0] * (B[1] - B[3]);
    int Q = A[3] * (B[2] - B[0]);
    int R = A[2] * (B[0] + B[1]);
    int S = A[1] * (B[2] + B[3]);
    int T = (A[0] + A[1]) * B[3];
    int U = (A[2] + A[3]) * B[0];
    int V = (A[0] - A[2]) * (B[0] + B[1]);
    int W = (A[1] + A[3]) * (B[2] + B[3]);

    C[0] = P + R - T + W;
    C[1] = Q + T;
    C[2] = R + S;
    C[3] = P + Q - S + V;
    return;
}

}

int main(int argc, char *argv[])
{
    // initialize the openmpi
    MPI_Init(&argc, &argv);

    // define the rank and size
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // A = [1 ,2]
    // B = [3 ,4]
    //     [5 ,6]
/*
int rowsA = 2, colsA = 1, colsB = 2;
int A[] = {1, 2};
int B[] = {3, 4};

*/
    // define the matrix size
    int rowsA = 2;
    int colsA = 1;
    int colsB = 2;
    // dynamic define the matrix A and B using new keyword
    int *A = new int[rowsA * colsA];
    int *B = new int[colsA * colsB];
    // define the matrix C
    int *C = new int[rowsA * colsB];

    if (rank == 0)
    {
        // initialize the matrix A and B
        A[0] = 1;
        A[1] = 2;
        B[0] = 3;
        B[1] = 4;
  
        // print the matrix A
        cout << "A = \n";
        for (int i = 0; i < rowsA; i++)
        {
            // print the matrix A
            for (int j = 0; j < colsA; j++)
            {
                cout << A[i * colsA + j] << " ";
            }
            cout << "\n";
        }

        // print the matrix B
        cout << "B = \n";
        for (int i = 0; i < colsA; i++)
        {
            // print the matrix B
            for (int j = 0; j < colsB; j++)
            {
                cout << B[i * colsB + j] << " ";
            }
            cout << "\n";
        }

        // fill C with rand
        srand(time(NULL));
        for (int i = 0; i < rowsA; i++)
        {
            // fill the matrix C with rand
            for (int j = 0; j < colsB; j++)
            {
                C[i * colsB + j] = rand() % 10;
            }
        }
    }

    MPI_Bcast(A, rowsA * colsA, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, colsA * colsB, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    { // print the matrix C
        strassen(A, B, C, rowsA, colsA, colsB);
        cout << "Resultant matrix is without N\n";
        for (int i = 0; i < rowsA; i++)
        {
            // print the matrix C
            for (int j = 0; j < colsB; j++)
            {
                cout << C[i * colsB + j] << " ";
            }
            cout << "\n";
        }
    }

    // free the memory
    delete[] A;
    delete[] B;
    delete[] C;

    // finalize MPI
    MPI_Finalize();

    return 0;
}

/**
void strassen(int *A, int *B, int *C, int N) {
    // Base case: If the matrix is 1x1, perform regular multiplication
    if (N == 1) {
        C[0] = A[0] * B[0];
        return;
    }

    // if N is == 2 then we will use formula to get the result
    if (N == 2) {
        int P = (A[0] + A[3]) * (B[0] + B[3]);
        int Q = (A[2] + A[3]) * B[0];
        int R = A[0] * (B[1] - B[3]);
        int S = A[3] * (B[2] - B[0]);
        int T = (A[0] + A[1]) * B[3];
        int U = (A[2] - A[0]) * (B[0] + B[1]);
        int V = (A[1] - A[3]) * (B[2] + B[3]);

        C[0] = P + S - T + V;
        C[1] = R + T;
        C[2] = Q + S;
        C[3] = P + R - Q + U;
        return;
    }
}*/

/*
// Function to add two matrices A and B into result
void add(int *A, int *B, int *result, int N) {
    int i, j;
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            result[i*N + j] = A[i*N + j] + B[i*N + j];
}

// Function to subtract matrix B from A and store in result
void subtract(int *A, int *B, int *result, int N) {
    int i, j;
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            result[i*N + j] = A[i*N + j] - B[i*N + j];
}


// Function to divide a matrix into four submatrices
void divide(int *A, int *a11, int *a12, int *a21, int *a22, int N) {
    int i, j, newSize = N / 2;
    for (i = 0; i < newSize; i++)
        for (j = 0; j < newSize; j++) {
            a11[i*newSize + j] = A[i*N + j];
            a12[i*newSize + j] = A[i*N + j + newSize];
            a21[i*newSize + j] = A[(i + newSize)*N + j];
            a22[i*newSize + j] = A[(i + newSize)*N + j + newSize];
        }
}

// Function to combine four submatrices into a matrix
void combine(int *a11, int *a12, int *a21, int *a22, int *A, int N) {
    int i, j, newSize = N / 2;
    for (i = 0; i < newSize; i++)
        for (j = 0; j < newSize; j++) {
            A[i*N + j] = a11[i*newSize + j];
            A[i*N + j + newSize] = a12[i*newSize + j];
            A[(i + newSize)*N + j] = a21[i*newSize + j];
            A[(i + newSize)*N + j + newSize] = a22[i*newSize + j];
        }
}




int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Only the master process performs the matrix multiplication
    if (rank == 0) {
        int N = 4; // Matrix size
        int *A = (int *)malloc(sizeof(int) * N * N);
        int *B = (int *)malloc(sizeof(int) * N * N);
        int *C = (int *)malloc(sizeof(int) * N * N);

        // Initialize matrices A and B
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i*N + j] = i + j;
                B[i*N + j] = i - j;
            }
        }

        // Perform Strassen's matrix multiplication
        strassen(A, B, C, N);

        // print the matrix A
        printf("A = \n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++)
                printf("%d ", A[i*N + j]);
            printf("\n");
        }

        // print the matrix B
        printf("B = \n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++)
                printf("%d ", B[i*N + j]);
            printf("\n");
        }


        // Print the result matrix C
        printf("Resultant matrix is\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%d ", C[i*N + j]);
            }
            printf("\n");
        }

        free(A);
        free(B);
        free(C);
    }

    MPI_Finalize();
    return 0;
}


// Function to perform Strassen's matrix multiplication
void strassen(int *A, int *B, int *C, int N) {
    // Base case: If the matrix is 1x1, perform regular multiplication
    if (N == 1) {
        C[0] = A[0] * B[0];
        return;
    }

    // Calculate new matrix dimensions for submatrices
    int newSize = N / 2;
        // Allocate memory for submatrices
    int *a11, *a12, *a21, *a22, *b11, *b12, *b21, *b22;
    int *c11, *c12, *c21, *c22, *m1, *m2, *m3, *m4, *m5, *m6, *m7;
    int *temp1, *temp2;

    a11 = (int *)malloc(sizeof(int) * newSize * newSize);
    a12 = (int *)malloc(sizeof(int) * newSize * newSize);
    a21 = (int *)malloc(sizeof(int) * newSize * newSize);
    a22 = (int *)malloc(sizeof(int) * newSize * newSize);
    b11 = (int *)malloc(sizeof(int) * newSize * newSize);
    b12 = (int *)malloc(sizeof(int) * newSize * newSize);
    b21 = (int *)malloc(sizeof(int) * newSize * newSize);
    b22 = (int *)malloc(sizeof(int) * newSize * newSize);

    c11 = (int *)malloc(sizeof(int) * newSize * newSize);
    c12 = (int *)malloc(sizeof(int) * newSize * newSize);
    c21 = (int *)malloc(sizeof(int) * newSize * newSize);
    c22 = (int *)malloc(sizeof(int) * newSize * newSize);

    m1 = (int *)malloc(sizeof(int) * newSize * newSize);
    m2 = (int *)malloc(sizeof(int) * newSize * newSize);
    m3 = (int *)malloc(sizeof(int) * newSize * newSize);
    m4 = (int *)malloc(sizeof(int) * newSize * newSize);
    m5 = (int *)malloc(sizeof(int) * newSize * newSize);
    m6 = (int *)malloc(sizeof(int) * newSize * newSize);
    m7 = (int *)malloc(sizeof(int) * newSize * newSize);

    temp1 = (int *)malloc(sizeof(int) * newSize * newSize);
    temp2 = (int *)malloc(sizeof(int) * newSize * newSize);

        // Divide the matrices into submatrices
    divide(A, a11, a12, a21, a22, N);
    divide(B, b11, b12, b21, b22, N);

    // Calculate m1 to m7
    add(a11, a22, temp1, newSize); // a11 + a22
    add(b11, b22, temp2, newSize); // b11 + b22
    strassen(temp1, temp2, m1, newSize); // m1 = (a11+a22) * (b11+b22)

    add(a21, a22, temp1, newSize); // a21 + a22
    strassen(temp1, b11, m2, newSize); // m2 = (a21+a22) * (b11)

    subtract(b12, b22, temp1, newSize); // b12 - b22
    strassen(a11, temp1, m3, newSize); // m3 = (a11) * (b12 - b22)

    subtract(b21, b11, temp1, newSize); // b21 - b11
    strassen(a22, temp1, m4, newSize); // m4 = (a22) * (b21 - b11)

    add(a11, a12, temp1, newSize); // a11 + a12
    strassen(temp1, b22, m5, newSize); // m5 = (a11+a12) * (b22)

        subtract(a21, a11, temp1, newSize); // a21 - a11
    add(b11, b12, temp2, newSize); // b11 + b12
    strassen(temp1, temp2, m6, newSize); // m6 = (a21-a11) * (b11+b12)

    subtract(a12, a22, temp1, newSize); // a12 - a22
    add(b21, b22, temp2, newSize); // b21 + b22
    strassen(temp1, temp2, m7, newSize); // m7 = (a12-a22) * (b21+b22)

    // Calculate c11, c12, c21, c22
    add(m1, m4, temp1, newSize); // m1 + m4
    subtract(temp1, m5, temp2, newSize); // m1 + m4 - m5
    add(temp2, m7, c11, newSize); // c11 = m1 + m4 - m5 + m7

    add(m3, m5, c12, newSize); // c12 = m3 + m5

    add(m2, m4, c21, newSize); // c21 = m2 + m4

    subtract(m1, m2, temp1, newSize); // m1 - m2
    add(temp1, m3, temp2, newSize); // m1 - m2 + m3
    add(temp2, m6, c22, newSize); // c22 = m1 - m2 + m3 + m6

        // Combine the submatrices c11, c12, c21, c22 into the result matrix C
    combine(c11, c12, c21, c22, C, N);

    // Free the dynamically allocated memory
    free(a11); free(a12); free(a21); free(a22);
    free(b11); free(b12); free(b21); free(b22);
    free(c11); free(c12); free(c21); free(c22);
    free(m1); free(m2); free(m3); free(m4); free(m5); free(m6); free(m7);
    free(temp1); free(temp2);
}000*/