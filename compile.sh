#!/bin/bash

# 定义编译器
CC="gcc"

# 定义编译选项
# -Wall -Wextra: 开启所有常用警告，帮助发现潜在问题
# -Iinclude: 告诉编译器在 'include' 目录中查找头文件
# -std=c11: 使用 C11 标准进行编译
CFLAGS="-Wall -Wextra -Iinclude -std=c11"

# 定义链接选项
# -lm: 链接数学库，因为您的 calculate.c 中使用了 sqrt 函数
LDFLAGS="-lm"

# 定义源文件列表
# 注意: 这里列出了您项目中的所有 .c 源文件
SRCS="src/main.c src/preprocess.c src/tokenization.c src/vectorization.c src/calculate.c"

# 定义可执行文件的名称
EXECUTABLE="code_similarity_checker"

# --- 编译主程序 ---
echo "正在编译项目..."

# 执行编译命令
# $CC: 编译器 (gcc)
# $CFLAGS: 编译选项
# $SRCS: 所有源文件
# -o $EXECUTABLE: 指定输出的可执行文件名称
# $LDFLAGS: 链接选项
$CC $CFLAGS $SRCS -o $EXECUTABLE $LDFLAGS

# 检查编译是否成功
if [ $? -eq 0 ]; then
    echo "编译成功！已生成可执行文件: ./$EXECUTABLE"
    echo "您现在可以使用 './$EXECUTABLE test/test1.c test/test2.c' 来运行程序。"
else
    echo "编译失败，请检查错误信息。"
    exit 1
fi

# --- 清理功能 (可选) ---
# 该功能用于删除编译过程中生成的所有 .o 文件和最终的可执行文件
# 您可以通过运行 'bash compile.sh clean' 来使用它
if [ "$1" == "clean" ]; then
    echo "正在清理生成的文件..."
    rm -f $EXECUTABLE
    echo "清理完成。"
fi
