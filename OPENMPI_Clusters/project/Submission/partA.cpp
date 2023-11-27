#include <iostream>
#include <iomanip>
#include <stack>
#include <string>
#include "functions.h"
#include <mpi.h>
#include <ctime>
#include <fstream>

using namespace std;

// int processId; // Process rank
MPI_Status status;

// function to multiply using blocking mpi -------- basic mpi implementation
void multiply(double **myarr1, double **myarr2, double **myarr3, const int r1, const int c1, const int c2, int id1, int id2)
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

        printf("\n\t\tMatrix - Matrix Multiplication using MPI\n\n");

        // Print Matrix A
        cout << "Matrix A (" << r1 << "x" << c1 << ") :::::::::::: A";
        id1 == -1 ? cout << "R" << endl : cout << id1 << endl;

        cout << "Matrix B (" << c1 << "x" << c2 << ") :::::::::::: A";
        id2 == -1 ? cout << "R" << endl : cout << id2 << endl;
        cout << endl;

        cout << "Matrix C (" << r1 << "x" << c2 << ") :::::::::::: AR" << endl;
        // for (int i = 0; i < r1; i++)
        // {
        //     for (int j = 0; j < c1; j++)
        //     {
        //         printf("%.0f\t", matrix_a[i][j]);
        //     }
        //     printf("\n");
        // }

        // // Print Matrix B
        // printf("\nMatrix B\n\n");
        // for (int i = 0; i < c1; i++)
        // {
        //     for (int j = 0; j < c2; j++)
        //     {
        //         printf("%.0f\t", matrix_b[i][j]);
        //     }
        //     printf("\n");
        // }

        // determine the rows of a so that to send each to slave
        // as rectangular one of slaves may have more than one rows to process
        int rows = r1 / slaveTaskCount;
        int extra = r1 % slaveTaskCount;

        // offset so to track the location of rows being sent
        offset = 0;

        // Calculation details are assigned to slave tasks. Process 1 onwards;
        // Each message's tag is 1
        for (dest = 1; dest <= slaveTaskCount; dest++)
        {
            // looking if we have extra rows
            int rowsToSend = (dest <= extra) ? rows + 1 : rows;
            // sending offsets
            MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            // sending row numbers
            MPI_Send(&rowsToSend, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            // Send rows of the Matrix A which will be assigned to slave process to compute (actual rows)
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

            // recvieing the results
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
        cout << "\n\t........ Processing with help of slaves ........" << endl;
        cout << "\t................ Result Returned ..............." << endl
             << endl;
        // for (int i = 0; i < r1; i++)
        // {
        //     for (int j = 0; j < c2; j++)
        //     {
        //         printf("%.0f\t", myarr3[i][j]);
        //     }
        //     printf("\n");
        // }
        // printf("\n");
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

// the basic function that solves according to the optimal order beig given
double **solveEvaluation(int **dimensions, double ***arrays, string evaluation)
{
    // maintaing stacks to solve the nested parenthesis operations
    stack<double **> stk;         // the array stack
    stack<int *> dimensionsStack; // to store dimensions parallely
    stack<int> arrayId;           // storing ids just for printing the stuff

    // iterate till whole evaluation string
    for (int i = 0; i < evaluation.length(); i++)
    {
        // pushing into stack if its digit else wise will pop when ever recieved a ) bracket
        if (isdigit(evaluation[i]))
        {
            // Push the matrix onto the stack
            stk.push(arrays[evaluation[i] - '1']);
            // Push the dimensions onto the stack
            dimensionsStack.push(dimensions[evaluation[i] - '1']);
            arrayId.push(evaluation[i] - '0');
        }
        else if (evaluation[i] == ')')
        {
            // Pop the top two matrices, multiply, and push the result back
            // getting the two top matrices to multiply each other
            double **matrix2 = stk.top();
            stk.pop();
            int *dim2 = dimensionsStack.top();
            dimensionsStack.pop();
            int id2 = arrayId.top();
            arrayId.pop();

            double **matrix1 = stk.top();
            stk.pop();
            int *dim1 = dimensionsStack.top();
            dimensionsStack.pop();
            int id1 = arrayId.top();
            arrayId.pop();

            // getting the dimensions accordingly
            int r1 = dim1[0];
            int c1 = dim1[1];
            int c2 = dim2[1];

            double **result = new double *[r1];
            for (int i = 0; i < r1; ++i)
                result[i] = new double[c2];

            // calling the actual multiply function
            // let say it will multiply A1 and A2 and would return result r we are boun dto get r
            // so we can procced furthure
            multiply(matrix1, matrix2, result, r1, c1, c2, id1, id2);

            MPI_Barrier(MPI_COMM_WORLD);
            // Push the result onto the stack
            stk.push(result);

            int *resultDim = new int[2];
            resultDim[0] = r1;
            resultDim[1] = c2;
            dimensionsStack.push(resultDim);
            arrayId.push(-1);
        }
    }

    // The final result should be at the top of the stack
    return stk.top();
}

int main(int argc, char **argv)
{

    // our arrays to store all
    int **dimensions = NULL;
    double ***array = NULL;
    int TOTALARRAYS = 0;

    // assuming dimensions good enough as being tested by main.cpp
    // and we have been called only when dimensions are being verified
    string file_name = "Dimensions.txt";
    dimensions = read_file(file_name, TOTALARRAYS);

    // 3d array memory allocation
    // according to the dimensions being read
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
        if (argc != 3)
        {
            cout << "Invalid arguments for part a to run " << endl;
            return 1;
        }

        cout << "\n\t::::::::::::::::::::::::::" << endl;
        cout << "\t Running part B - MPI Mul" << endl;
        cout << "\t::::::::::::::::::::::::::\n"
             << endl;

        // // the arrays
        // cout << "The arrays are: " << endl;
        // for (int i = 0; i < TOTALARRAYS; i++)
        // {
        //     cout << "Array " << i + 1 << endl;
        //     printArray(array[i], dimensions[i][0], dimensions[i][1]);
        //     cout << endl;
        // }

        cout << "\n Lets multiply using MPI according to optimal order " << endl;
    }

    if (argc != 3)
    {

        return 1;
    }

    // the resultant array ---- the grand final result
    int fr = dimensions[0][0];
    int fc = dimensions[TOTALARRAYS - 1][1];
    double **finalResult = new double *[fr];
    for (int i = 0; i < fr; i++)
    {
        finalResult[i] = new double[fc];
    }

    std::clock_t start, stop;

   // MPI_Barrier(MPI_COMM_WORLD);
    if (processId == 0)
        // Get the starting clock tick
        start = std::clock();

    // storing the result
    finalResult = solveEvaluation(dimensions, array, evalautaionString);

    // Get the ending clock tick s
    if (processId == 0)
        stop = std::clock();

    if (processId == 0)
    {
        // asking if wanted to print because too big answer
        char option = 0;
        cout << "You want to see final result (Y/N) : ";
        cin >> option;

        if (option == 'Y' || option == 'y')
        {
            cout << "\nFinal Resultant array is: " << endl;
            printArray(finalResult, fr, fc);
        }
        // Calculate the elapsed time in seconds
        double elapsedSeconds = (double)(stop - start) / CLOCKS_PER_SEC;
        double elapsedMillisecond = (double)(stop - start) * 1000 / CLOCKS_PER_SEC;

        cout << fixed << setprecision(2);
        cout << "\n\nTime taken by code: " << elapsedSeconds << " seconds" << endl;
        cout << "Time taken by code: " << elapsedMillisecond << " milliseconds" << endl
             << endl;

        // writing in the csv
        writeTimestampToExcel(elapsedSeconds, argv[2]);
        cout << "\n\nGraphs are in proces ...............\n\n"
             << endl;
    }

   // MPI_Barrier(MPI_COMM_WORLD);

    // free the memory
    delete[] dimensions;
    delete[] array;

    // Finalize the MPI environment.
    MPI_Finalize();

    return 0;
}