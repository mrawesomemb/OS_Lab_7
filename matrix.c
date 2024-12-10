#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 20  // Size of the matrix

int matA[MAX][MAX]; 
int matB[MAX][MAX]; 

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX]; 
int matProductResult[MAX][MAX]; 

// Structure to hold thread arguments
typedef struct {
    int row;
    int col;
} ThreadArgs;

void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i < MAX; i++) {
        for(int j = 0; j < MAX; j++) {
            matrix[i][j] = rand() % 10 + 1;  // Random values between 1 and 10
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i < MAX; i++) {
        for(int j = 0; j < MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Function to compute the sum of the matrices
void* computeSum(void* args) {
    ThreadArgs* tArgs = (ThreadArgs*) args;
    int row = tArgs->row;
    int col = tArgs->col;
    matSumResult[row][col] = matA[row][col] + matB[row][col];
    free(args); // Free allocated memory for thread arguments
    pthread_exit(NULL);  // Terminate thread
}

// Function to compute the difference of the matrices
void* computeDiff(void* args) {
    ThreadArgs* tArgs = (ThreadArgs*) args;
    int row = tArgs->row;
    int col = tArgs->col;
    matDiffResult[row][col] = matA[row][col] - matB[row][col];
    free(args); // Free allocated memory for thread arguments
    pthread_exit(NULL);  // Terminate thread
}

// Function to compute the dot product of the matrices
void* computeProduct(void* args) {
    ThreadArgs* tArgs = (ThreadArgs*) args;
    int row = tArgs->row;
    int col = tArgs->col;
    matProductResult[row][col] = 0;
    
    // Dot product: row of matA and column of matB
    for (int k = 0; k < MAX; k++) {
        matProductResult[row][col] += matA[row][k] * matB[k][col];
    }

    free(args); // Free allocated memory for thread arguments
    pthread_exit(NULL);  // Terminate thread
}

int main() {
    srand(time(0));  // Initialize random seed
    
    // 0. Get the matrix size from the command line and assign it to MAX
    // Here, MAX is predefined to be 20.

    // 1. Fill the matrices (matA and matB) with random values.
    fillMatrix(matA);
    fillMatrix(matB);

    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);
    
    // 3. Create pthread_t objects for our threads.
    pthread_t threads[MAX][MAX];

    // 4. Create a thread for each cell of each matrix operation.
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            // Arguments for each thread
            ThreadArgs* args = (ThreadArgs*) malloc(sizeof(ThreadArgs));
            args->row = i;
            args->col = j;
            
            // Create a thread for matrix sum
            pthread_create(&threads[i][j], NULL, computeSum, (void*) args);
        }
    }
    
    // Wait for all sum threads to finish
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            pthread_join(threads[i][j], NULL);
        }
    }

    // Repeat the process for matrix difference
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            ThreadArgs* args = (ThreadArgs*) malloc(sizeof(ThreadArgs));
            args->row = i;
            args->col = j;
            pthread_create(&threads[i][j], NULL, computeDiff, (void*) args);
        }
    }
    
    // Wait for all difference threads to finish
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            pthread_join(threads[i][j], NULL);
        }
    }

    // Repeat the process for matrix product
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            ThreadArgs* args = (ThreadArgs*) malloc(sizeof(ThreadArgs));
            args->row = i;
            args->col = j;
            pthread_create(&threads[i][j], NULL, computeProduct, (void*) args);
        }
    }
    
    // Wait for all product threads to finish
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            pthread_join(threads[i][j], NULL);
        }
    }

    // 6. Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);

    return 0;
}
