#include <iostream>
#include <vector>

using namespace std;

// Function to print a 3x3 matrix for demonstration
// This helps students visualize the matrices
void printMatrix(const vector<vector<int>>& mat) {
    int rows = min(3, (int)mat.size());
    int cols = min(3, (int)mat[0].size());
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << mat[i][j] << " ";
        }
        cout << endl;
    }
}

// Procedural implementation of the classic i-j-k matrix multiplication
// This algorithm multiplies Matrix A and Matrix B, saving the result in Matrix C.
// Focus for students: Notice the order of the loops: i, then j, then k.
void multiplyMatrixIJK(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C) {
    int n = A.size();
    
    // Loop through the rows of Matrix A
    for (int i = 0; i < n; i++) {
        // Loop through the columns of Matrix B
        for (int j = 0; j < n; j++) {
            // Loop through the elements to calculate the dot product
            for (int k = 0; k < n; k++) {
                // Here, B is accessed column by column: B[0][j], B[1][j], B[2][j]...
                // This causes "Cache Misses" because elements in a column are far apart in memory!
                C[i][j] = C[i][j] + (A[i][k] * B[k][j]);
            }
        }
    }
}

// Procedural implementation of the optimized i-k-j matrix multiplication
// This algorithm does exactly the same math, but changes the order of the loops.
// Focus for students: Notice the order of the loops: i, then k, then j.
void multiplyMatrixIKJ(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C) {
    int n = A.size();
    
    // Loop through the rows of Matrix A
    for (int i = 0; i < n; i++) {
        // Loop through the columns of Matrix A / rows of Matrix B
        for (int k = 0; k < n; k++) {
            // Here, A[i][k] is a constant for the innermost loop.
            // Loop through the columns of Matrix B
            for (int j = 0; j < n; j++) {
                // Here, B and C are accessed row by row: B[k][0], B[k][1], B[k][2]...
                // Elements in a row are next to each other in memory.
                // This causes "Cache Hits", making the CPU read memory extremely fast!
                C[i][j] = C[i][j] + (A[i][k] * B[k][j]);
            }
        }
    }
}

int main() {
    // Demonstration on a small 3x3 matrix for 1st-year students
    int n = 3;
    
    // Initialize matrices A and B with 1s, and C with 0s
    // A vector of vectors is used to represent a 2D matrix
    vector<vector<int>> A(n, vector<int>(n, 1));
    vector<vector<int>> B(n, vector<int>(n, 1));
    vector<vector<int>> C1(n, vector<int>(n, 0));
    vector<vector<int>> C2(n, vector<int>(n, 0));
    
    // Run the classic unoptimized method
    cout << "Running Classic i-j-k Method..." << endl;
    multiplyMatrixIJK(A, B, C1);
    cout << "Result C1:" << endl;
    printMatrix(C1);
    
    // Run the optimized method
    cout << "\nRunning Optimized i-k-j Method..." << endl;
    multiplyMatrixIKJ(A, B, C2);
    cout << "Result C2:" << endl;
    printMatrix(C2);
    
    return 0;
}
