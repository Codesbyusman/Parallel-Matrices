#include <iostream>

using namespace std;
// Declaration of the function in helper.cpp
extern void processArray(int argc, char *argv[], int rowsA,int colsA,int colsB);

int main(int argc, char *argv[]) {
  

    // Call the function in helper.cpp
    processArray(argc, argv, 4, 4, 4);

    cout << "Hello from main.cpp!" << endl;

    return 0;
}
