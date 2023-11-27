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

// will be using for checking the dimesnsions provided are correct or not
bool dimensionChecker(int **dimensions, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        // someting went wrong
        // previous col not eqiual to nexts row
        if (dimensions[i][1] != dimensions[i + 1][0])
        {
            return false;
        }
    }

    // all is fine dimensions are correct
    return true;
}

// Function: main
int main(int argc, char **argv)
{

    int **dimensions = NULL;
    int TOTALARRAYS = 0;

    srand(time(NULL));

    string file_name = "Dimensions.txt";
    dimensions = read_file(file_name, TOTALARRAYS);

    // checkig if dimension provided are correct are not
    if (!dimensionChecker(dimensions, TOTALARRAYS))
    {
        cout << "\n\n ....... Wrong Dimensions --- Not compatible for multiplication change Dimensions ......\n"
             << endl;

        return 1;
    }

    cout << "\n\nTotal arrays are: " << TOTALARRAYS << endl;
    cout << "with dimensions as : " << endl;
    printArray(dimensions, TOTALARRAYS, 2);
    // 3d array memory allocation

    // print the 3d array
    // cout << arrays[0][0] << arrays[0][1] << endl;

    // initalizing the array
    // for (int i = 0; i < TOTALARRAYS; i++)
    // {
    //     initArray(array[i], dimensions[i][0], dimensions[i][1]);
    // }

    // printArray(array[0], dimensions[0][0], dimensions[0][1]);

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

    // will use for mappping brackets
    int *mybrak = new int[n * n];

    cout << "\n\n    ..... Chain Matrix Mltiplication ..... "
         << endl;
    cout << "   ........ FInding Optimal Solution ......... \n"
         << endl;

    printf("Optimal Cost: %d\n", MatrixChainOrder(mydimentions, n, mybrak));
    printf("Optimal Parenthesization: ");
    printOptimalParenthesis(1, n - 1, n, mybrak);

    // the evaluation string
    string evaluationFlow = getOPtimalParenthesis(1, n - 1, n, mybrak);

    cout << endl;

    // creating news graphs.csv each time
    system("rm graphs.csv ; touch graphs.csv");

    // invoking the system call to compile the part a code
    // invoking the system call to compile the part a code
    string command = "mpic++ -o partA partA.cpp functions.cpp";
    system(command.c_str());

    cout << "\n\n ::::::::: Running on 2 processes :::::::::\n\n"
         << endl;
    command = "mpirun -np=2 ./partA \"" + evaluationFlow + "\" 2";
    system(command.c_str());
    
    cout << "\n\n ::::::::: Running on 4 processes :::::::::\n\n"
         << endl;
    command = "mpirun -np=4 ./partA \"" + evaluationFlow + "\" 4";
    system(command.c_str());

    cout << "\n\n ::::::::: Running on 6 processes :::::::::\n\n"
         << endl;
    command = "mpirun -np=6 ./partA \"" + evaluationFlow + "\" 6";
    system(command.c_str());

    cout << "\n\n ::::::::: Running on 8 processes :::::::::\n\n"
         << endl;
    command = "mpirun -np=8 ./partA \"" + evaluationFlow + "\" 8";
    system(command.c_str());

    system("exit");

    
    delete[] mybrak;
    delete[] mydimentions;

    return 0;
}
