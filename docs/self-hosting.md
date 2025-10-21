# Compiler Self-Hosting Plan (编译器自举计划)

## 概述 (Overview)

编译器自举 (Self-hosting) 是指编译器能够编译自身的源代码。这是编译器成熟度的重要标志。

Compiler self-hosting means the compiler can compile its own source code. This is an important milestone for compiler maturity.

## 实现路线 (Implementation Roadmap)

### 阶段 1: 语言特性扩展 (Phase 1: Language Feature Extensions)

当前 SYSY 语言缺少一些编写编译器所需的特性：

Current SYSY language lacks some features needed for writing a compiler:

1. **字符串支持** (String Support)
   - 字符串字面量
   - 字符串操作函数
   - 文件 I/O

2. **结构体和类** (Structures and Classes)
   - 需要支持复杂数据结构（AST 节点、符号表等）
   - 成员函数和方法

3. **动态内存管理** (Dynamic Memory Management)
   - malloc/free 或类似机制
   - 垃圾回收（可选）

4. **标准库** (Standard Library)
   - 输入输出
   - 字符串处理
   - 容器（vector, map 等）

### 阶段 2: 编译器源码改写 (Phase 2: Compiler Source Rewrite)

将当前的 C++ 编译器逐步改写为 SYSY 语言：

Gradually rewrite the current C++ compiler to SYSY language:

#### 2.1 核心数据结构 (Core Data Structures)

```c
// Token 结构
struct Token {
    int type;
    int line;
    int column;
    int value;
};

// AST 节点基类
struct ASTNode {
    int node_type;
    // ... 其他字段
};

// 符号表
struct SymbolTable {
    // ... 实现
};
```

#### 2.2 词法分析器 (Lexer)

将 `src/lexer/lexer.cpp` 改写为 SYSY：

Rewrite `src/lexer/lexer.cpp` in SYSY:

```c
int getNextToken() {
    // 实现词法分析逻辑
}
```

#### 2.3 语法分析器 (Parser)

将 `src/parser/parser.cpp` 改写为 SYSY：

Rewrite `src/parser/parser.cpp` in SYSY:

```c
struct ASTNode* parseExpression() {
    // 实现语法分析逻辑
}
```

#### 2.4 IR 生成器 (IR Generator)

将 `src/ir/ir_generator.cpp` 改写为 SYSY。

Rewrite `src/ir/ir_generator.cpp` in SYSY.

#### 2.5 代码生成器 (Code Generator)

将 `src/codegen/codegen.cpp` 改写为 SYSY。

Rewrite `src/codegen/codegen.cpp` in SYSY.

### 阶段 3: 引导过程 (Phase 3: Bootstrapping Process)

#### 3.1 第一次编译 (First Compilation)

使用 C++ 版本的编译器编译 SYSY 版本的编译器：

Use the C++ compiler to compile the SYSY version:

```bash
./bin/sysyc compiler_sysy/main.sy -o compiler_stage1.s
as compiler_stage1.s -o compiler_stage1.o
ld compiler_stage1.o -o sysyc_stage1
```

#### 3.2 第二次编译 (Second Compilation)

使用第一阶段生成的编译器重新编译自己：

Use the stage 1 compiler to recompile itself:

```bash
./sysyc_stage1 compiler_sysy/main.sy -o compiler_stage2.s
as compiler_stage2.s -o compiler_stage2.o
ld compiler_stage2.o -o sysyc_stage2
```

#### 3.3 验证 (Verification)

验证两个编译器生成相同的代码：

Verify that both compilers generate the same code:

```bash
# 使用两个编译器编译同一个测试程序
./sysyc_stage1 test.sy -o test1.s
./sysyc_stage2 test.sy -o test2.s

# 比较生成的汇编代码
diff test1.s test2.s
```

如果输出相同，则说明编译器已成功自举。

If the output is identical, the compiler has successfully bootstrapped.

## 技术挑战 (Technical Challenges)

### 1. 内存管理 (Memory Management)

SYSY 需要支持动态内存分配：

SYSY needs to support dynamic memory allocation:

- 实现简单的堆管理器
- 或依赖操作系统的 malloc/free

### 2. 字符串处理 (String Handling)

编译器大量使用字符串操作：

Compilers heavily use string operations:

- 字符串拼接
- 字符串比较
- 子字符串提取

### 3. 文件 I/O (File I/O)

编译器需要读写文件：

Compilers need to read and write files:

- 读取源文件
- 写入汇编文件
- 错误日志

### 4. 复杂数据结构 (Complex Data Structures)

需要实现：

Need to implement:

- 动态数组 (vector)
- 哈希表 (map)
- 链表
- 树结构 (AST)

## 性能考虑 (Performance Considerations)

### 编译时间 (Compilation Time)

自举后的编译器可能比 C++ 版本慢：

The bootstrapped compiler may be slower than the C++ version:

- 优化编译器生成的代码
- 实现更高效的算法
- 使用更好的数据结构

### 生成代码质量 (Generated Code Quality)

确保生成的代码质量不降低：

Ensure generated code quality doesn't degrade:

- 实现更多优化 pass
- 更好的寄存器分配
- 指令选择优化

## 测试策略 (Testing Strategy)

### 1. 单元测试 (Unit Tests)

每个组件都需要独立测试：

Each component needs independent testing:

```bash
# 测试词法分析器
./test_lexer_sysy

# 测试语法分析器
./test_parser_sysy
```

### 2. 集成测试 (Integration Tests)

端到端测试：

End-to-end testing:

```bash
# 编译并运行测试程序
./sysyc_sysy examples/fibonacci.sy -o fib.s
as fib.s -o fib.o
ld fib.o -o fib
./fib
```

### 3. 回归测试 (Regression Tests)

确保新版本不破坏现有功能：

Ensure new versions don't break existing features:

```bash
# 运行所有测试用例
for test in tests/*.sy; do
    ./sysyc_sysy $test
done
```

## 时间估算 (Time Estimation)

| 阶段 | 预计时间 | 描述 |
|------|---------|------|
| 阶段 1 | 2-3 周 | 语言特性扩展 |
| 阶段 2.1-2.2 | 1-2 周 | 词法和语法分析器 |
| 阶段 2.3-2.4 | 2-3 周 | IR 和优化 |
| 阶段 2.5 | 1-2 周 | 代码生成 |
| 阶段 3 | 1 周 | 引导和验证 |
| **总计** | **7-11 周** | |

## 参考资料 (References)

1. **"Building a Self-Hosting Compiler"** - 自举编译器构建指南
2. **"Reflections on Trusting Trust"** by Ken Thompson - 关于编译器信任的经典论文
3. **Tiny C Compiler (TCC)** - 一个可以自举的小型 C 编译器
4. **Oberon** - 自举的系统和编译器

## 当前状态 (Current Status)

- ✅ C++ 版本编译器完成
- ✅ 基本语言特性实现
- ⏳ 语言扩展开发中
- ⏳ SYSY 版本编译器开发中
- ⏳ 自举测试待进行

## 下一步 (Next Steps)

1. 实现字符串支持
2. 添加结构体/类支持
3. 实现动态内存管理
4. 开始改写核心模块
5. 进行渐进式测试

---

更新日期 (Last Updated): 2025-10-21
