# SYSY Compiler Usage Guide

## 使用指南 (Usage Guide)

本文档介绍如何使用 SYSY 编译器。

This document describes how to use the SYSY compiler.

## 安装 (Installation)

### 前置条件 (Prerequisites)

- Ubuntu 22.04 (x86_64)
- g++ (支持 C++17)
- GNU Make
- GNU Assembler (as)
- GNU Linker (ld)

### 编译编译器 (Building the Compiler)

```bash
cd AICompliler
make
```

编译成功后，可执行文件位于 `bin/sysyc`。

The executable will be located at `bin/sysyc` after successful compilation.

### 安装到系统 (System Installation)

```bash
sudo make install
```

这将把编译器安装到 `/usr/local/bin/sysyc`。

This installs the compiler to `/usr/local/bin/sysyc`.

## 基本使用 (Basic Usage)

### 编译单个文件 (Compiling a Single File)

```bash
./bin/sysyc examples/hello.sy
```

这将生成 `a.s` 汇编文件。

This generates an assembly file `a.s`.

### 指定输出文件 (Specifying Output File)

```bash
./bin/sysyc examples/hello.sy -o hello.s
```

### 查看中间表示 (Viewing Intermediate Representation)

```bash
./bin/sysyc examples/hello.sy -ir
```

### 查看 Token 序列 (Viewing Token Sequence)

```bash
./bin/sysyc examples/hello.sy -tokens
```

## 完整编译流程 (Complete Compilation Process)

### 从源代码到可执行文件 (From Source to Executable)

```bash
# 1. 编译为汇编代码
./bin/sysyc program.sy -o program.s

# 2. 汇编为目标文件
as program.s -o program.o

# 3. 链接生成可执行文件
ld program.o -o program

# 4. 运行程序
./program
```

### 使用 GCC 链接 (Linking with GCC)

如果需要使用 C 标准库，可以使用 GCC 进行链接：

```bash
./bin/sysyc program.sy -o program.s
gcc program.s -o program -no-pie
./program
```

## 命令行选项 (Command-Line Options)

```
sysyc <input.sy> [options]

Options:
  -o <file>    指定输出汇编文件 (默认: a.s)
               Specify output assembly file (default: a.s)
               
  -ir          输出中间表示
               Output intermediate representation
               
  -tokens      输出词法分析的 token 序列
               Output tokens from lexical analysis
               
  -h, --help   显示帮助信息
               Display help information
```

## SYSY 语言特性 (SYSY Language Features)

### 支持的数据类型 (Supported Data Types)

- `int`: 32位整数
- `void`: 用于函数返回类型
- 数组: `int a[10];`

### 支持的运算符 (Supported Operators)

#### 算术运算符 (Arithmetic Operators)
- `+` 加法
- `-` 减法
- `*` 乘法
- `/` 除法
- `%` 取模

#### 比较运算符 (Comparison Operators)
- `<` 小于
- `<=` 小于等于
- `>` 大于
- `>=` 大于等于
- `==` 等于
- `!=` 不等于

#### 逻辑运算符 (Logical Operators)
- `&&` 逻辑与
- `||` 逻辑或
- `!` 逻辑非

### 支持的语句 (Supported Statements)

- 变量声明: `int x;` 或 `int x = 10;`
- 常量声明: `const int MAX = 100;`
- 赋值: `x = 5;`
- 条件语句: `if (condition) { ... } else { ... }`
- 循环语句: `while (condition) { ... }`
- 返回语句: `return value;`
- 跳转语句: `break;`, `continue;`

### 函数 (Functions)

```c
// 无参数函数
int getValue() {
    return 42;
}

// 有参数函数
int add(int a, int b) {
    return a + b;
}

// void 函数
void printValue(int x) {
    // 实现
    return;
}
```

## 示例程序 (Example Programs)

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
    int result;
    result = fib(10);
    return result;
}
```

### 循环求和 (Loop Sum)

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

## 错误处理 (Error Handling)

编译器会报告以下类型的错误：

The compiler reports the following types of errors:

### 词法错误 (Lexical Errors)
- 非法字符
- 未终止的注释

### 语法错误 (Syntax Errors)
- 缺少分号
- 括号不匹配
- 非法的语句结构

### 语义错误 (Semantic Errors)
- 未声明的变量
- 类型不匹配
- 重复定义

## 调试技巧 (Debugging Tips)

### 查看中间表示 (View IR)

```bash
./bin/sysyc program.sy -ir > program.ir
```

### 查看汇编代码 (View Assembly)

```bash
./bin/sysyc program.sy -o program.s
cat program.s
```

### 使用 GDB 调试生成的代码 (Debug with GDB)

```bash
gcc -g program.s -o program -no-pie
gdb program
```

## 性能优化建议 (Performance Tips)

1. 使用局部变量而不是全局变量
2. 避免不必要的函数调用
3. 使用简单的循环结构
4. 尽量使用常量

## 限制和已知问题 (Limitations and Known Issues)

1. 目前不支持浮点数
2. 不支持字符串
3. 数组功能有限
4. 没有预处理器
5. 标准库支持有限

## 获取帮助 (Getting Help)

如遇到问题，请查看：
- 设计文档: `docs/design.md`
- 示例程序: `examples/`
- 测试用例: `tests/`

If you encounter issues, please refer to:
- Design document: `docs/design.md`
- Example programs: `examples/`
- Test cases: `tests/`
