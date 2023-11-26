#include <iostream>

void splitMatrix(int matrix[][5], int rows, int cols, int blocks[][2][2]) {
    int blockIndex = 0;
    for (int i = 0; i < rows; i += 2) {
        for (int j = 0; j < cols; j += 2) {
            for (int k = 0; k < 2 && i + k < rows; ++k) {
                for (int l = 0; l < 2 && j + l < cols; ++l) {
                    blocks[blockIndex][k][l] = matrix[i + k][j + l];
                }
            }
            ++blockIndex;
        }
    }
}
int main() {
    // Example matrix
    int matrix[5][5] = {
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 10},
        {11, 12, 13, 14, 15},
        {16, 17, 18, 19, 20},
        {21, 22, 23, 24, 25}
    };

    // Create an array to store the blocks
    int blocks[25][2][2] = {0};  // Maximum possible number of blocks for a 5x5 matrix

    // Split the matrix into blocks
    splitMatrix(matrix, 5, 5, blocks);

// Calculate the number of blocks
int numBlocks = ((5 + 1) / 2) * ((5 + 1) / 2);

// Print the blocks
for (int i = 0; i < numBlocks; ++i) {
    std::cout << "Block " << i + 1 << ":\n";
    for (int j = 0; j < 2; ++j) {
        for (int k = 0; k < 2; ++k) {
            std::cout << blocks[i][j][k] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}
    // Rest of your code...
    return 0;
}