#include "functions.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>

using namespace std;

// function to display array
void printArray(double **arr, int row, int col)
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
int **read_file(string file_name, int &TOTALARRAYS)
{
    ifstream file(file_name.c_str());
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
        array[i][0] = atoi(token.c_str());
        // After the delimiter
        string token2 = mydata[i].substr(mydata[i].find(delimiter) + 1, mydata[0].length());
        // cout << token2 << endl;
        array[i][1] = atoi(token2.c_str());
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
void print3dArray(double ***arr, int row, int col, int depth)
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
void initArray(double **arr, int row, int col)
{
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            arr[i][j] = rand() % 10;
        }
    }
}
