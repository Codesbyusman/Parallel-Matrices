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
#include <set>
#include <climits>
#include <vector>
#include <string>  // Add this line for 'stoi'


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
        array[i][0] = std::stoi(token);        // After the delimiter
        string token2 = mydata[i].substr(mydata[i].find(delimiter) + 1, mydata[0].length());
        // cout << token2 << endl;
        array[i][1] = std::stoi(token2);
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
                    brackets[i + j * n] = k;  // Store the value of k
                }
            }
        }
    }

    return m[1][n - 1];
}

// Function to print the optimal matrix chain multiplication
std::string getOptimalParenthesis(int i, int j, int n, const std::vector<int>& brackets)
{
    if (i == j)
    {
        return "A" + std::to_string(i);
    }
    else
    {
        std::string result = "(";
        result += getOptimalParenthesis(i, brackets[i + j * n], n, brackets);
        result += getOptimalParenthesis(brackets[i + j * n] + 1, j, n, brackets);
        result += ")";
        return result;
    }
}
// a function that will return the dimentions after reading the array
//2 3 
//3 4 
//4 2 
//2 1 
// i want 2 3 4 2 1

int* getDimentions(int **arr, int row, int col)
{   
    int index=0;
    int *mydimention= new int[TOTALARRAYS+1];

    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            // get the first index of the first array
            if(i == 0 && j == 0)
            {
                mydimention[index] = arr[i][j];
                 index++;
                cout << "\n first index:" << arr[i][j] << "\n ";
               
            }
            // get the second index of the all other arrays
            else if(i != 0 && j == 0)
            {
                  mydimention[index] = arr[i][j];
                 index++;
                cout <<"\n Last index: " <<arr[i][j] << "\n ";
            }
            // get the last index of the last array
            else if(i == row - 1 && j == col - 1)
            {   mydimention[index] = arr[i][j];
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


// Function: main
int main()
{
    srand(time(NULL));

    string file_name = "test.txt";
    int **dimensions = read_file(file_name);
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
         cout << "Array " << i + 1 << endl;
         printArray(array[i], dimensions[i][0], dimensions[i][1]);
        cout << endl;
     }

    // getting the dimentions into the right format as needed
    cout << "Total Dimentions  are: " << TOTALARRAYS + 1 << endl;
    int* mydimentions = getDimentions(dimensions, TOTALARRAYS, 2);

    // printing the mydimentions array
    cout << "\nFetched the Dimentions\n";
    for (int i = 0; i < TOTALARRAYS + 1; i++)
    {
        cout << mydimentions[i] << " ";
    }    
    cout << "\n------\n";
    // chain multiplication for the above arrays to get the optimal solution
    //int mydim[] = {2,3,4,2,1};
    int n = TOTALARRAYS+1;
    cout << "\nn is " << n << "\n";
    int mybrak[n * n] = {0};

    printf("Optimal Cost: %d\n", MatrixChainOrder(mydimentions, n, mybrak));
    printf("Optimal Parenthesization: ");
  std::string optimalParenthesis = getOptimalParenthesis(1, n - 1, n, mybrak);

    // Now 'optimalParenthesis' contains the final string
    std::cout << "Optimal Parenthesis: " << optimalParenthesis << std::endl;
dResult' contains the final string
    delete[] mydimentions;

    cout << "Hello World!" << endl;
    return 0;
}