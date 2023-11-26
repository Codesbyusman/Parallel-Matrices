// including the basic header files
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>
#include "functions.h"

using namespace std;

// Function for matrix chain multiplication
int MatrixChainOrder(int dimensions[], int n, int brackets[])
{
    int m[n][n];

    int i, j, k, L, q;

    // cost is zero when multiplying one matrix.
    for (i = 1; i < n; i++)
        m[i][i] = 0;

    // L is chain length.
    for (L = 2; L < n; L++)
    {
        for (i = 1; i < n - L + 1; i++)
        {
            j = i + L - 1;
            m[i][j] = INT_MAX;
            for (k = i; k <= j - 1; k++)
            {
                // q = cost/scalar multiplications
                q = m[i][k] + m[k + 1][j] + dimensions[i - 1] * dimensions[k] * dimensions[j];
                if (q < m[i][j])
                {
                    m[i][j] = q;
                    brackets[i + j * n] = k; // Store the value of k
                }
            }
        }
    }

    return m[1][n - 1];
}

// Function to print the optimal matrix chain multiplication
void printOptimalParenthesis(int i, int j, int n, int brackets[])
{
    if (i == j)
    {
        printf("A%d", i);
    }
    else
    {
        printf("(");
        printOptimalParenthesis(i, brackets[i + j * n], n, brackets);
        printOptimalParenthesis(brackets[i + j * n] + 1, j, n, brackets);
        printf(")");
    }
}

// getting the string in which have to evaluate all of this
string getOPtimalParenthesis(int i, int j, int n, int brackets[])
{
    std::string result;
    if (i == j)
    {
        std::stringstream ss;
        ss << i;
        result += ss.str();
    }
    else
    {
        result += "(";
        result += getOPtimalParenthesis(i, brackets[i + j * n], n, brackets);
        result += getOPtimalParenthesis(brackets[i + j * n] + 1, j, n, brackets);
        result += ")";
    }
    return result;
}

int *getDimentions(int **arr, int row, int col)
{
    int index = 0;
    int *mydimention = new int[row + 1];

    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            // get the first index of the first array
            if (i == 0 && j == 0)
            {
                mydimention[index] = arr[i][j];
                index++;
                // cout << "\n first index:" << arr[i][j] << "\n ";
            }
            // get the second index of the all other arrays
            else if (i != 0 && j == 0)
            {
                mydimention[index] = arr[i][j];
                index++;
                // cout << "\n Last index: " << arr[i][j] << "\n ";
            }
            // get the last index of the last array
            else if (i == row - 1 && j == col - 1)
            {
                mydimention[index] = arr[i][j];
                index++;
                // cout << "\n Final index: " << arr[i][j] << "\n ";
            }
            else
            {
                // cout << arr[i][j] << " ";
            }
        }
        // cout << endl;
    }

    return mydimention;
}

// Function: main
int main(int argc, char **argv)
{

    int **dimensions = NULL;
    double ***array = NULL;
    int TOTALARRAYS = 0;

    srand(time(NULL));

    string file_name = "test.txt";
    dimensions = read_file(file_name, TOTALARRAYS);
    printArray(dimensions, TOTALARRAYS, 2);
    cout<<"\n\n total arrays are: "<<TOTALARRAYS<<endl;
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

    // print the 3d array
    // cout << arrays[0][0] << arrays[0][1] << endl;

    // initalizing the array
    for (int i = 0; i < TOTALARRAYS; i++)
    {
        initArray(array[i], dimensions[i][0], dimensions[i][1]);
    }


    printArray(array[0], dimensions[0][0], dimensions[0][1]);

    // // print the array
    // for (int i = 0; i < TOTALARRAYS; i++)
    // {
    //     cout << "Array " << i + 1 << endl;
    //     printArray(array[i], dimensions[i][0], dimensions[i][1]);
    //     cout << endl;
    // }

    // getting the dimentions into the right format as needed
    // cout << "Total Dimentions  are: " << TOTALARRAYS + 1 << endl;
    int *mydimentions = getDimentions(dimensions, TOTALARRAYS, 2);

    // printing the mydimentions array
    // cout << "\nFetched the Dimentions\n";
    // for (int i = 0; i < TOTALARRAYS + 1; i++)
    // {
    //     cout << mydimentions[i] << " ";
    // }
    // cout << "\n------\n";
    // chain multiplication for the above arrays to get the optimal solution
    // int mydim[] = {2,3,4,2,1};
    int n = TOTALARRAYS + 1;
    cout << "\nn is " << n << "\n";
    int *mybrak = new int[n * n];

    printf("Optimal Cost: %d\n", MatrixChainOrder(mydimentions, n, mybrak));
    printf("Optimal Parenthesization: ");
    printOptimalParenthesis(1, n - 1, n, mybrak);

    string evaluationFlow = getOPtimalParenthesis(1, n - 1, n, mybrak);

    cout << endl;
    // invoking the system call to compile the part a code
    // invoking the system call to compile the part a code
    string command = "mpic++ -o partA partA.cpp functions.cpp";
    system(command.c_str());

    command = "mpirun -np=4 ./partA \"" + evaluationFlow + "\"";
    system(command.c_str());

    delete[] mydimentions;

    return 0;
}
