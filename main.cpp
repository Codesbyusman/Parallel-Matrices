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

// the chain matrix multiplication function
// will get the optimal solution for multiplying
void chainMatrixMultiplication(int arr[], int size)
{
    
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
    // for (int i = 0; i < TOTALARRAYS; i++)
    // {
    //     cout << "Array " << i + 1 << endl;
    //     printArray(array[i], dimensions[i][0], dimensions[i][1]);
    //     cout << endl;
    // }

    cout << "Hello World!" << endl;
    return 0;
}