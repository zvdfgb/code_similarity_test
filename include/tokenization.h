//
// Created by mmm on 2025/12/5.
//
// tokenization.h
#ifndef TOKENIZATION_H
#define TOKENIZATION_H

//1:定义token的类型标签
//枚举每个类型的名字
typedef enum{
    TOKEN_KEYWORD,//关键字
    TOKEN_IDENTIFIER,//标识符
    TOKEN_NUMBER,//数字
    TOKEN_OPERATOR,//运算符或符号
    TOKEN_END,//特殊类型
    TOKEN_STRING,//字符串类型
}TokenType;

//2:定义token结构体
//包含他的类型和他的具体内容
typedef struct {
    TokenType type;//类型
    char value[100];//值
}Token;

// 3. 声明函数 (告诉编译器这些函数在另一个文件里)
int is_keyword(const char *str);
void get_next_token(const char *source, int *pos, Token *token);

#endif