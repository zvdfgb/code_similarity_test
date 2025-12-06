//
// Created by mmm on 2025/12/6.
//
#ifndef VECTORIZATION_H
#define VECTORIZATION_H

// 1. 宏定义搬家
// 把维度定义在这里，这样 main.c 和 vectorization.c 都能看到同一个数字
// 必须保持和你代码里的逻辑一致 (32个具体 + 3个抽象 = 35)
#define VECTOR_DIMENSION 35

// 2. 导出全局变量 (extern)
// 意思是告诉外界："有个叫 FEATURE_MAP 的字符数组，具体内容在 .c 文件里，你尽管用。"
// 这样你的 main 函数才能打印出 "int: 1次" 这种具体的名称
extern const char *FEATURE_MAP[VECTOR_DIMENSION];

// 3. 函数声明
// 告诉外界：给我一段代码和一个数组，我帮你填满它
void generate_vector(const char *code, int vector[]);

#endif