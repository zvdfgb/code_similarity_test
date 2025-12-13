#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

void greet() {
    printf("Hello from greet function!\n");
}

int main() {
    // Calling functions in same logic but defined in different order above
    greet();
    int sum = add(5, 3);
    printf("Sum is: %d\n", sum);
    return 0;
}

