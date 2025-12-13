#ifndef PREPROCESS_H
#define PREPROCESS_H

// 函数：预处理源代码文件
// 功能：读取文件，移除注释、#指令、多余空白和字符串字面量，并转为小写
// 输入：文件路径 (const char*)
// 输出：处理后的纯净代码字符串 (char*)，需在外部释放内存
char* preprocess_file(const char* filepath);

#endif