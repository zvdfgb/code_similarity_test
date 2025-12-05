# 项目规划：C语言代码相似度检测系统

本规划是根据 `readme.md` 文件内容，为您的课程设计提供的详细开发方案。它涵盖了文件结构、函数定义、核心功能实现以及 `main.c` 的编写思路，旨在帮助您系统地构建代码。

---

### 1. 文件结构规划

为了代码的清晰和可维护性，建议您创建以下文件结构：

```
.
├── include/              // 存放头文件
│   ├── preprocess.h
│   ├── vectorizer.h
│   └── similarity.h
├── src/                  // 存放源文件
│   ├── main.c
│   ├── preprocess.c
│   ├── vectorizer.c
│   └── similarity.c
├── tests/                // 存放用于测试的代码文件（例如 test1_a.c, test1_b.c 等）
├── readme.md             // 项目说明
└── Makefile (或 compile.sh) // 编译脚本
```

-   **`include/` 目录**：存放所有模块的头文件 (`.h`)，用于声明函数和数据结构。
-   **`src/` 目录**：存放所有模块的实现文件 (`.c`)。
-   **`main.c`**: 程序的入口，负责调用其他模块，控制整体流程。
-   **`preprocess.c/.h`**: 代码预处理模块。
-   **`vectorizer.c/.h`**: 代码向量化模块，包括词汇表构建和频率向量生成。
-   **`similarity.c/.h`**: 余弦相似度计算模块。

---

### 2. 各模块核心函数设计与功能

#### **A. 预处理模块 (`preprocess.c` / `preprocess.h`)**

这个模块负责将原始代码“清洗”干净，移除对相似度计算无影响或有干扰的元素。

-   **头文件 (`preprocess.h`)**:

    ```c
    #ifndef PREPROCESS_H
    #define PREPROCESS_H

    // 函数：预处理源代码文件
    // 功能：读取文件，移除注释、#指令、多余空白和字符串字面量，并转为小写
    // 输入：文件路径 (const char*)
    // 输出：处理后的纯净代码字符串 (char*)，需在外部释放内存
    char* preprocess_file(const char* filepath);

    #endif
    ```

-   **源文件 (`preprocess.c`)**:
    -   **`char* preprocess_file(const char* filepath)`**
        -   **实现思路**：
            1.  使用 `fopen` 和 `fread` 读取整个文件内容到一个大的字符缓冲区中。
            2.  创建一个新的缓冲区用于存放结果。
            3.  遍历源缓冲区，逐字符进行判断和复制：
                -   使用一个状态标志（如 `in_multiline_comment`）来识别并跳过多行注释 (`/* ... */`)。
                -   识别并跳过单行注释 (`// ...`)。
                -   识别并跳过预处理指令（以 `#` 开头到行尾）。
                -   识别并跳过字符串字面量（以 `"` 开始到下一个 `"` ）。
                -   将所有大写字母用 `tolower()` 转换为小写。
                -   将所有空白字符（`\n`, `\t`, `\r` 及多个空格）统一替换为单个空格。
            4.  返回结果字符串的指针。**切记**，这个字符串是动态分配的 (`malloc`)，使用完毕后需要 `free`。

#### **B. 向量化模块 (`vectorizer.c` / `vectorizer.h`)**

这个模块是核心，负责将文本转换为数学向量。推荐使用哈希表来管理词汇表，以提高效率。

-   **头文件 (`vectorizer.h`)**:

    ```c
    #ifndef VECTORIZER_H
    #define VECTORIZER_H

    // 词汇表节点的结构体（用于哈希表）
    typedef struct WordNode {
        char* word;
        struct WordNode* next; // 处理哈希冲突
    } WordNode;

    // 词汇表/哈希表的结构体
    typedef struct Vocabulary {
        WordNode** table; // 哈希表本体
        int size;         // 词汇表中的单词总数
        int capacity;     // 哈希表的容量
    } Vocabulary;

    // 函数：构建词汇表
    // 功能：整合两个代码字符串中的所有唯一单词
    // 输入：两个预处理过的代码字符串 (const char* textA, const char* textB)
    // 输出：指向词汇表数据结构的指针 (Vocabulary*)
    Vocabulary* build_vocabulary(const char* textA, const char* textB);

    // 函数：创建词频向量
    // 功能：根据给定代码字符串和全局词汇表，计算每个词汇的出现频率，生成频率向量
    // 输入：一个代码字符串 (const char* text), 词汇表 (const Vocabulary* vocab)
    // 输出：一个整型数组（频率向量），动态分配 (int*)
    int* create_frequency_vector(const char* text, const Vocabulary* vocab);

    // 函数：释放词汇表内存
    // 功能：释放词汇表及其内部所有动态分配的内存
    // 输入：词汇表指针 (Vocabulary* vocab)
    void free_vocabulary(Vocabulary* vocab);

    #endif
    ```

