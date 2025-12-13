#include "preprocess.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * @brief 辅助函数：将整个文件内容读取到动态分配的缓冲区中。
 * @param filepath 文件的路径。
 * @return 包含文件内容的缓冲区指针，如果失败则返回NULL。调用者需要负责释放此内存。
 */
static char* read_file_content(const char* filepath) {
    // 以二进制模式打开文件，以便准确获取文件大小
    FILE* file = fopen(filepath, "rb");
    if (file == NULL) {
        perror("错误：无法打开文件");
        return NULL;
    }

    // 将文件指针移动到文件末尾以获取文件大小
    fseek(file, 0, SEEK_END);
    long length = ftell(file); // 获取文件当前位置，即文件大小
    fseek(file, 0, SEEK_SET); // 将文件指针重置到文件开头

    if (length == -1) {
        perror("错误：无法获取文件大小");
        fclose(file);
        return NULL;
    }

    // 为文件内容分配内存，加1是为了存放字符串结束符'\0'
    char* buffer = (char*)malloc(length + 1);
    if (buffer == NULL) {
        perror("错误：为文件内容分配内存失败");
        fclose(file);
        return NULL;
    }

    // 读取文件内容到缓冲区
    size_t bytes_read = fread(buffer, 1, length, file);
    // 检查是否所有字节都已读取
    if (bytes_read != (size_t)length) {
        perror("错误：读取文件失败");
        free(buffer);
        fclose(file);
        return NULL;
    }
    buffer[length] = '\0'; // 确保字符串以空字符结尾

    fclose(file);
    return buffer;
}

/**
 * @brief 预处理源代码文件。
 * 功能：读取文件，移除注释、预处理指令、多余空白、字符串字面量和字符字面量，并转为小写。
 * @param filepath 源代码文件的路径。
 * @return 处理后的纯净代码字符串（动态分配），如果失败则返回NULL。调用者必须使用free()释放此内存。
 */
