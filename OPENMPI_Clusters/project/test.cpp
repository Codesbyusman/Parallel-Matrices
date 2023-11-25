#include <iostream>
#include <algorithm>

const int rows = 3; // Set the number of rows
const int cols = 4; // Set the number of columns

int main() {
    // Original 2D array
    int originalArray[rows][cols] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    // Create a temporary array with the same dimensions
    int tempArray[rows][cols];

    // Use std::copy to copy the elements
    std::copy(&originalArray[0][0], &originalArray[0][0] + rows * cols, &tempArray[0][0]);

    // Display the contents of the temporary array
    std::cout << "Temporary Array:" << std::endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << tempArray[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
