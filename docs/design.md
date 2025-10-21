# SYSY Compiler Design Document

## 概述 (Overview)

本文档详细描述了 SYSY 编译器的设计和实现。SYSY 是一个类 C 语言的子集，本编译器将 SYSY 源代码编译为 x86_64 汇编代码。

This document describes the design and implementation of the SYSY compiler. SYSY is a C-like language subset, and this compiler translates SYSY source code to x86_64 assembly.

## 编译器架构 (Compiler Architecture)

编译器采用传统的多阶段设计：

```
Source Code (.sy)
    ↓
Lexical Analysis (词法分析)
    ↓
Tokens
    ↓
Syntax Analysis (语法分析)
    ↓
Abstract Syntax Tree (AST)
    ↓
Intermediate Code Generation (中间代码生成)
    ↓
Intermediate Representation (IR)
    ↓
Optimization (优化)
    ↓
Optimized IR
    ↓
Code Generation (目标代码生成)
    ↓
Assembly Code (.s)
```

## 1. 词法分析 (Lexical Analysis)

### 功能 (Functionality)
- 将源代码分解为 token 序列
- 识别关键字、标识符、字面量、操作符和分隔符
- 跳过空白字符和注释

### 实现 (Implementation)
- **文件**: `src/lexer/lexer.cpp`, `include/lexer.h`
- **主要类**: `Lexer`
- **Token 类型**: 
  - 关键字: `const`, `int`, `void`, `if`, `else`, `while`, `break`, `continue`, `return`
  - 标识符和字面量: `IDENT`, `INT_LITERAL`
  - 运算符: `+`, `-`, `*`, `/`, `%`, `<`, `<=`, `>`, `>=`, `==`, `!=`, `&&`, `||`, `!`
  - 分隔符: `(`, `)`, `{`, `}`, `[`, `]`, `;`, `,`

### 算法 (Algorithm)
使用状态机进行 token 识别：
1. 跳过空白字符
2. 处理注释（单行 `//` 和多行 `/* */`）
3. 识别数字字面量
4. 识别标识符和关键字
5. 识别运算符和分隔符

## 2. 语法分析 (Syntax Analysis)

### 功能 (Functionality)
- 将 token 序列解析为抽象语法树 (AST)
- 检查语法错误
- 构建程序的层次结构

### 实现 (Implementation)
- **文件**: `src/parser/parser.cpp`, `include/parser.h`
- **主要类**: `Parser`
- **AST 节点类型**:
  - 表达式: `IntLiteralExpr`, `IdentExpr`, `BinaryExpr`, `UnaryExpr`, `CallExpr`
  - 语句: `ExprStmt`, `Block`, `IfStmt`, `WhileStmt`, `ReturnStmt`, `BreakStmt`, `ContinueStmt`
  - 声明: `VarDecl`, `FunctionDef`

### 语法规则 (Grammar)
使用递归下降解析器实现以下文法：

```
Program         → Declaration*
Declaration     → FuncDef | VarDecl | ConstDecl
FuncDef         → Type IDENT '(' Params? ')' Block
VarDecl         → 'int' IDENT ('[' INT ']')? ('=' Expr)? ';'
ConstDecl       → 'const' 'int' IDENT '=' Expr ';'
Block           → '{' Statement* '}'
Statement       → VarDecl | ConstDecl | IfStmt | WhileStmt 
                | ReturnStmt | BreakStmt | ContinueStmt 
                | Block | ExprStmt
IfStmt          → 'if' '(' Expr ')' Statement ('else' Statement)?
WhileStmt       → 'while' '(' Expr ')' Statement
ReturnStmt      → 'return' Expr? ';'
ExprStmt        → Expr ';'
Expr            → LogicalOr
LogicalOr       → LogicalAnd ('||' LogicalAnd)*
LogicalAnd      → Equality ('&&' Equality)*
Equality        → Relational (('==' | '!=') Relational)*
Relational      → Additive (('<' | '<=' | '>' | '>=') Additive)*
Additive        → Multiplicative (('+' | '-') Multiplicative)*
Multiplicative  → Unary (('*' | '/' | '%') Unary)*
Unary           → ('+' | '-' | '!')? Primary
Primary         → INT_LITERAL | IDENT | FuncCall | '(' Expr ')'
```

## 3. 中间代码生成 (Intermediate Code Generation)

### 功能 (Functionality)
- 将 AST 转换为三地址码形式的中间表示
- 简化后续优化和目标代码生成

### 实现 (Implementation)
- **文件**: `src/ir/ir_generator.cpp`, `include/ir_generator.h`
- **主要类**: `IRGenerator`, `IRModule`, `IRFunction`, `IRInstruction`
- **IR 指令类型**:
  - 算术: `ADD`, `SUB`, `MUL`, `DIV`, `MOD`
  - 比较: `EQ`, `NE`, `LT`, `LE`, `GT`, `GE`
  - 逻辑: `AND`, `OR`, `NOT`
  - 内存: `LOAD`, `STORE`, `ALLOC`
  - 控制流: `LABEL`, `JUMP`, `BRANCH`
  - 函数: `CALL`, `RETURN`, `PARAM`
  - 其他: `MOVE`, `CONST`

