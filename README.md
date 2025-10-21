# SYSY Compiler (AICompliler)

一个用 C++ 实现的 SYSY 语言编译器，支持完整的编译流程，从词法分析到目标代码生成。

A SYSY language compiler implemented in C++, supporting the complete compilation pipeline from lexical analysis to code generation.

## 项目特点 (Features)

- ✅ **完整的编译流程**: 词法分析 → 语法分析 → 中间代码生成 → 优化 → 目标代码生成
- ✅ **分阶段实现**: 每个编译阶段都有独立的模块
- ✅ **详细的测试用例**: 包含单元测试和集成测试
- ✅ **完善的文档**: 提供设计文档和使用指南
- ✅ **x86_64 平台**: 编译平台和目标平台均为 x86_64 Ubuntu 22.04
- ✅ **Makefile 组织**: 使用标准 Makefile 管理项目构建
- 🚧 **编译器自举**: 计划实现编译器自举

## 项目结构 (Project Structure)

```
AICompliler/
├── src/
│   ├── lexer/          # 词法分析器 (Lexical Analyzer)
│   │   ├── token.cpp
│   │   └── lexer.cpp
│   ├── parser/         # 语法分析器 (Parser)
│   │   ├── ast.cpp
│   │   └── parser.cpp
│   ├── ir/             # 中间代码生成 (IR Generation)
│   │   ├── ir.cpp
│   │   └── ir_generator.cpp
│   ├── optimizer/      # 优化器 (Optimizer) - 待实现
│   ├── codegen/        # 目标代码生成 (Code Generator)
│   │   └── codegen.cpp
│   └── main.cpp        # 主程序
├── include/            # 头文件 (Header files)
│   ├── token.h
│   ├── lexer.h
│   ├── ast.h
│   ├── parser.h
│   ├── ir.h
│   ├── ir_generator.h
│   └── codegen.h
├── tests/              # 测试用例 (Test cases)
│   ├── test_lexer.cpp
│   └── test_parser.cpp
├── examples/           # 示例程序 (Example programs)
│   ├── hello.sy
│   ├── arithmetic.sy
│   ├── fibonacci.sy
│   └── loop.sy
├── docs/               # 文档 (Documentation)
│   ├── design.md       # 设计文档
│   └── usage.md        # 使用指南
├── Makefile            # 构建文件
└── README.md           # 本文件
```

## 快速开始 (Quick Start)

### 前置条件 (Prerequisites)

- Ubuntu 22.04 (x86_64)
- g++ (支持 C++17)
- GNU Make
- GNU Binutils (as, ld)

### 编译 (Build)

```bash
# 克隆仓库
git clone https://github.com/yuanxiaoaihezhou/AICompliler.git
cd AICompliler

# 编译编译器
make

# 运行测试
make test

# 编译示例程序
make examples
```

### 使用示例 (Usage Example)

```bash
# 编译 SYSY 源文件
./bin/sysyc examples/hello.sy -o hello.s

# 汇编和链接
as hello.s -o hello.o
ld hello.o -o hello

# 运行
./hello
echo $?  # 查看返回值
```

## 编译器阶段 (Compiler Phases)

### 1. 词法分析 (Lexical Analysis)

将源代码分解为 token 序列。

- **输入**: SYSY 源代码
- **输出**: Token 序列
- **实现**: `src/lexer/lexer.cpp`

支持的 token 类型：
- 关键字: `const`, `int`, `void`, `char`, `typedef`, `if`, `else`, `while`, `break`, `continue`, `return`
- 运算符: `+`, `-`, `*`, `/`, `%`, `<`, `<=`, `>`, `>=`, `==`, `!=`, `&&`, `||`, `!`, `&`, `++`, `--`, `->`
- 标识符和字面量: 整数、字符、字符串
- 分隔符: `()`, `{}`, `[]`, `;`, `,`, `.`

### 2. 语法分析 (Syntax Analysis)

将 token 序列解析为抽象语法树 (AST)。