char* preprocess_file(const char* filepath) {
    char* source_code = read_file_content(filepath);
    if (source_code == NULL) {
        return NULL; // 文件读取失败
    }

    size_t source_len = strlen(source_code);
    // 预处理后的代码缓冲区，最大可能与原代码等长
    char* preprocessed_code = (char*)malloc(source_len + 1);
    if (preprocessed_code == NULL) {
        perror("错误：为预处理代码分配内存失败");
        free(source_code);
        return NULL;
    }

    size_t i = 0; // source_code 的当前读取位置（源字符串迭代器）
    size_t j = 0; // preprocessed_code 的当前写入位置（目标字符串迭代器）
    
    // 状态标志：用于跟踪当前解析器所处的上下文
    int in_multiline_comment = 0;   // 是否在多行注释 /* ... */ 中
    int in_single_line_comment = 0; // 是否在单行注释 // ... 中
    int in_string_literal = 0;      // 是否在字符串字面量 " ... " 中
    int in_char_literal = 0;        // 是否在字符字面量 ' ... ' 中
    int in_preprocessor_directive = 0; // 是否在预处理指令 # ... 中

    // 辅助标志：用于确保连续的空白字符被压缩成一个空格
    int prev_was_space = 1; // 初始设为1，以便处理文件开头的空白

    // 遍历源代码中的每个字符
    while (i < source_len) {
        char current_char = source_code[i];
        // 获取下一个字符，用于检查多字符序列（如 /*, //, "）
        char next_char = (i + 1 < source_len) ? source_code[i+1] : '\0';

        // --- 状态处理：如果当前处于某种特殊状态，则直接处理该状态 ---
        if (in_multiline_comment) { // 处理多行注释 /* ... */
            if (current_char == '*' && next_char == '/') {
                in_multiline_comment = 0; // 退出多行注释状态
                i += 2; // 跳过 '*/'
                continue; // 继续下一个字符
            }
            i++; // 跳过注释内容
            continue;
        } else if (in_single_line_comment) { // 处理单行注释 // ...
            if (current_char == '\n') {
                in_single_line_comment = 0; // 退出单行注释状态（遇到换行符）
                // 不直接continue，而是让它作为空白字符被后续逻辑处理，有助于压缩空白
            }
            i++; // 跳过注释内容
            continue;
        } else if (in_string_literal) { // 处理字符串字面量 " ... "
            // 移除字符串字面量内容，但保留其作为一个“空白”的占位符
            if (current_char == '\\' && next_char != '\0') { // 处理转义字符，如 \"
                i += 2; // 跳过转义序列
                continue;
            } else if (current_char == '"') {
                in_string_literal = 0; // 退出字符串字面量状态
            }
            i++; // 跳过字符串内容
            continue;
        } else if (in_char_literal) { // 处理字符字面量 ' ... '
            // 移除字符字面量内容
             if (current_char == '\\' && next_char != '\0') { // 处理转义字符，如 '\n'
                i += 2; // 跳过转义序列
                continue;
            } else if (current_char == '\'') {
                in_char_literal = 0; // 退出字符字面量状态
            }
            i++; // 跳过字符内容
            continue;
        } else if (in_preprocessor_directive) { // 处理预处理指令 # ...
            if (current_char == '\n') {
                in_preprocessor_directive = 0; // 退出预处理指令状态（遇到换行符）
                // 不直接continue，而是让它作为空白字符被后续逻辑处理
            }
            i++; // 跳过指令内容
            continue;
        }
        
        // --- 检查是否进入新的特殊状态 ---
        if (current_char == '/' && next_char == '*') { // 进入多行注释
            in_multiline_comment = 1;
            i += 2; // 跳过 "/*"
            continue;
        } else if (current_char == '/' && next_char == '/') { // 进入单行注释
            in_single_line_comment = 1;
            i += 2; // 跳过 "//"
            continue;
        } else if (current_char == '"') { // 进入字符串字面量
            in_string_literal = 1;
            // 根据规划，我们直接移除字符串内容。这里通过跳过其内部直到匹配到结束引号
            i++;
            while(i < source_len && (source_code[i] != '"' || (i > 0 && source_code[i-1] == '\\'))) {
                 i++;
            }
            if (i < source_len && source_code[i] == '"') { // 跳过结束引号
                i++;
            }
            prev_was_space = 1; // 移除字符串后，视同插入了一个空格
            continue;
        } else if (current_char == '\'') { // 进入字符字面量
            in_char_literal = 1;
            // 移除字符字面量内容
            i++;
            while(i < source_len && (source_code[i] != '\'' || (i > 0 && source_code[i-1] == '\\'))) {
                 i++;
            }
            if (i < source_len && source_code[i] == '\'') { // 跳过结束引号
                i++;
            }
            prev_was_space = 1; // 移除字符后，视同插入了一个空格
            continue;
        } else if (current_char == '#' && prev_was_space) { // 进入预处理指令 (行首或空白后)
            in_preprocessor_directive = 1;
            i++; // 跳过 '#'
            continue;
        }
        
        // --- 处理常规字符（不在任何特殊状态中） ---
        if (isspace(current_char)) { // 如果是空白字符
            if (!prev_was_space) { // 如果前一个字符不是空白，则插入一个空格
                preprocessed_code[j++] = ' ';
                prev_was_space = 1;
            }
        } else { // 如果是非空白字符
            preprocessed_code[j++] = tolower(current_char); // 转换为小写并添加到结果
            prev_was_space = 0; // 标记当前字符不是空白
        }
        i++; // 移动到下一个源字符
    }

    // 确保结果字符串以空字符结尾
    preprocessed_code[j] = '\0';
    // 根据实际使用的长度重新分配内存，释放多余空间
    char* final_code = (char*)realloc(preprocessed_code, j + 1);
    // realloc 失败时可能返回 NULL，且原内存块未被释放
    if (final_code == NULL && j > 0) { 
        perror("错误：为最终预处理代码重新分配内存失败");
        free(preprocessed_code); // 释放原始内存块
        free(source_code);
        return NULL;
    } else if (final_code == NULL && j == 0) {
        // 如果 final_code 是 NULL 且 j=0，这意味着 realloc 请求分配0字节，
        // 它可能返回 NULL，但我们仍希望得到一个空字符串。
        final_code = (char*)malloc(1);
        if (final_code) final_code[0] = '\0';
        else {
             perror("错误：为清空预处理代码分配内存失败");
             free(source_code);
             return NULL;
        }
    }

    free(source_code); // 释放原始源代码缓冲区
    return final_code; // 返回预处理后的代码
}