### IR 格式 (IR Format)
```
result = arg1 op arg2    # 二元运算
result = op arg1          # 一元运算
result = LOAD addr        # 加载
STORE addr, value         # 存储
LABEL:                    # 标签
JUMP label                # 无条件跳转
BRANCH cond, true_label, false_label  # 条件跳转
result = CALL func        # 函数调用
RETURN value              # 返回
```

## 4. 优化 (Optimization)

### 计划实现的优化 (Planned Optimizations)
1. **常量折叠** (Constant Folding)
   - 在编译时计算常量表达式
   - 例: `2 + 3` → `5`

2. **常量传播** (Constant Propagation)
   - 替换已知常量值的变量
   - 例: `x = 5; y = x + 1;` → `y = 6;`

3. **死代码消除** (Dead Code Elimination)
   - 删除永远不会执行的代码
   - 删除未使用的变量和赋值

4. **公共子表达式消除** (Common Subexpression Elimination)
   - 避免重复计算相同的表达式

### 实现 (Implementation)
- **文件**: `src/optimizer/optimizer.cpp`, `include/optimizer.h`
- 遍历 IR 并应用各种优化技术

## 5. 目标代码生成 (Code Generation)

### 功能 (Functionality)
- 将优化后的 IR 翻译为 x86_64 汇编代码
- 管理寄存器分配和栈帧
- 生成函数调用序列

### 实现 (Implementation)
- **文件**: `src/codegen/codegen.cpp`, `include/codegen.h`
- **主要类**: `CodeGenerator`
- **目标平台**: x86_64 (AMD64)

### 寄存器使用 (Register Usage)
- `%rax`: 累加器，函数返回值
- `%rbx`: 通用寄存器
- `%rsp`: 栈指针
- `%rbp`: 帧指针
- `%rdi, %rsi, %rdx, %rcx, %r8, %r9`: 函数参数（System V ABI）

### 栈帧布局 (Stack Frame Layout)
```
高地址 (High Address)
    +----------------+
    | 参数 n         |
    | ...            |
    | 参数 1         |
    +----------------+
    | 返回地址       |
    +----------------+ ← 调用前的 %rsp
    | 保存的 %rbp    |
    +----------------+ ← 当前 %rbp
    | 局部变量 1     |
    | 局部变量 2     |
    | ...            |
    | 临时变量       |
    +----------------+ ← 当前 %rsp
低地址 (Low Address)
```

### 指令映射 (Instruction Mapping)
| IR 指令 | x86_64 汇编 |
|---------|-------------|
| ADD     | addq        |
| SUB     | subq        |
| MUL     | imulq       |
| DIV     | idivq       |
| LOAD    | movq        |
| STORE   | movq        |
| JUMP    | jmp         |
| BRANCH  | cmpq + jne  |

## 6. 编译器自举 (Self-Hosting)

### 计划 (Plan)
1. 使用 C++ 实现编译器初始版本
2. 将编译器源代码翻译为 SYSY 语言
3. 使用初始版本编译 SYSY 版本的编译器
4. 验证生成的编译器可以编译自身

### 挑战 (Challenges)
- SYSY 语言特性限制
- 需要实现足够的标准库支持
- 内存管理和复杂数据结构

## 测试策略 (Testing Strategy)

### 单元测试 (Unit Tests)
- 词法分析器测试: `tests/test_lexer.cpp`
- 语法分析器测试: `tests/test_parser.cpp`
- IR 生成器测试: `tests/test_ir.cpp`
- 代码生成器测试: `tests/test_codegen.cpp`

### 集成测试 (Integration Tests)
- 端到端编译测试
- 示例程序: `examples/*.sy`
- 验证生成的汇编代码可以正确执行

### 性能测试 (Performance Tests)
- 编译时间测试
- 生成代码的执行效率

## 构建系统 (Build System)

使用 Makefile 组织项目构建：

```bash
make            # 构建编译器
make test       # 运行测试
make examples   # 编译示例程序
make clean      # 清理构建产物
make install    # 安装编译器
```

## 项目结构 (Project Structure)

```
AICompliler/
├── src/
│   ├── lexer/          # 词法分析器
│   ├── parser/         # 语法分析器
│   ├── ir/             # 中间代码生成
│   ├── optimizer/      # 优化器
│   ├── codegen/        # 目标代码生成
│   └── main.cpp        # 主程序
├── include/            # 头文件
├── tests/              # 测试用例
├── examples/           # 示例程序
├── docs/               # 文档
├── Makefile            # 构建文件
└── README.md           # 项目说明
```

## 参考资料 (References)

1. "Compilers: Principles, Techniques, and Tools" (Dragon Book)
2. "Engineering a Compiler" by Cooper and Torczon
3. x86_64 Assembly Language Reference
4. System V AMD64 ABI
5. SYSY Language Specification
