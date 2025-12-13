#include <stdio.h>

void greet() {
    printf("Hello from greet function!\n");
}

int add(int a, int b) {
    return a + b;
}

int main() {
    greet();
    int sum = add(5, 3);
    printf("Sum is: %d\n", sum);
    return 0;
}

