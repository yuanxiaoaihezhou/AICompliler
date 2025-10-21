# Testing Guide (测试指南)

## 概述 (Overview)

本文档描述了 SYSY 编译器的测试策略和方法。

This document describes the testing strategy and methods for the SYSY compiler.

## 测试架构 (Testing Architecture)

```
测试层级 (Test Hierarchy):
├── 单元测试 (Unit Tests)
│   ├── 词法分析器测试
│   ├── 语法分析器测试
│   ├── IR 生成器测试
│   ├── 优化器测试
│   └── 代码生成器测试
├── 集成测试 (Integration Tests)
│   ├── 端到端编译测试
│   └── 示例程序测试
└── 性能测试 (Performance Tests)
    ├── 编译时间测试
    └── 生成代码效率测试
```

## 运行测试 (Running Tests)

### 运行所有测试 (Run All Tests)

```bash
make test
```

### 运行特定测试 (Run Specific Test)

```bash
# 词法分析器测试
./bin/test_test_lexer

# 语法分析器测试
./bin/test_test_parser

# 优化器测试
./bin/test_test_optimizer
```

## 单元测试 (Unit Tests)

### 1. 词法分析器测试 (Lexer Tests)

**文件**: `tests/test_lexer.cpp`

**测试内容**:
- 关键字识别
- 运算符识别
- 标识符和数字字面量
- 注释处理

**示例**:
```cpp
void test_keywords() {
    std::string source = "const int void if else while";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    // 验证 token 类型
}
```

### 2. 语法分析器测试 (Parser Tests)

**文件**: `tests/test_parser.cpp`

**测试内容**:
- 简单函数解析
- 带参数的函数
- if/else 语句
- while 循环
- 表达式解析

**示例**:
```cpp
void test_simple_function() {
    std::string source = "int main() { return 0; }";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();
    // 验证 AST 结构
}
```

### 3. 优化器测试 (Optimizer Tests)

**文件**: `tests/test_optimizer.cpp`

**测试内容**:
- 常量折叠
- 常量传播
- 死代码消除

**示例**:
```cpp
void test_constant_folding() {
    IRFunction func("test", "int");
    func.addInstruction(IRInstruction(IROpcode::ADD, "t0", "2", "3"));
    Optimizer optimizer;
    IRFunction optimized = optimizer.optimizeFunction(func);
    // 验证优化结果
}
```

## 集成测试 (Integration Tests)

### 示例程序测试 (Example Program Tests)

#### 1. Hello World

```bash
./bin/sysyc examples/hello.sy -o hello.s
as hello.s -o hello.o
ld hello.o -o hello
./hello
echo $?  # 应该输出 0
```

#### 2. 算术运算

```bash
./bin/sysyc examples/arithmetic.sy -o arithmetic.s
as arithmetic.s -o arithmetic.o
ld arithmetic.o -o arithmetic
./arithmetic
echo $?  # 应该输出 20 (10 + 5 * 2)
```

#### 3. 斐波那契数列

```bash
./bin/sysyc examples/fibonacci.sy -o fib.s
as fib.s -o fib.o
ld fib.o -o fib
./fib
echo $?  # 应该输出 55 (fib(10))
```

#### 4. 阶乘

```bash
./bin/sysyc examples/factorial.sy -o factorial.s
as factorial.s -o factorial.o
ld factorial.o -o factorial
./factorial
echo $?  # 应该输出 120 (5!)
```

#### 5. 最大公约数

```bash
./bin/sysyc examples/gcd.sy -o gcd.s
as gcd.s -o gcd.o
ld gcd.o -o gcd
./gcd
echo $?  # 应该输出 6 (gcd(48, 18))
```

## 测试用例编写指南 (Test Case Writing Guidelines)

### 1. 测试用例结构 (Test Case Structure)

```cpp
void test_feature_name() {
    // 1. 准备 (Arrange)
    std::string input = "...";
    
    // 2. 执行 (Act)
    auto result = function_under_test(input);
    
    // 3. 验证 (Assert)
    assert(result == expected);
    
    std::cout << "test_feature_name passed\n";
}
```

### 2. 测试覆盖原则 (Test Coverage Principles)

- **边界条件测试** (Boundary Testing)
  - 空输入
  - 最大/最小值
  - 特殊字符

- **正常路径测试** (Happy Path Testing)
  - 典型用例
  - 常见场景

- **异常路径测试** (Error Path Testing)
  - 语法错误
  - 语义错误
  - 运行时错误

## 添加新测试 (Adding New Tests)

### 1. 创建测试文件 (Create Test File)

```bash
# 在 tests/ 目录下创建新的测试文件
touch tests/test_new_feature.cpp
```

### 2. 编写测试代码 (Write Test Code)

