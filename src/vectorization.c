#include <stdio.h>
#include <string.h>
#include "include/tokenization.h" // 【重要】必须引入头文件，才能连接到你的分词器

// 第一部分：全局配置 (特征表)
// 向量维度：决定了我们一共统计多少种特征
// 35维 = 32个具体词 + 3个抽象类
#define VECTOR_DIMENSION 35

// 【特征映射表 (Feature Map)】
// 这是一个“字典”，它的下标(0,1,2...)对应向量数组的下标。
// 比如：FEATURE_MAP[0] 是 "int"，那么 vector[0] 就专门记录 int 的个数。
const char *FEATURE_MAP[VECTOR_DIMENSION] = {
        // [下标 0-14] C语言常见关键字
        "int", "float", "double", "char", "void", "return", "if", "else",
        "for", "while", "do", "break", "continue", "switch", "case",

        // [下标 15-31] 常见运算符和符号
        "=", "+", "-", "*", "/", "%", "++", "--",
        "==", "!=", ">", "<", ">=", "<=", ";", "&&", "||",

        // [下标 32-34] 三大抽象分类 (防作弊核心！)
        "<变量名>",  // index 32: 所有的变量名(age, score...)都算到这里
        "<数  字>",  // index 33: 所有的数字(10, 3.14...)都算到这里
        "<字符串>"   // index 34: 所有的字符串("hello"...)都算到这里
};

// 给最后这三个特殊的下标起个名字，方便下面代码里写
#define IDX_ID      32
#define IDX_NUM     33
#define IDX_STR     34


// 第二部分：查表函数 (Token -> 数组下标)


/**
 * 函数名：get_feature_index
 * 作用：拿到一个 Token，查查它应该放在向量的第几号格子里。
 * 返回值：0 ~ 34 表示下标；-1 表示忽略。
 */
int get_feature_index(Token *token) {

    //1. 优先处理三大抽象类

    // 如果是变量名，不管叫什么，统一归类到 IDX_ID (32号)
    if (token->type == TOKEN_IDENTIFIER) return IDX_ID;

    // 如果是数字，统一归类到 IDX_NUM (33号)
    if (token->type == TOKEN_NUMBER)     return IDX_NUM;

    // 如果是字符串，统一归类到 IDX_STR (34号)
    if (token->type == TOKEN_STRING)     return IDX_STR;

    // --- 2. 处理具体的关键字和符号 ---

    // 遍历特征表的前 32 个元素
    for (int i = 0; i < 32; i++) {
        // strcmp 返回 0 表示字符串一样
        if (strcmp(token->value, FEATURE_MAP[i]) == 0) {
            return i; // 找到了！返回下标
        }
    }

    // --- 3. 没找到 ---
    return -1; // 忽略
}



// 第三部分：核心向量化函数 (源代码 -> 向量数组)
/**
 * 函数名：generate_vector
 * 作用：读取一段源代码，统计特征，填入 vector 数组。
 * 参数 vector: 必须是一个长度为 VECTOR_DIMENSION 的整数数组
 */
void generate_vector(const char *code, int vector[]) {

    // [步骤1：清零] 非常重要！防止数组里有垃圾值
    for (int i = 0; i < VECTOR_DIMENSION; i++) {
        vector[i] = 0;
    }

    int pos = 0;
    Token token;

    // [步骤2：循环分词]
    do {
        // 调用你在 tokenization.c 里写的函数
        get_next_token(code, &pos, &token);

        if (token.type != TOKEN_END) {
            // 查表：看这个词对应几号格子
            int idx = get_feature_index(&token);

            // 计数：如果是有效特征
            if (idx != -1) {
                vector[idx]++;
            }
        }
    } while (token.type != TOKEN_END);
}

// ==========================================================
// 第四部分：测试主函数
// ==========================================================

int main() {
    // 测试代码：包含关键字、变量、数字、符号
    const char *test_code = "int score = 60; if(score > 0) return score;";

    // 准备一个数组存结果
    int my_vector[VECTOR_DIMENSION];

    printf("正在分析代码: %s\n", test_code);
    printf("--------------------------------------------------\n");

    // 生成向量
    generate_vector(test_code, my_vector);

    // 打印结果
    printf("生成的特征向量 (只显示非 0 的项):\n");
    printf("[ ");
    for (int i = 0; i < VECTOR_DIMENSION; i++) {
        if (my_vector[i] > 0) {
            printf("%s:%d | ", FEATURE_MAP[i], my_vector[i]);
        }
    }
    printf("]\n");

    return 0;
}
