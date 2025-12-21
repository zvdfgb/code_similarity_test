#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>                   //字符分类/转换
#include "preprocess.h"

char* preprocess_file(const char* filepath)   //返回处理后的字符串
{
    FILE* file = fopen(filepath, "r");        //以只读的方式打开文件
    if (!file) {                              //打开失败处理
        printf ( "错误：无法打开文件 %s\n", filepath);
        return NULL;                          //返回空指针表示失败
    }
                                              
    // 文件读取和验证
    fseek(file, 0, SEEK_END);                //指针file移动到文件末尾
    long file_size = ftell(file);            //获取当前位置（即文件大小）
    fseek(file, 0, SEEK_SET);                //重置到文件开头
                                             //获取文件大小以分配内存
    if (file_size <= 0) {                    //文件为空或获取大小失败
        fclose(file);
        fprintf(stderr, "错误：文件为空或读取失败\n");  //fprintf输出到任意文件流，stderr为标准错误流
        return NULL;
    }

    // 分配缓冲区和读取
    char* source = (char*)malloc(file_size + 1);       //malloc()用于动态内存分配，file_size+1为需要分配的字节数
    if (!source) {                                     //malloc()原返回void*型指针，即无类型指针，可被转换为任何指针类型
        fclose(file);
        fprintf(stderr, "错误：内存分配失败\n"); 
        return NULL;                                   //如果内存分配失败，返回空指针
    }

    size_t bytes_read = fread(source, 1, file_size, file);  //从文件中读取file_size个字节，
                                                            //每个字节1个字节大小，存到source指向的内存中
               //size_t fread(void *ptr, size_t size, size_t count, FILE *stream);
    fclose(file);                                           //关闭文件，不再需要
    
    if (bytes_read != (size_t)file_size) {        //若读取字节数与预期不符
        free(source);
        fprintf(stderr, "错误：文件读取不完整\n");
        return NULL;
    }
    source[bytes_read] = '\0';            //添加字符串终止符  

    // 分配结果缓冲区（处理后内容通常更短）
    char* result = (char*)malloc(bytes_read + 1);
    if (!result) {
        free(source);
        fprintf(stderr, "错误：内存分配失败\n");
        return NULL;
    }

    // 状态标志
    int in_comment = 0;           // 是否在多行注释中(/*……*/)
    int in_single_comment = 0;    // 是否在单行注释中(//……)
    int in_string = 0;            // 是否在字符串中
    int in_include = 0;           // 是否在预处理指令中(#开头的行)
    int last_char_was_space = 0;  // 前一个字符是空格(用于压缩空格)
    int escape_next = 0;          // 转义下一个字符（用于字符串中的\）  

    size_t result_index = 0;      //结果缓冲区索引    
    size_t i = 0;                 //源字符串索引      

    while (source[i] != '\0') 
    {
        char current = source[i];        //定义字符current               
        char next = source[i + 1];       //定义字符next

        // 处理转义字符（主要在字符串中）
        if (escape_next)    //如果为真；            
        {
            escape_next = 0;
            i++;
            continue;
        }

        // 1. 多行注释处理
        if (!in_string && !in_single_comment && !in_include) //不在其他特殊状态中
        {
            if (!in_comment && current == '/' && next == '*') {
                in_comment = 1;
                i += 2;
                continue;
            }
            if (in_comment && current == '*' && next == '/') {
                in_comment = 0;
                i += 2;
                continue;
            }
            if (in_comment) {
                i++;
                continue;
            }
        }

        // 2. 单行注释处理
        if (!in_string && !in_comment && !in_include) 
        {
            if (!in_single_comment && current == '/' && next == '/') {
                in_single_comment = 1;
                i += 2;
                continue;
            }
            if (in_single_comment && current == '\n') {
                in_single_comment = 0;
                // 继续处理换行符（将其转换为空格）
            }
            if (in_single_comment) {
                i++;
                continue;
            }
        }

        // 3. 预处理指令处理
        if (!in_string && !in_comment && !in_single_comment)
        {
            if (!in_include && current == '#') {
                in_include = 1;
                i++;
                continue;
            }
            if (in_include) {
                if (current == '\n') {
                    in_include = 0;
                } else {
                    i++;
                    continue;
                }
            }
        }

        // 4. 字符串字面量处理
        if (!in_comment && !in_single_comment && !in_include) 
        {
            if (!in_string && current == '"') {
                in_string = 1;
                i++;
                continue;
            }
            if (in_string) {
                if (current == '\\') {
                    escape_next = 1;
                } else if (current == '"') {
                    in_string = 0;
                }
                i++;
                continue;
            }
        }

        // 5. 空白字符处理
        if (isspace(current)) 
        {
            if (!last_char_was_space && result_index > 0) {
                result[result_index++] = ' ';
                last_char_was_space = 1;
            }
            i++;
            continue;
        }

        // 6. 正常字符处理：转换为小写并添加到结果
        if (!in_comment && !in_single_comment && !in_include && !in_string) 
        {
            result[result_index++] = tolower(current);
            last_char_was_space = 0;
        }

        i++;
    }

    // 确保结果字符串正确终止
    result[result_index] = '\0';

    // 释放源文件缓冲区
    free(source);

    // 如果结果为空，返回空字符串
    if (result_index == 0) {
        result[0] = '\0';
    }

    return result;
}