-   **源文件 (`vectorizer.c`)**:
    -   **`Vocabulary* build_vocabulary(...)`**
        -   **实现思路**：
            1.  初始化一个 `Vocabulary` 哈希表结构。
            2.  使用 `strtok_r` (线程安全版 `strtok`) 按空格分割 `textA` 和 `textB`，得到一个个单词 (token)。
            3.  对每个单词，计算其哈希值，并在哈希表中查找。如果不存在，则将其插入到哈希表中，并增加 `vocab->size`。
    -   **`int* create_frequency_vector(...)`**
        -   **实现思路**:
            1.  使用 `calloc` 创建一个大小为 `vocab->size` 的全零整数数组（向量）。
            2.  再次使用 `strtok_r` 分割 `text`，得到每个单词。
            3.  对于每个单词，在 `vocab` 哈希表中查找它的索引（即它在词汇表中的位置）。
            4.  将频率向量中对应索引位置的值加一。
    -   **`void free_vocabulary(...)`**
        -   **实现思路**: 遍历哈希表，释放所有 `WordNode` 和它们包含的 `word` 字符串，最后释放哈希表本身。

#### **C. 相似度计算模块 (`similarity.c` / `similarity.h`)**

这个模块负责根据余弦相似度公式计算两个频率向量之间的相似度得分。

-   **头文件 (`similarity.h`)**:

    ```c
    #ifndef SIMILARITY_H
    #define SIMILARITY_H

    #include <math.h> // 需要用到 sqrt 函数

    // 函数：计算余弦相似度
    // 功能：根据余弦相似度公式，计算两个频率向量之间的相似度得分
    // 输入：两个词频向量 (const int* vecA, const int* vecB) 及其大小 (int size)
    // 输出：0到1之间的相似度得分 (double)
    double calculate_cosine_similarity(const int* vecA, const int* vecB, int size);

    #endif
    ```

-   **源文件 (`similarity.c`)**:
    -   **`double calculate_cosine_similarity(...)`**
        -   **实现思路**:
            1.  初始化三个变量：`dot_product = 0.0` (点积), `norm_a = 0.0` (A的模), `norm_b = 0.0` (B的模)。
            2.  遍历向量（从 `i = 0` 到 `size - 1`）：
                -   `dot_product += vecA[i] * vecB[i];`
                -   `norm_a += (double)vecA[i] * vecA[i];` (注意类型转换，避免溢出)
                -   `norm_b += (double)vecB[i] * vecB[i];` (注意类型转换)
            3.  计算最终的模：`norm_a = sqrt(norm_a);` 和 `norm_b = sqrt(norm_b);`
            4.  如果 `(norm_a * norm_b)` 不为零，则返回 `dot_product / (norm_a * norm_b)`，否则返回 `0.0` (避免除以零)。

---

### 3. `main.c` 主函数编写指南

`main.c` 是整个程序的“指挥官”，负责流程控制和用户交互。它将依次调用上述模块完成任务。

