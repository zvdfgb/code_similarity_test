# C语言向量化模块学习计划

本文档为希望学习并实现C语言代码相似度检测项目中“向量化”模块的开发者提供一份详细的学习计划和知识点梳理。

### 核心任务

向量化模块的目标是做两件事：
1.  **分词 (Tokenization)**：将一大段纯净的代码文本（例如 `"int main char argv ..."`）切分成一个个独立的单词（`"int"`, `"main"`, `"char"`, `"argv"`, ...）。
2.  **向量化 (Vectorization)**：将这些单词转换成计算机可以计算的数学形式——**词频向量**。

为此，您需要掌握以下知识和工具。

---

### 一、所需知识与工具

#### 1. C语言基础工具 (必备)

-   **字符串处理**:
    -   头文件: `#include <string.h>`
    -   **`strtok_r()`**: 这是完成“分词”任务最理想的函数。它能安全地按指定分隔符（在这里是空格）切分字符串。相比 `strtok()`，它是线程安全的，也是处理嵌套循环分词的首选。
    -   **`strcpy()` / `strdup()`**: 用于复制字符串（例如，将分词后的单词存入你的数据结构）。`strdup` 会自动 `malloc` 内存，更方便但别忘记 `free`。
    -   **`strcmp()`**: 用于比较两个字符串是否相等。

-   **动态内存管理**:
    -   头文件: `#include <stdlib.h>`
    -   **`malloc()` / `calloc()`**: 用于动态申请内存。例如，创建大小不定的哈希表、词频向量，或者存储单词。`calloc` 会将内存初始化为零，很适合创建频率向量。
    -   **`free()`**: 用于释放不再使用的内存，防止内存泄漏。这是C语言编程的重中之重。

-   **结构体 `struct`**:
    -   用于将相关数据打包在一起，是实现后续“哈希表”数据结构的基础。

#### 2. 核心数据结构：哈希表 (Hash Table)

这是本模块的灵魂。为什么需要它？

-   **目的**: 我们需要一个“词汇表”来存放两个代码文件中所有**不重复**的单词。
-   **挑战**: 每当分词得到一个新单词时，我们需要快速判断：
    1.  这个单词是否已经在词汇表里了？
    2.  如果不在，就把它加进去。
-   **为什么用哈希表?**: 如果用普通数组，每次添加新单词都要遍历整个数组来检查是否重复，效率极低 (O(n))。而**哈希表可以让我们以接近 O(1) 的平均时间复杂度完成查找和插入**，速度飞快，非常适合这个场景。

**哈希表的组成部分**:
1.  **哈希函数 (Hash Function)**: 一个“魔术”函数，能将一个字符串（比如 "printf"）转换成一个数字（比如 5381）。这个数字将作为数组的索引。
2.  **数组 (Array of Buckets)**: 哈希表的主体，一个指针数组。哈希函数算出的数字就对应这个数组的某个位置。
3.  **冲突处理 (Collision Handling)**: 有时两个不同的字符串（比如 "apple" 和 "banana"）可能会被哈希函数算成同一个数字。这叫“哈希冲突”。最简单的解决方法是**链表法 (Chaining)**，即把所有哈希值相同的元素用链表串在数组的同一个位置上。

---

### 二、向量化模块学习与实现规划

您可以按照以下四个阶段来学习和实践，逐步攻克难关。

#### **阶段一：基础准备 (1-2天)**

-   **目标**: 巩固C语言核心技能，为后续复杂操作打下坚实基础。
-   **任务**:
    1.  复习 `struct` 的定义和使用方法。
    2.  复习 `malloc` 分配内存和 `free` 释放内存，理解什么是指针、指针的指针 (`**`)。
    3.  **实践**: 编写一个简单程序，创建一个 `Person` 结构体（包含 `char* name` 和 `int age`），动态创建一个 `Person` 数组，并填充数据，最后正确释放所有内存。

#### **阶段二：掌握分词工具 (1天)**

