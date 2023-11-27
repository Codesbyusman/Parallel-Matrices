#include <iostream>
#include <mpi.h>

using namespace std;
// Declaration of the function in helper.cpp
extern void processArray(int argc, char *argv[], int rowsA,int colsA,int colsB, int p_rank, int num_process, int** returnarray);

int main(int argc, char *argv[]) {
    // Initialize the MPI environment
    int** returnarray= new int*[4];
    for(int i = 0; i < 4; i++){
        returnarray[i] = new int[4];
    }

        int p_rank;
    int num_process;

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
    {
        printf("MPI-INIT Failed\n");
        return 0;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &p_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_process);
    // Call the function in helper.cpp
    processArray(argc, argv, 4, 4, 4, p_rank, num_process, returnarray);
    MPI_Barrier(MPI_COMM_WORLD);
   

    if(p_rank == 0){
        //print the result
        cout << "Finally Result: " << endl;
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                cout << returnarray[i][j] << " ";
            }
            cout << endl;
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);

    cout << "Hello from main.cpp!" << endl;

    return 0;
}
