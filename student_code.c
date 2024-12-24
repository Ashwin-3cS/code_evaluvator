#include <stdio.h>

// Factorial function implementation
int factorial(int n) {
    if (n == 0 || n == 1) return 1;
    return n * factorial(n - 1);
}

int main() {
    int n;
    // Read input number
    if (scanf("%d", &n) != 1) {
        printf("Invalid input\n");
        return 1; // Return error code if input is invalid
    }

    if (n < 0) {
        printf("Error: Factorial of a negative number is undefined.\n");
        return 1; // Return error code for invalid input
    }

    // Compute and print factorial
    printf("%d\n", factorial(n));
    return 0;
}