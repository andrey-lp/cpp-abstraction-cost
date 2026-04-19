#include <iostream>
#include <vector>
#include <chrono>

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
[[gnu::noinline]]
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
[[gnu::noinline]]
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

// Procedural implementation of the optimized i-k-j matrix multiplication on a 1D Flat Array
// Focus for students: Notice the difference between vector<vector<int>> (array of pointers) and vector<int> (contiguous memory)
[[gnu::noinline]]
void multiplyMatrixFlat(const vector<int>& A, const vector<int>& B, vector<int>& C, int n) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            int a_ik = A[i * n + k];
            for (int j = 0; j < n; j++) {
                C[i * n + j] += a_ik * B[k * n + j];
            }
        }
    }
}

int main(int argc, char* argv[]) {
    string mode = "all";
    if (argc > 1) mode = argv[1];
    // Demonstration on a large 2048x2048 matrix to see the real speed difference
    // Size is chosen to exceed typical LLC (Last Level Cache) to demonstrate RAM fetch performance.
    int n = 2048;
    
    // Initialize matrices A and B with 1s, and C with 0s
    // A vector of vectors is used to represent a 2D matrix
    // Note: Creating large matrices takes a moment
    cout << "Initializing matrices of size " << n << "x" << n << "..." << endl;
    vector<vector<int>> A(n, vector<int>(n, 1));
    vector<vector<int>> B(n, vector<int>(n, 1));
    vector<vector<int>> C1(n, vector<int>(n, 0));
    vector<vector<int>> C2(n, vector<int>(n, 0));

    // Allocate 1D arrays
    vector<int> flatA(n * n, 1);
    vector<int> flatB(n * n, 1);
    vector<int> flatC(n * n, 0);
    
    chrono::duration<double> diff_ijk, diff_ikj, diff_flat;

    // Run the classic unoptimized method
    if (mode == "all" || mode == "ijk") {
        cout << "\nRunning Classic i-j-k Method..." << endl;
        auto start_ijk = chrono::high_resolution_clock::now();
        multiplyMatrixIJK(A, B, C1);
        auto end_ijk = chrono::high_resolution_clock::now();
        diff_ijk = end_ijk - start_ijk;
        cout << "Time taken (i-j-k): " << diff_ijk.count() << " seconds" << endl;
    }
    
    // Run the optimized method
    if (mode == "all" || mode == "ikj") {
        cout << "\nRunning Optimized i-k-j Method..." << endl;
        auto start_ikj = chrono::high_resolution_clock::now();
        multiplyMatrixIKJ(A, B, C2);
        auto end_ikj = chrono::high_resolution_clock::now();
        diff_ikj = end_ikj - start_ikj;
        cout << "Time taken (i-k-j): " << diff_ikj.count() << " seconds" << endl;
    }
    
    // Run the 1D Flat Array method
    if (mode == "all" || mode == "flat") {
        cout << "\nRunning Optimized i-k-j on Flat 1D Array..." << endl;
        auto start_flat = chrono::high_resolution_clock::now();
        multiplyMatrixFlat(flatA, flatB, flatC, n);
        auto end_flat = chrono::high_resolution_clock::now();
        diff_flat = end_flat - start_flat;
        cout << "Time taken (Flat 1D): " << diff_flat.count() << " seconds" << endl;
    }
    
    if (mode == "all") {
        cout << "\nSpeedup (IJK vs IKJ 2D): " << diff_ijk.count() / diff_ikj.count() << "x faster!" << endl;
        cout << "Speedup (IJK vs Flat 1D): " << diff_ijk.count() / diff_flat.count() << "x faster!" << endl;
    }
    cout << "Checksum (to prevent optimizer from skipping loops): " << C1[n-1][n-1] + C2[n-1][n-1] + flatC[n * n - 1] << endl;
    
    return 0;
}
