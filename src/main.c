#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// 引入各模块头文件
#include "preprocess.h"
#include "tokenization.h"
#include "vectorization.h"
#include "calculate.h"

// 打印使用说明
void print_usage(const char *program_name) {
    fprintf(stderr, "用法: %s <文件1路径> <文件2路径>\n", program_name);
    fprintf(stderr, "例如: %s test/test1.c test/test2.c\n", program_name);
}

// 评估相似度得分并输出结论
void evaluate_similarity(double score) {
    printf("\n--- 评估结果 ---\n");
    printf("代码相似度得分: %.4f (%.2f%%)\n", score, score * 100.0);

    if (score >= 0.9) {
        printf("结论: [极高] 两份代码高度相似，极有可能存在直接抄袭。\n");
    } else if (score >= 0.75) {
        printf("结论: [高] 两份代码非常相似，结构和逻辑基本一致。\n");
    } else if (score >= 0.5) {
        printf("结论: [中] 两份代码有一定相似性，可能复用了部分逻辑或结构。\n");
    } else {
        printf("结论: [低] 两份代码差异较大，相似度较低。\n");
    }
}

int main(int argc, char *argv[]) {
    // 1. 检查参数
    if (argc != 3) {
        print_usage(argv[0]);
        return 1;
    }

    const char *file1_path = argv[1];
    const char *file2_path = argv[2];

    printf("--- C语言代码相似度检测系统 ---\n");
    printf("正在比较:\n  文件 A: %s\n  文件 B: %s\n\n", file1_path, file2_path);

    // 变量声明
    char *clean_code_A = NULL;
    char *clean_code_B = NULL;
    int vector_A[VECTOR_DIMENSION];
    int vector_B[VECTOR_DIMENSION];
    double similarity = 0.0;
    int exit_code = 0;

    // 2. 预处理 (Preprocess)
    printf("[1/3] 正在预处理代码...\n");
    clean_code_A = preprocess_file(file1_path);
    if (!clean_code_A) {
        fprintf(stderr, "错误: 无法预处理文件 '%s'。\n", file1_path);
        exit_code = 1;
        goto cleanup;
    }
    
    clean_code_B = preprocess_file(file2_path);
    if (!clean_code_B) {
        fprintf(stderr, "错误: 无法预处理文件 '%s'。\n", file2_path);
        exit_code = 1;
        goto cleanup;
    }
    printf("      预处理完成。\n");

    // 3. 向量化 (Vectorization)
    // 注意：generate_vector 内部会调用分词器 (tokenization)
    printf("[2/3] 正在生成特征向量...\n");
    generate_vector(clean_code_A, vector_A);
    generate_vector(clean_code_B, vector_B);
    printf("      向量生成完成。\n");

    // 4. 计算相似度 (Calculation)
    printf("[3/3] 正在计算余弦相似度...\n");
    similarity = calculate_cosine_similarity(vector_A, vector_B, VECTOR_DIMENSION);
    
    // 5. 输出结果
    evaluate_similarity(similarity);

cleanup:
    // 内存清理
    if (clean_code_A) free(clean_code_A);
    if (clean_code_B) free(clean_code_B);

    return exit_code;
}
