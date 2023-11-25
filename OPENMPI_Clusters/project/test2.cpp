#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Specify the ranks to be stopped
    int ranksToStop[] = {1, 2};
    int numRanksToStop = sizeof(ranksToStop) / sizeof(ranksToStop[0]);

    // Check if the current rank is in the list of ranks to be stopped
    for (int i = 0; i < numRanksToStop; ++i) {
        if (rank == ranksToStop[i]) {
            std::cout << "Rank " << rank << " stopping.\n";
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    // Continue with the rest of the program
    std::cout << "Rank " << rank << " continuing.\n";

    MPI_Finalize();
    return 0;
}
