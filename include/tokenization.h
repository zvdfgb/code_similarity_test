//
// Created by mmm on 2025/12/5.
//
// tokenization.h
#ifndef TOKENIZATION_H
#define TOKENIZATION_H

// 1. 定义 Token 类型
typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_END,
    TOKEN_STRING
} TokenType;

// 2. 定义 Token 结构体
typedef struct {
    TokenType type;
    char value[100];
} Token;

// 3. 声明函数 (告诉编译器这些函数在另一个文件里)
int is_keyword(const char *str);
void get_next_token(const char *source, int *pos, Token *token);

#endif