- **输入**: Token 序列
- **输出**: 抽象语法树 (AST)
- **实现**: `src/parser/parser.cpp`
- **方法**: 递归下降解析

支持的语法结构：
- 函数定义和调用
- 变量声明和赋值
- 控制流语句 (if, while, return, break, continue)
- 表达式 (算术、比较、逻辑)
- 数组

### 3. 中间代码生成 (IR Generation)

将 AST 转换为三地址码形式的中间表示。

- **输入**: 抽象语法树
- **输出**: 中间表示 (IR)
- **实现**: `src/ir/ir_generator.cpp`
- **格式**: 三地址码

IR 指令类型：
- 算术: `ADD`, `SUB`, `MUL`, `DIV`, `MOD`
- 比较: `EQ`, `NE`, `LT`, `LE`, `GT`, `GE`
- 控制流: `LABEL`, `JUMP`, `BRANCH`
- 内存: `LOAD`, `STORE`, `ALLOC`
- 函数: `CALL`, `RETURN`, `PARAM`

### 4. 优化 (Optimization) - 待实现

计划实现的优化：
- 常量折叠 (Constant Folding)
- 常量传播 (Constant Propagation)
- 死代码消除 (Dead Code Elimination)
- 公共子表达式消除 (Common Subexpression Elimination)

### 5. 目标代码生成 (Code Generation)

将 IR 翻译为 x86_64 汇编代码。

- **输入**: 中间表示 (IR)
- **输出**: x86_64 汇编代码
- **实现**: `src/codegen/codegen.cpp`
- **平台**: x86_64 (System V ABI)

## 测试 (Testing)

### 单元测试 (Unit Tests)

```bash
# 运行所有测试
make test

# 运行特定测试
./bin/test_lexer
./bin/test_parser
```

### 示例程序 (Examples)

```bash
# 编译所有示例
make examples

# 手动编译单个示例
./bin/sysyc examples/fibonacci.sy -o fib.s
as fib.s -o fib.o
ld fib.o -o fib
./fib
```

## 文档 (Documentation)

- [设计文档 (Design Document)](docs/design.md) - 详细的编译器设计说明
- [使用指南 (Usage Guide)](docs/usage.md) - 编译器使用方法和示例
- [自举特性 (Self-Hosting Features)](docs/self-hosting-features.md) - 新增自举语言特性说明

## SYSY 语言示例 (SYSY Language Examples)

### Hello World

```c
int main() {
    return 0;
}
```

### 斐波那契数列 (Fibonacci)

```c
int fib(int n) {
    if (n <= 1) {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}

int main() {
    return fib(10);
}
```

### 循环 (Loop)

```c
int main() {
    int i;
    int sum;
    i = 0;
    sum = 0;
    while (i < 10) {
        sum = sum + i;
        i = i + 1;
    }
    return sum;
}
```

## 路线图 (Roadmap)

- [x] 词法分析器实现
- [x] 语法分析器实现
- [x] 中间代码生成
- [x] 基本代码生成
- [x] 测试框架
- [x] 文档完善
- [x] 优化器实现
- [x] 字符类型和字符串字面量支持
- [x] 指针类型支持
- [x] 自举语言特性增强
- [ ] 完整的标准库支持
- [ ] 动态内存分配
- [ ] 结构体和类型系统
- [ ] 编译器自举 (进行中)
- [ ] 性能优化
- [ ] 错误恢复和报告改进

## 贡献 (Contributing)

欢迎贡献代码、报告问题或提出改进建议！

Contributions are welcome! Feel free to submit issues or pull requests.

## 许可证 (License)

MIT License

## 参考资料 (References)

- "Compilers: Principles, Techniques, and Tools" (Dragon Book)
- "Engineering a Compiler" by Cooper and Torczon
- x86_64 Assembly Language Reference
- System V AMD64 ABI
- SYSY Language Specification

## 联系方式 (Contact)

如有问题或建议，请通过 GitHub Issues 联系。

For questions or suggestions, please contact via GitHub Issues.