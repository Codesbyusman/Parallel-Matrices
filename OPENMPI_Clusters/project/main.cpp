// including the basic header files
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>
#include <vector>
#include <set>
#include <climits>
#include <mpi.h>
#include <stack>

extern void processArray(int argc, char *argv[], int rowsA, int colsA, int colsB, int p_rank, int num_process, int **returnarray);

using namespace std;

int TOTALARRAYS = 0;

// function to display array
void printArray(int **arr, int row, int col)
{
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
}

// A function to read the file and return the data from the file
// Function: read_file
int **read_file(string file_name)
{
    ifstream file(file_name);
    string line;
    string data;
    // create an array to return the data
    string mydata[100];

    while (getline(file, line))
    {
        data = line;
        // cout <<data << endl;
        mydata[TOTALARRAYS] = data;
        TOTALARRAYS++;
    }

    // // print the mydata array
    // for (int i = 0; i < 3; i++)
    // {
    //     //   cout << mydata[i] << endl;
    // }

    // making a dynamic array
    int **array = new int *[TOTALARRAYS];
    for (int i = 0; i < TOTALARRAYS; ++i)
        array[i] = new int[2];

    // addint the loop for it
    for (int i = 0; i < TOTALARRAYS; i++)
    {
        // break the mydata each TOTALARRAYS by using  a delimiter
        string delimiter = "X";
        string token = mydata[i].substr(0, mydata[i].find(delimiter));
        // cout << token << endl;
        array[i][0] = stoi(token);
        // After the delimiter
        string token2 = mydata[i].substr(mydata[i].find(delimiter) + 1, mydata[0].length());
        // cout << token2 << endl;
        array[i][1] = stoi(token2);
    }

    // print he array
    /*
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            cout << array[i][j] << " ";
        }
        cout << endl;
    }*/

    // printArray(array, 3, 2);
    return array;
}

// printing the 3d array
void print3dArray(int ***arr, int row, int col, int depth)
{
    for (int i = 0; i < depth; ++i)
    {
        for (int j = 0; j < row; ++j)
        {
            for (int k = 0; k < col; ++k)
            {
                cout << arr[i][j][k] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}

// initalizing the array with random number
void initArray(int **arr, int row, int col)
{
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            arr[i][j] = rand() % 100;
        }
    }
}

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

// Function to get the optimal matrix chain multiplication
string getOptimalParenthesis(int i, int j, int n, int brackets[])
{
    if (i == j)
    {
        return "A" + to_string(i);
    }
    else
    {
        string leftOperation = getOptimalParenthesis(i, brackets[i + j * n], n, brackets);
        string rightOperation = getOptimalParenthesis(brackets[i + j * n] + 1, j, n, brackets);
        return "(" + leftOperation + rightOperation + ")";
    }
}

// a function that will return the dimentions after reading the array
// 2 3
// 3 4
// 4 2
// 2 1
// i want 2 3 4 2 1

int *getDimentions(int **arr, int row, int col)
{
    int index = 0;
    int *mydimention = new int[TOTALARRAYS + 1];

    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            // get the first index of the first array
            if (i == 0 && j == 0)
            {
                mydimention[index] = arr[i][j];
                index++;
                cout << "\n first index:" << arr[i][j] << "\n ";
            }
            // get the second index of the all other arrays
            else if (i != 0 && j == 0)
            {
                mydimention[index] = arr[i][j];
                index++;
                cout << "\n Last index: " << arr[i][j] << "\n ";
            }
            // get the last index of the last array
            else if (i == row - 1 && j == col - 1)
            {
                mydimention[index] = arr[i][j];
                index++;
                cout << "\n Final index: " << arr[i][j] << "\n ";
            }
            else
            {
                // cout << arr[i][j] << " ";
            }
        }
        cout << endl;
    }

    return mydimention;
}

