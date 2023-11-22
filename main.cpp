// including the basic header files
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>
#include <set>

using namespace std;

// function to display array
void printArray(int** arr, int row, int col)
{
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            cout << arr[i][j]<<" ";
        }
        cout << endl;
    }
}



// A function to read the file and return the data from the file
// Function: read_file
int** read_file(string file_name)
{
    ifstream file(file_name);
    string line;
    string data;
    // create an array to return the data
    string mydata[100];
    int index = 0;
    while (getline(file, line))
    {
        data = line;
        // cout <<data << endl;
        mydata[index] = data;
        index++;
    }

    // print the mydata array
    for (int i = 0; i < 3; i++)
    {
        //   cout << mydata[i] << endl;
    }

    // making a dynamic array
      int** array = new int*[3];
        for(int i = 0; i < 3; ++i)
            array[i] = new int[2];

 

    // addint the loop for it
    for (int i = 0; i < 3; i++)
    {
        // break the mydata each index by using  a delimiter
        string delimiter = "X";
        string token = mydata[i].substr(0, mydata[i].find(delimiter));
        //cout << token << endl;
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



// Function: main
int main()
{
    string file_name = "test.txt";
    int** data = read_file(file_name);
    printArray(data, 3, 2);

    cout << "Hello World!" << endl;
    return 0;
}