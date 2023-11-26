#include <iostream>
#include <stack>
#include "functions.h"
#include <mpi.h>

using namespace std;

// int processId; // Process rank
MPI_Status status;

// function to multiply using blocking mpi
void multiply(double **myarr1, double **myarr2, double **myarr3, const int r1, const int c1, const int c2)
{

    // creating the temporary arrays
    // making the depp copies
    double matrix_a[r1][c1];
    double matrix_b[c1][c2];
    double matrix_c[r1][c2]; // the resultant

    // // Use std::copy to copy the elements (such as writing loop and doing)
    // std::copy(&myarr1[0][0], &myarr1[0][0] + r1 * c1, &matrix_a[0][0]);
    // std::copy(&myarr2[0][0], &myarr2[0][0] + c1 * c2, &matrix_b[0][0]);
    // std::copy(&myarr3[0][0], &myarr3[0][0] + r1 * c2, &matrix_c[0][0]);
    // Copy the elements using loops
    for (int i = 0; i < r1; i++)
        for (int j = 0; j < c1; j++)
            matrix_a[i][j] = myarr1[i][j];

    for (int i = 0; i < c1; i++)
        for (int j = 0; j < c2; j++)
            matrix_b[i][j] = myarr2[i][j];

    for (int i = 0; i < r1; i++)
        for (int j = 0; j < c2; j++)
            matrix_c[i][j] = myarr3[i][j];

    // helper variables for getting the processors and all
    int processCount, slaveTaskCount, source, dest, offset, processId;

    // Each process id
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);

    // the size -- number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);

    // the salve processes as 0 would be master so doing -1
    slaveTaskCount = processCount - 1;

    // Master Process
    if (processId == 0)
    {

        printf("\n\t\tMatrix - Matrix Multiplication using MPI\n");

        // Print Matrix A
        printf("\nMatrix A\n\n");
        for (int i = 0; i < r1; i++)
        {
            for (int j = 0; j < c1; j++)
            {
                printf("%.0f\t", matrix_a[i][j]);
            }
            printf("\n");
        }

        // Print Matrix B
        printf("\nMatrix B\n\n");
        for (int i = 0; i < c1; i++)
        {
            for (int j = 0; j < c2; j++)
            {
                printf("%.0f\t", matrix_b[i][j]);
            }
            printf("\n");
        }

        // Determine number of rows of the Matrix A, that is sent to each slave process
        int rows = r1 / slaveTaskCount;
        int extra = r1 % slaveTaskCount;

        // Offset variable determines the starting point of the row which sent to slave process
        offset = 0;

        // Calculation details are assigned to slave tasks. Process 1 onwards;
        // Each message's tag is 1
        for (dest = 1; dest <= slaveTaskCount; dest++)
        {
            int rowsToSend = (dest <= extra) ? rows + 1 : rows;
            // Acknowledging the offset of the Matrix A
            MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            // Acknowledging the number of rows
            MPI_Send(&rowsToSend, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            // Send rows of the Matrix A which will be assigned to slave process to compute
            MPI_Send(&matrix_a[offset][0], rowsToSend * c1, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);
            // Matrix B is sent
            MPI_Send(&matrix_b, c2 * c1, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);

            // Offset is modified according to number of rows sent to each process
            offset += rowsToSend;
        }

        // Root process waits untill the each slave proces sent their calculated result with message tag 2
        for (int i = 1; i <= slaveTaskCount; i++)
        {
            source = i;
            // Receive the offset of particular slave process
            MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
            // Receive the number of rows that each slave process processed
            MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
            // Calculated rows of the each process will be stored int Matrix C according to their offset and
            // the processed number of rows
            MPI_Recv(&matrix_c[offset][0], rows * c2, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
        }

        // copy the matrix_c to myarray3 i.e. resultant
        for (int i = 0; i < r1; i++)
        {
            std::copy(&matrix_c[i][0], &matrix_c[i][0] + c2, &myarr3[i][0]);
        }
        // Print the result matrix
        printf("\nResult Matrix C = Matrix A * Matrix B:\n\n");
        for (int i = 0; i < r1; i++)
        {
            for (int j = 0; j < c2; j++)
            {
                printf("%.0f\t", myarr3[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }

    // Slave Processes
    if (processId > 0)
    {

        int rows = 0;

        // Source process ID is defined
        source = 0;

        // Slave process waits for the message buffers with tag 1, that Root process sent
        // Each process will receive and execute this separately on their processes

        // The slave process receives the offset value sent by root process
        MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        // The slave process receives number of rows sent by root process
        MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        // The slave process receives the sub portion of the Matrix A which assigned by Root
        MPI_Recv(&matrix_a, rows * c1, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
        // The slave process receives the Matrix B
        MPI_Recv(&matrix_b, c1 * c2, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);

        // Matrix multiplication

        for (int k = 0; k < c2; k++)
        {
            for (int i = 0; i < rows; i++)
            {
                // Set initial value of the row summataion
                matrix_c[i][k] = 0.0;
                // Matrix A's element(i, j) will be multiplied with Matrix B's element(j, k)
                for (int j = 0; j < c1; j++)
                    matrix_c[i][k] = matrix_c[i][k] + matrix_a[i][j] * matrix_b[j][k];
            }
        }

        // Calculated result will be sent back to Root process (process 0) with message tag 2

        // Offset will be sent to Root, which determines the starting point of the calculated
        // value in matrix C
        MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        // Number of rows the process calculated will be sent to root process
        MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        // Resulting matrix with calculated rows will be sent to root process
        MPI_Send(&matrix_c, rows * c2, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
    }
}

double **solveEvaluation(int **dimensions, double ***arrays, string evaluation)
{
    stack<double **> stk;
    stack<int *> dimensionsStack;

    for (int i = 0; i < evaluation.length(); i++)
    {
        if (isdigit(evaluation[i]))
        {
            // Push the matrix onto the stack
            stk.push(arrays[evaluation[i] - '1']);
            // Push the dimensions onto the stack
            dimensionsStack.push(dimensions[evaluation[i] - '1']);
        }
        else if (evaluation[i] == ')')
        {
            // Pop the top two matrices, multiply, and push the result back
            double **matrix2 = stk.top();
            stk.pop();
            int *dim2 = dimensionsStack.top();
            dimensionsStack.pop();

            double **matrix1 = stk.top();
            stk.pop();
            int *dim1 = dimensionsStack.top();
            dimensionsStack.pop();

            int r1 = dim1[0];
            int c1 = dim1[1];
            int c2 = dim2[1];

            double **result = new double *[r1];
            for (int i = 0; i < r1; ++i)
                result[i] = new double[c2];

            multiply(matrix1, matrix2, result, r1, c1, c2);

            // Push the result onto the stack
            stk.push(result);

            int *resultDim = new int[2];
            resultDim[0] = r1;
            resultDim[1] = c2;
            dimensionsStack.push(resultDim);
        }
    }

    // The final result should be at the top of the stack
    return stk.top();
}

int main(int argc, char **argv)
{

    int **dimensions = NULL;
    double ***array = NULL;
    int TOTALARRAYS = 0;

    string file_name = "test.txt";
    dimensions = read_file(file_name, TOTALARRAYS);

    // 3d array memory allocation
    array = new double **[TOTALARRAYS];
    for (int i = 0; i < TOTALARRAYS; ++i)
        array[i] = new double *[dimensions[i][0]];

    for (int i = 0; i < TOTALARRAYS; i++)
    {
        for (int j = 0; j < dimensions[i][0]; j++)
        {

            array[i][j] = new double[dimensions[i][1]];
        }
    }

    // initalizing the array
    for (int i = 0; i < TOTALARRAYS; i++)
    {
        initArray(array[i], dimensions[i][0], dimensions[i][1]);
    }

    string evalautaionString = argv[1];

    MPI_Init(&argc, &argv);

    int processId, processCount;

    MPI_Comm_size(MPI_COMM_WORLD, &processCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);

    // the master process
    if (processId == 0)
    {
        if (argc != 2)
        {
            cout << "Invalid arguments for part a to run " << endl;
            return 0;
        }

        cout << "\n\t::::::::::::::::::::::::::" << endl;
        cout << "\t Running part A - MPI Mul" << endl;
        cout << "\t::::::::::::::::::::::::::\n"
             << endl;

        // the arrays
        cout << "The arrays are: " << endl;
        for (int i = 0; i < TOTALARRAYS; i++)
        {
            cout << "Array " << i + 1 << endl;
            printArray(array[i], dimensions[i][0], dimensions[i][1]);
            cout << endl;
        }
    }

    solveEvaluation(dimensions, array, evalautaionString);

    MPI_Finalize();

    return 0;
}