std::stack<std::string> extractOperations(const std::string& s) {
    std::stack<std::string> operations;
    std::string currentOperation;
    int openBrackets = 0;
    for (char c : s) {
        if (c == '(') {
            openBrackets++;
            if (openBrackets > 1) {
                currentOperation += c;
            }
        } else if (c == ')') {
            openBrackets--;
            if (openBrackets >= 1) {
                currentOperation += c;
            }
            if (openBrackets == 1 && !currentOperation.empty()) {
                operations.push(currentOperation);
                currentOperation.clear();
            }
        } else {
            currentOperation += c;
        }
    }
    return operations;
}
// it just print the array but squre only
void printArray(int n, int **mat)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << mat[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

std::stack<std::string> operations;
string file_name = "test.txt";
int **dimensions = read_file(file_name);

// Function: main
int main(int argc, char *argv[])
{
    int p_rank;
    int num_process;

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
    {
        printf("MPI-INIT Failed\n");
        return 0;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &p_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_process);

    // adding a if statement to check the array index
    int first_row = 0;
    int first_col = 0;
    int second_row = 0;
    int second_col = 0;

    if (p_rank == 0)
    {
        printf("Number of processes: %d\n", num_process);

        srand(time(NULL));

        printArray(dimensions, TOTALARRAYS, 2);

        // 3d array memory allocation
        int ***array = new int **[TOTALARRAYS];
        for (int i = 0; i < TOTALARRAYS; ++i)
            array[i] = new int *[dimensions[i][0]];

        for (int i = 0; i < TOTALARRAYS; i++)
        {
            for (int j = 0; j < dimensions[i][0]; j++)
            {

                array[i][j] = new int[dimensions[i][1]];
            }
        }

        // print the 3d array
        // cout << arrays[0][0] << arrays[0][1] << endl;

        // initalizing the array
        for (int i = 0; i < TOTALARRAYS; i++)
        {
            initArray(array[i], dimensions[i][0], dimensions[i][1]);
        }

        // // print the array
        for (int i = 0; i < TOTALARRAYS; i++)
        {
            cout << "Array " << i << endl;
            printArray(array[i], dimensions[i][0], dimensions[i][1]);
            cout << endl;
        }

        // getting the dimentions into the right format as needed
        cout << "Total Dimentions  are: " << TOTALARRAYS + 1 << endl;
        int *mydimentions = getDimentions(dimensions, TOTALARRAYS, 2);

        // printing the mydimentions array
        cout << "\nFetched the Dimentions\n";
        for (int i = 0; i < TOTALARRAYS + 1; i++)
        {
            cout << mydimentions[i] << " ";
        }
        cout << "\n------\n";
        // chain multiplication for the above arrays to get the optimal solution
        // int mydim[] = {2,3,4,2,1};
        int n = TOTALARRAYS + 1;
        cout << "\nn is " << n << "\n";
        int mybrak[n * n] = {0};

        printf("Optimal Cost: %d\n", MatrixChainOrder(mydimentions, n, mybrak));
        printf("Optimal Parenthesization: ");
        string mystring = getOptimalParenthesis(1, n - 1, n, mybrak);
        cout << mystring << endl;

        std::string s = mystring;
        operations = extractOperations(s);
        int index = 0;
        // while (!operations.empty())
        // {
        std::cout << operations.top() << std::endl;
        // Find the middle index
        if (index == 0)
        {
            string input_string = operations.top();
            size_t middle_index = input_string.length() / 2;

            // Split the string into two parts
            std::string first_part = input_string.substr(0, middle_index);
            std::string second_part = input_string.substr(middle_index);

            // Output the results
            std::cout << "First part: " << first_part << std::endl;
            std::cout << "Second part: " << second_part << std::endl;

            // Extract the numeric part from the string (assuming it's always at the end)
            std::string firstnumericPart = first_part.substr(1); // Assuming the string format is always "A" followed by digits
            // Convert the numeric part to an integer using std::atoi
            int firstnumericValue = std::atoi(firstnumericPart.c_str());
            std::cout << "First Numeric value: " << firstnumericValue << std::endl;
            first_row = dimensions[firstnumericValue - 1][0];
            first_col = dimensions[firstnumericValue - 1][1];
            cout << "Rows: " << first_row << " Cols: " << first_col << endl;

            // Extract the numeric part from the string (assuming it's always at the end)
            std::string secondnumericPart = second_part.substr(1); // Assuming the string format is always "A" followed by digits
            // Convert the numeric part to an integer using std::atoi
            int secondnumericValue = std::atoi(secondnumericPart.c_str());
            std::cout << "Second Numeric value: " << secondnumericValue << std::endl;
            second_row = dimensions[secondnumericValue - 1][0];
            second_col = dimensions[secondnumericValue - 1][1];
            cout << "Rows: " << second_row << " Cols: " << second_col << endl;
        }
        else
        {
            string input_string = operations.top();
            size_t middle_index = input_string.length() / 2;

            // Split the string into two parts
            std::string useless = input_string.substr(0, middle_index);
            std::string array_number = input_string.substr(middle_index);

            // Output the results
            std::cout << "\nUseless: " << useless << std::endl;
            std::cout << "\nArray Number: " << array_number << std::endl;

            int next_array_number = std::atoi(array_number.c_str());

            cout << "Rows: " << dimensions[next_array_number - 1][0] << " Cols: " << dimensions[next_array_number - 1][1] << endl;
        }
        index++;
        operations.pop();
        //}
    }

    MPI_Barrier(MPI_COMM_WORLD);
    // processArray(argc, argv, 4, 4, 4, p_rank, num_process);
    // an array to get the results
    // getting the size
   // for(int i=0;i<5;i++){
 
    int squareSize2 = max(first_row, first_col);
    int squareSize = max(squareSize2, second_col);
    int **returnarray = new int *[squareSize];
    for (int i = 0; i < squareSize; i++)
    {
        returnarray[i] = new int[squareSize];
    }
    processArray(argc, argv, first_row, first_col, second_col, p_rank, num_process, returnarray);
    MPI_Barrier(MPI_COMM_WORLD);

    if (p_rank == 0)
    {
        // print the result
        cout << "Finally Result: " << endl;
        printArray(squareSize, returnarray);
    }
    // delete[] mydimentions;
    MPI_Barrier(MPI_COMM_WORLD);

    if (p_rank == 0)
    {
        string input_string = operations.top();
        size_t middle_index = input_string.length() / 2;

        // Split the string into two parts
        std::string useless = input_string.substr(0, middle_index);
        std::string array_number = input_string.substr(middle_index);

        // Output the results
        std::cout << "\nUseless: " << useless << std::endl;
        std::cout << "\nArray Number: " << array_number << std::endl;

        int next_array_number = std::atoi(array_number.c_str());

        cout << "Rows: " << dimensions[next_array_number - 1][0] << " Cols: " << dimensions[next_array_number - 1][1] << endl;

        first_row = dimensions[next_array_number - 1][0];
        first_col = dimensions[next_array_number - 1][1];
        second_col = squareSize;
        operations.pop();
    //}
    MPI_Barrier(MPI_COMM_WORLD);
   }
/*
    processArray(argc, argv, 4, 3, squareSize, p_rank, num_process, returnarray);
    MPI_Barrier(MPI_COMM_WORLD);
    if (p_rank == 0)
    {
        // print the result
        cout << "Finally Result: " << endl;
        printArray(squareSize, returnarray);
    }
    MPI_Barrier(MPI_COMM_WORLD);
*/
    // rows:1,cols:3
    cout << "Hello World!" << endl;
    return 0;
}