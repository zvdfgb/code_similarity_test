# C Code Similarity Checker (代码相似度检测系统)

![Language](https://img.shields.io/badge/language-C11-blue.svg)
![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

## 📖 项目简介 (Introduction)

**C Code Similarity Checker** 是一个基于 C 语言开发的轻量级代码查重工具。它通过 **余弦相似度 (Cosine Similarity)** 算法，对两个 C 语言源文件的结构和逻辑进行比较，从而判断它们的代码相似程度。

该项目不仅仅是简单的文本比对，它内置了预处理、分词和向量化模块，能够有效识别“修改变量名”、“重排代码顺序”或“修改注释”等常见的规避手段。

## ✨ 核心功能 (Features)

*   **预处理 (Preprocessing)**：自动去除代码中的注释（`//` 和 `/*...*/`）以及多余的空白字符，消除格式差异带来的干扰。
*   **词法分析 (Tokenization)**：将源代码分解为最小的语义单元（Token），精确识别关键字、标识符、运算符和字面量。
*   **向量空间模型 (VSM)**：
    *   基于 35 维特征向量（涵盖 C 语言关键字、常见运算符等）。
    *   *智能过滤*：在向量化阶段特意忽略了用户自定义的变量名、数字和字符串，专注于比较代码的**逻辑骨架**。
*   **余弦相似度计算**：利用数学模型计算两个代码向量的夹角余弦值，输出 0.0 到 1.0 之间的相似度得分。

## 📂 项目结构 (Structure)

```text
.
├── src/                # 源代码目录
│   ├── main.c          # 程序入口，负责流程控制
│   ├── preprocess.c    # 预处理模块：去注释、清洗
│   ├── tokenization.c  # 分词模块：提取 Token
│   ├── vectorization.c # 向量化模块：特征统计
│   └── calculate.c     # 计算模块：余弦相似度算法
├── include/            # 头文件目录
├── test/               # 测试用例目录 (包含不同相似度的代码样本)
├── compile.sh          # Linux/Unix 编译脚本
└── README.md           # 项目说明文档
```

## 🚀 快速开始 (Getting Started)

### 环境要求 (Prerequisites)
*   **GCC 编译器** (支持 C11 标准)
*   Windows / Linux / macOS 均可运行

### 1. 编译项目 (Build)

在终端中运行以下命令即可生成可执行文件 `sim.exe` (Windows) 或 `sim` (Linux/Mac)。

**Windows (推荐):**
为了防止中文乱码，建议指定字符集编译：
```powershell
gcc -Wall -Wextra -Iinclude -std=c11 -finput-charset=UTF-8 -fexec-charset=GBK src/main.c src/preprocess.c src/tokenization.c src/vectorization.c src/calculate.c -o sim.exe -lm
```

**Linux / macOS:**
```bash
gcc -Wall -Wextra -Iinclude -std=c11 src/main.c src/preprocess.c src/tokenization.c src/vectorization.c src/calculate.c -o sim -lm
```

### 2. 运行程序 (Usage)

编译成功后，使用命令行传入两个需要比较的 C 文件路径。

**基本用法:**
```powershell
./sim.exe <文件A路径> <文件B路径>
```

**示例:**
```powershell
./sim.exe test/test1.c test/test2.c
```

### 3. 结果解读

程序将输出一个 0.00 到 1.00 的分数：
*   **0.90 - 1.00**: [极高] 极有可能存在直接抄袭。
*   **0.75 - 0.89**: [高] 结构和逻辑基本一致。
*   **0.50 - 0.74**: [中] 可能复用了部分逻辑。
*   **0.00 - 0.49**: [低] 代码差异较大。

## 🧪 测试集说明 (Test Cases)

项目 `test/` 目录下提供了用于验证算法的测试文件：

| 文件对 | 预期结果 | 说明 |
| :--- | :--- | :--- |
| `test1.c` vs `test2.c` | **高相似度** | 代码逻辑相同，仅注释不同（验证预处理能力） |
| `test3.c` vs `test4.c` | **高相似度** | 函数顺序不同，但逻辑一致（验证特征向量稳定性） |
| `test5.c` vs `test6.c` | **低相似度** | 完全不同的逻辑和算法（验证区分能力） |

## 🤝 贡献 (Contributing)

欢迎提交 Issue 或 Pull Request 来改进算法精度！

## 📄 许可证 (License)

本项目采用 MIT 许可证。