```cpp
#include <iostream>
#include <cassert>
#include "your_header.h"

void test_case_1() {
    // 测试逻辑
    assert(condition);
    std::cout << "test_case_1 passed\n";
}

int main() {
    std::cout << "Running New Feature Tests...\n";
    test_case_1();
    std::cout << "All tests passed!\n";
    return 0;
}
```

### 3. 更新 Makefile (Update Makefile)

Makefile 会自动检测 `tests/` 目录下的所有 `.cpp` 文件，无需手动修改。

The Makefile automatically detects all `.cpp` files in the `tests/` directory.

## 持续集成 (Continuous Integration)

### GitHub Actions 配置示例

```yaml
name: CI

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-22.04
    steps:
      - uses: actions/checkout@v2
      - name: Install dependencies
        run: sudo apt-get install -y g++ make binutils
      - name: Build
        run: make
      - name: Test
        run: make test
```

## 回归测试 (Regression Testing)

### 创建回归测试套件 (Create Regression Test Suite)

```bash
#!/bin/bash
# regression_test.sh

echo "Running regression tests..."

for test in examples/*.sy; do
    echo "Testing $test..."
    ./bin/sysyc "$test" -o "${test%.sy}.s"
    if [ $? -ne 0 ]; then
        echo "FAILED: $test"
        exit 1
    fi
done

echo "All regression tests passed!"
```

### 运行回归测试 (Run Regression Tests)

```bash
chmod +x regression_test.sh
./regression_test.sh
```

## 性能测试 (Performance Testing)

### 1. 编译时间测试 (Compilation Time Test)

```bash
time ./bin/sysyc large_program.sy -o output.s
```

### 2. 生成代码效率测试 (Generated Code Efficiency Test)

```bash
# 比较优化前后的性能
./bin/sysyc program.sy -O0 -o program_no_opt.s
./bin/sysyc program.sy -o program_opt.s

# 汇编、链接并运行
as program_no_opt.s -o program_no_opt.o
ld program_no_opt.o -o program_no_opt

as program_opt.s -o program_opt.o
ld program_opt.o -o program_opt

# 测试执行时间
time ./program_no_opt
time ./program_opt
```

## 测试报告 (Test Reports)

### 生成测试报告 (Generate Test Report)

```bash
make test > test_report.txt 2>&1
```

### 测试覆盖率 (Test Coverage)

使用 gcov 生成代码覆盖率报告：

```bash
# 编译时启用覆盖率
CXXFLAGS="$CXXFLAGS --coverage" make

# 运行测试
make test

# 生成覆盖率报告
gcov src/**/*.cpp

# 或使用 lcov 生成 HTML 报告
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage_html
```

## 调试失败的测试 (Debugging Failed Tests)

### 1. 使用 GDB 调试 (Debug with GDB)

```bash
gdb ./bin/test_test_parser
(gdb) run
(gdb) backtrace
```

### 2. 添加调试输出 (Add Debug Output)

```cpp
void test_function() {
    std::cout << "Debug: input = " << input << "\n";
    auto result = parse(input);
    std::cout << "Debug: result = " << result << "\n";
    assert(result == expected);
}
```

### 3. 查看中间结果 (View Intermediate Results)

```bash
# 查看 token 序列
./bin/sysyc program.sy -tokens

# 查看 IR
./bin/sysyc program.sy -ir

# 查看生成的汇编
./bin/sysyc program.sy -o output.s
cat output.s
```

## 最佳实践 (Best Practices)

1. **测试先行** (Test First)
   - 在实现功能前编写测试
   - TDD (Test-Driven Development)

2. **保持测试独立** (Keep Tests Independent)
   - 每个测试不依赖其他测试
   - 可以以任意顺序运行

3. **使用有意义的测试名称** (Use Meaningful Test Names)
   - `test_parse_if_statement` ✅
   - `test1` ❌

4. **测试要快速** (Tests Should Be Fast)
   - 单元测试应该在毫秒级完成
   - 避免耗时的操作

5. **定期运行测试** (Run Tests Regularly)
   - 每次提交前运行测试
   - 使用 CI/CD 自动运行

## 测试清单 (Test Checklist)

开发新功能时的测试清单：

Checklist for testing new features:

- [ ] 编写单元测试
- [ ] 测试正常情况
- [ ] 测试边界情况
- [ ] 测试错误情况
- [ ] 运行所有测试确保没有回归
- [ ] 更新文档
- [ ] 代码审查

## 参考资料 (References)

1. **Test-Driven Development** by Kent Beck
2. **xUnit Test Patterns** by Gerard Meszaros
3. **Google Test Framework** - C++ 测试框架
4. **Catch2** - 现代 C++ 测试框架

---

更新日期 (Last Updated): 2025-10-21
