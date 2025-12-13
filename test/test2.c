#include <stdio.h>

/* 
 * This function calculates the nth Fibonacci number.
 * It uses a recursive approach.
 */
int fibonacci(int n) {
    if (n <= 1) return n; // Base case
    return fibonacci(n - 1) + fibonacci(n - 2); // Recursive step
}

int main() {
    int n = 10;
    /* Output the calculation result to the console */
    printf("Fibonacci of %d is %d\n", n, fibonacci(n));
    return 0;
}

