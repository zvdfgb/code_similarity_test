//
// Created by mmm on 2025/12/5.
//
#include<stdio.h>
#include<ctype.h> //包含判断字符类型的函数

//1:定义token的类型标签
//枚举每个类型的名字
typedef enum{
    TOKEN_KEYWORD,//关键字
    TOKEN_INENTIFIER,//标识符
    TOKEN_NUMBER,//数字
    TOKEN_OPERATOR,//运算符或符号
    TOKEN_END,//特殊类型
}TokenType;

//2:定义token结构体
//包含他的类型和他的具体内容
typedef struct {
    TokenType type;//类型
    char value[100];//值
}Token;

//辅助函数：判断一个单词是不是C语言的关键字
int is_keyword(const char *str)
{
    //列举关键字
    const char *keyword[] = {"int","float","if","else","return","while","for"}//后续需要补充！！！！
    int num_keyword = 7;
    for(int i=0;i<num_keyword;i++)
    {
        if(strcmp(str,keyword[i])==0)
        {
            return 1;
        }
    }
    return 0;
}


/**
* 3:核心分词函数
*source = 源代码字符串
*pos = 当前读到的位置的指针
*token = 用来存放解析结果的结构体
*/
void get_next_token(const char *source,int *pos,Token *token)
{
    //获取当前位置的字符
    char current_char = source[*pos];


    //步骤1：跳过没用的空白字符
    //如果当前是空格，换行，制表符就跳过往后走
    while(isspace(current_char))
    {
        (*pos) = (*pos)+1;
        current_char = source[*pos];//更新当前字符
    }


    //步骤2：检查是否读完了
    //如果读到‘\0'，说明代码结束了
    if(current_char== '\0')
    {
        (*token).type = TOKEN_END;//设置类型为结束
        return;
    }
    // 步骤3: 处理 单词
    // 规则：如果第一个字符是字母或下划线
    if (isalpha(current_char) || current_char == '_')
    {
        int i = 0;
        // 只要后面接着的字符是 字母、数字 或 下划线，就一直读
        while (isalnum(current_char) || current_char == '_') {
            (*token).value[i] = current_char; // 把字符存入结构体
            i++;
            // 移动源代码的光标
            (*pos) = (*pos) + 1;
            current_char = source[*pos];
        }
        (*token).value[i] = '\0'; // 重要！在字符串末尾补上结束符

        // 读完了一个单词，判断它是系统关键字，还是用户自定义的变量名？
        if (is_keyword((*token).value)) {
            (*token).type = TOKEN_KEYWORD;
        } else {
            (*token).type = TOKEN_IDENTIFIER;
        }
        return; // 这个 Token 处理完了，返回
    }

    //步骤4: 处理 数字
    // 规则：如果第一个字符是数字
    if (isdigit(current_char))
    {
        int i = 0;
        // 只要后面是数字，就一直读
        while (isdigit(current_char)) {
            (*token).value[i] = current_char;
            i++;

            (*pos) = (*pos) + 1;
            current_char = source[*pos];
        }
        (*token).value[i] = '\0'; // 补结束符
        (*token).type = TOKEN_NUMBER; // 设置类型为数字
        return;
    }

    //步骤 5: 处理 符号 (运算符/标点)
    // 如果不是字母也不是数字，那就当做符号处理 (比如 = ; + )
    // 这里简单处理，每次只取 1 个字符
    (*token).value[0] = current_char;
    (*token).value[1] = '\0';
    (*token).type = TOKEN_OPERATOR;
    // 别忘了把光标往后移一位，否则下次还会读到这个符号，造成死循环
    (*pos) = (*pos) + 1;
}