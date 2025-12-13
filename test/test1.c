#include <stdio.h>

// 计算斐波那契数列的第 n 项
// 这是一个递归实现
int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main() {
    int n = 10;
    // 打印结果
    printf("Fibonacci of %d is %d\n", n, fibonacci(n));
    return 0;
}