-   **目标**: 学会使用 `strtok_r()` 函数准确地切分字符串。
-   **任务**:
    -   阅读 `strtok_r` 的文档，理解它的三个参数：`char* str`, `const char* delim`, `char** saveptr`。
    -   **实践**: 编写一个程序，输入一个包含多个单词的字符串（例如 `char sentence[] = "hello world this is a test";`），使用 `strtok_r` 和 `while` 循环，将每个单词打印出来。

    ```c
    // strtok_r 示例代码
    #include <stdio.h>
    #include <string.h>

    int main() {
        char sentence[] = "int main char argv";
        char* token;
        char* saveptr; // 用于 strtok_r 保存上下文状态

        // 第一次调用，需要传入原字符串
        token = strtok_r(sentence, " ", &saveptr);

        while (token != NULL) {
            printf("单词: %s\n", token);
            // 后续调用，第一个参数传入 NULL，让函数继续处理之前保存的字符串
            token = strtok_r(NULL, " ", &saveptr);
        }
        return 0;
    }
    ```

#### **阶段三：从零实现一个简单的哈希表 (2-3天)**

-   **目标**: 理解并亲手实现哈希表的核心逻辑。这是最关键、最有挑战性的一步。
-   **任务**:
    1.  **定义数据结构** (参考 `development_plan.md`):
        -   `WordNode` 链表节点: `struct WordNode { char* word; int id; struct WordNode* next; };` (建议增加一个 `id` 字段，表示单词在词汇表中的唯一编号)。
        -   `Vocabulary` 哈希表: `struct Vocabulary { WordNode** table; int size; int capacity; };`
    2.  **实现哈希函数**: 你不需要自己发明，用一个经典的字符串哈希函数即可，例如 `djb2`。
        ```c
        // djb2 哈希函数 (unsigned long long 避免溢出)
        unsigned long long hash(const char *str) {
            unsigned long long hash = 5381;
            int c;
            while ((c = *str++)) {
                hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
            }
            return hash;
        }
        ```
    3.  **实现核心功能函数**:
        -   `Vocabulary* create_vocabulary(int capacity)`: `malloc` 哈希表和内部的指针数组，并初始化为 `NULL`。
        -   `void insert_word(Vocabulary* vocab, const char* word)`:
            -   计算 `word` 的哈希值，再对 `capacity` 取模，得到数组索引 `index`。
            -   遍历 `vocab->table[index]` 指向的链表，检查单词是否已存在。
            -   如果不存在，则创建一个新的 `WordNode` (使用 `strdup` 复制 `word`，并分配一个新的 `id`)，然后将此新节点插入到链表的头部。
        -   `int find_word_index(Vocabulary* vocab, const char* word)`: 查找一个单词在词汇表中的 `id`。如果找不到，返回 -1。
        -   `void free_vocabulary(Vocabulary* vocab)`: 释放整个哈希表（包括所有链表节点和用 `strdup` 复制的单词字符串）。

#### **阶段四：整合为向量化模块 (1-2天)**

-   **目标**: 将你实现的哈希表和你学到的分词技巧组合起来，完成 `vectorizer.c` 的功能。
-   **任务**:
    1.  **实现 `build_vocabulary()`**:
        -   创建一个哈希表实例 (`create_vocabulary`)。
        -   使用 `strtok_r` 分别对两个代码文本 `textA` 和 `textB` 进行分词。
        -   对于得到的每个单词，调用你写的 `insert_word()` 函数将其加入哈希表。
    2.  **实现 `create_frequency_vector()`**:
        -   根据 `vocab->size` 使用 `calloc` 创建一个全为0的词频向量 `int* vector`。
        -   再次使用 `strtok_r` 对单个代码文本进行分词。
        -   对于得到的每个单词，调用 `find_word_index()` 在哈希表中找到它的唯一ID。
        -   如果找到了ID (不为-1)，则将 `vector[ID]` 的值加1。
        -   返回这个 `vector`。

---

### 总结建议

-   **不要急于求成**: 严格按照阶段来，确保前一个阶段的知识你已经通过实践掌握了，再进入下一个阶段。
-   **多写测试**: 每完成一个小函数（如 `insert_word`），就编写一个小的 `main` 函数来测试它，确保它能正确工作。
-   **调试**: 学会使用 `printf` 或 GDB 等调试工具来查看程序中间状态，定位问题。
-   **参考资料**: 在网上搜索“C语言实现哈希表”、“C语言字符串处理”等关键词，可以找到大量优秀的教程和代码示例作为参考。

这个过程不仅能让你完成项目，更能让你对C语言的内存管理、数据结构和算法有一次脱胎换骨的深入理解。祝你学习顺利！