```c
#include <stdio.h>    // 标准输入输出
#include <stdlib.h>   // 内存分配和通用工具
#include <string.h>   // 字符串处理

// 引入自定义模块的头文件
#include "preprocess.h"
#include "vectorizer.h"
#include "similarity.h"

// 函数：评估抄袭风险
// 功能：根据计算出的相似度得分和预设的阈值，给出评估结果并打印到控制台
// 输入：相似度得分 (double score)
// 输出：无 (void)
void evaluate_plagiarism(double score) {
    printf("\n--- 评估结果 ---\n");
    printf("代码相似度得分: %.4f\n", score);
    if (score >= 0.9) {
        printf("结论: 高度相似，可能存在抄袭风险。\n");
    } else if (score >= 0.7) {
        printf("结论: 比较相似，建议进行人工审查。\n");
    } else {
        printf("结论: 相似度较低。\n");
    }
}

int main(int argc, char *argv[]) {
    // 1. 检查命令行参数数量
    if (argc != 3) {
        fprintf(stderr, "用法错误: %s <文件1路径> <文件2路径>\n", argv[0]);
        return 1; // 返回非零表示错误
    }

    // 2. 初始化所有可能需要动态分配内存的指针为 NULL
    char *clean_text_A = NULL;
    char *clean_text_B = NULL;
    Vocabulary *vocab = NULL;
    int *vector_A = NULL;
    int *vector_B = NULL;
    double similarity_score = 0.0; // 初始化相似度得分

    // --- 主要业务逻辑 ---

    printf("--- 代码相似度检测系统 --- \n");
    printf("正在检测文件: '%s' 和 '%s'\n\n", argv[1], argv[2]);

    // 3. 调用 preprocess_file 对两个输入文件进行预处理
    printf("步骤 1/3: 正在预处理代码文件...\n");
    clean_text_A = preprocess_file(argv[1]);
    clean_text_B = preprocess_file(argv[2]);

    // 错误处理：检查预处理是否成功
    if (!clean_text_A) {
        fprintf(stderr, "错误: 文件 '%s' 预处理失败或文件不存在。\n", argv[1]);
        goto cleanup; // 跳转到内存清理部分
    }
    if (!clean_text_B) {
        fprintf(stderr, "错误: 文件 '%s' 预处理失败或文件不存在。\n", argv[2]);
        goto cleanup; // 跳转到内存清理部分
    }
    printf("预处理完成。\n\n");

    // 4. 调用 build_vocabulary 构建词汇表，并调用 create_frequency_vector 生成词频向量
    printf("步骤 2/3: 正在构建词汇表和生成词频向量...\n");
    vocab = build_vocabulary(clean_text_A, clean_text_B);
    // 错误处理：检查词汇表是否成功构建
    if (!vocab) {
        fprintf(stderr, "错误: 词汇表构建失败。\n");
        goto cleanup;
    }

    vector_A = create_frequency_vector(clean_text_A, vocab);
    vector_B = create_frequency_vector(clean_text_B, vocab);
    // 错误处理：检查向量是否成功生成
    if (!vector_A || !vector_B) {
        fprintf(stderr, "错误: 词频向量生成失败。\n");
        goto cleanup;
    }
    printf("词汇表和词频向量生成完成 (词汇量: %d)。\n\n", vocab->size);


    // 5. 调用 calculate_cosine_similarity 计算相似度得分
    printf("步骤 3/3: 正在计算余弦相似度...\n");
    similarity_score = calculate_cosine_similarity(vector_A, vector_B, vocab->size);
    printf("相似度计算完成。\n\n");

    // 6. 调用 evaluate_plagiarism 打印评估结果
    evaluate_plagiarism(similarity_score);

// --- 内存清理部分 ---
cleanup:
    printf("\n--- 清理内存 ---\n");
    if (clean_text_A) {
        free(clean_text_A);
        clean_text_A = NULL;
    }
    if (clean_text_B) {
        free(clean_text_B);
        clean_text_B = NULL;
    }
    if (vector_A) {
        free(vector_A);
        vector_A = NULL;
    }
    if (vector_B) {
        free(vector_B);
        vector_B = NULL;
    }
    if (vocab) {
        free_vocabulary(vocab);
        vocab = NULL;
    }
    printf("所有动态内存已释放。程序退出。\n");

    return 0; // 成功完成
}
```

---

### 4. 编译与运行

当您写好所有代码后，需要进行编译。为了简化这个过程，建议创建一个 `Makefile` 文件。

**`Makefile` 示例**:

```makefile
# 定义编译器和编译选项
CC = gcc
# -Wall -Wextra 开启所有警告，-Iinclude 指定头文件搜索路径，-std=c11 使用 C11 标准
CFLAGS = -Wall -Wextra -Iinclude -std=c11
# LDFLAGS 用于链接库，-lm 链接数学库（因为 calculate_cosine_similarity 中使用了 sqrt）
LDFLAGS = -lm

# 定义源文件和目标文件
SRCS = src/main.c src/preprocess.c src/vectorizer.c src/similarity.c
# 将所有 .c 源文件转换为 .o 目标文件
OBJS = $(SRCS:.c=.o)
# 定义最终可执行文件的名称
EXECUTABLE = code_similarity_checker

# 默认目标：构建所有
all: $(EXECUTABLE)

# 构建可执行文件：链接所有目标文件
$(EXECUTABLE): $(OBJS)
	@echo "Linking $(EXECUTABLE)..."
	$(CC) $(OBJS) -o $(EXECUTABLE) $(LDFLAGS)
	@echo "Build successful: $(EXECUTABLE)"

# 编译 .c 文件为 .o 文件
# $< 代表第一个先决条件 (即 .c 文件)，$@ 代表目标 (即 .o 文件)
%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# 清理目标：删除所有生成的目标文件和可执行文件
clean:
	@echo "Cleaning up..."
	rm -f $(OBJS) $(EXECUTABLE)
	@echo "Clean complete."

.PHONY: all clean
```

-   **编译步骤**:
    1.  在项目根目录下创建上述 `Makefile` 文件。
    2.  打开终端或命令行，进入项目根目录。
    3.  运行 `make` 命令。

-   **运行步骤**:
    1.  编译成功后，会在项目根目录生成一个名为 `code_similarity_checker` (在Windows上可能是 `code_similarity_checker.exe`) 的可执行文件。
    2.  运行命令，并传入两个代码文件的路径作为参数：
        ```bash
        ./code_similarity_checker tests/file1.c tests/file2.c
        ```
        (请确保 `tests/file1.c` 和 `tests/file2.c` 是您准备好的测试文件路径)

这份规划为您提供了一个详细的开发框架，您可以按照这个结构逐步填充代码，并利用 `Makefile` 方便地编译和管理项目。祝您课程设计顺利！
