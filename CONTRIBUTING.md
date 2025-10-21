# Contributing to SYSY Compiler (贡献指南)

感谢您考虑为 SYSY 编译器项目做出贡献！

Thank you for considering contributing to the SYSY Compiler project!

## 如何贡献 (How to Contribute)

### 报告 Bug (Reporting Bugs)

如果您发现了 bug，请创建一个 issue 并包含：

If you find a bug, please create an issue with:

1. **问题描述** (Description)
   - 简洁明了的问题描述
   - Clear description of the problem

2. **重现步骤** (Steps to Reproduce)
   ```bash
   ./bin/sysyc test.sy
   # 预期结果 vs 实际结果
   ```

3. **环境信息** (Environment)
   - OS: Ubuntu 22.04
   - g++ version: 11.3.0
   - 编译器版本 (Compiler version)

4. **相关代码** (Relevant Code)
   ```c
   // 导致问题的 SYSY 代码
   ```

### 提出新功能 (Suggesting Features)

在开始实现新功能前：

Before starting a new feature:

1. 检查是否已有相关 issue
2. 创建 feature request issue
3. 描述功能的用例和好处
4. 讨论实现方案
5. 获得维护者的反馈

### 提交代码 (Submitting Code)

#### 1. Fork 仓库 (Fork the Repository)

```bash
# Fork the repo on GitHub, then:
git clone https://github.com/YOUR_USERNAME/AICompliler.git
cd AICompliler
```

#### 2. 创建分支 (Create a Branch)

```bash
git checkout -b feature/your-feature-name
# 或
git checkout -b fix/bug-description
```

#### 3. 进行更改 (Make Changes)

- 遵循代码风格指南
- 添加测试
- 更新文档

#### 4. 测试 (Test)

```bash
make clean
make
make test
```

#### 5. 提交更改 (Commit Changes)

```bash
git add .
git commit -m "feat: add support for new feature"
# 或
git commit -m "fix: resolve issue with parser"
```

**提交信息格式** (Commit Message Format):
```
<type>: <subject>

<body>

<footer>
```

**类型** (Types):
- `feat`: 新功能
- `fix`: Bug 修复
- `docs`: 文档更新
- `style`: 代码格式化
- `refactor`: 重构
- `test`: 添加测试
- `chore`: 构建/工具更改

#### 6. 推送并创建 PR (Push and Create PR)

```bash
git push origin feature/your-feature-name
```

然后在 GitHub 上创建 Pull Request。

## 代码风格 (Code Style)

### C++ 代码规范 (C++ Code Standards)

1. **命名约定** (Naming Conventions)
   ```cpp
   // 类名：大驼峰
   class TokenType { };
   
   // 函数名：小驼峰
   void parseExpression() { }
   
   // 变量名：蛇形命名
   int token_count;
   std::string file_name;
   
   // 常量：全大写
   const int MAX_BUFFER_SIZE = 1024;
   ```

2. **缩进** (Indentation)
   - 使用 4 个空格缩进
   - 不使用 Tab

3. **大括号** (Braces)
   ```cpp
   // 推荐
   if (condition) {
       statement;
   }
   
   // 不推荐
   if (condition)
   {
       statement;
   }
   ```

4. **注释** (Comments)
   ```cpp
   // 单行注释使用 //
   
   /**
    * 多行注释使用这种格式
    * 用于函数和类的文档
    */
   ```

5. **头文件保护** (Header Guards)
   ```cpp
   #ifndef FILENAME_H
   #define FILENAME_H
   
   // 内容
   
   #endif // FILENAME_H
   ```

### 格式化工具 (Formatting Tools)

使用 clang-format 自动格式化：

```bash
clang-format -i src/**/*.cpp include/**/*.h
```

## 项目结构 (Project Structure)

```
AICompliler/
├── src/              # 源代码
│   ├── lexer/       # 词法分析器
│   ├── parser/      # 语法分析器
│   ├── ir/          # 中间表示
│   ├── optimizer/   # 优化器
│   └── codegen/     # 代码生成
├── include/         # 头文件
├── tests/           # 测试
├── examples/        # 示例程序
├── docs/            # 文档
└── Makefile         # 构建文件
```

## 开发工作流 (Development Workflow)

### 1. 本地开发 (Local Development)

```bash
# 克隆仓库
git clone https://github.com/yuanxiaoaihezhou/AICompliler.git
cd AICompliler

# 构建
make

# 运行测试
make test

# 清理
make clean
```

### 2. 添加新功能 (Adding a New Feature)

假设要添加一个新的优化 pass：

Example: Adding a new optimization pass:

1. **创建头文件** (Create Header)
   ```bash
   touch include/new_optimizer.h
   ```

2. **创建实现文件** (Create Implementation)
   ```bash
   touch src/optimizer/new_optimizer.cpp
   ```

3. **添加测试** (Add Tests)
   ```bash
   touch tests/test_new_optimizer.cpp
   ```

4. **更新 Makefile** (Update Makefile)
   - Makefile 会自动包含新文件

5. **实现功能** (Implement)
   ```cpp
   // include/new_optimizer.h
   #ifndef NEW_OPTIMIZER_H
   #define NEW_OPTIMIZER_H
   
   class NewOptimizer {
   public:
       void optimize();
   };
   
   #endif
   ```

6. **编写测试** (Write Tests)
   ```cpp
   // tests/test_new_optimizer.cpp
   void test_new_optimization() {
       // 测试代码
       assert(result == expected);
   }
   ```

7. **运行测试** (Run Tests)
   ```bash
   make test
   ```

### 3. 修复 Bug (Fixing a Bug)

1. **重现 Bug** (Reproduce the Bug)
   - 创建最小测试用例
   - 添加到测试套件

2. **调试** (Debug)
   ```bash
   gdb ./bin/sysyc
   (gdb) run test.sy
   ```

3. **修复** (Fix)
   - 修改相关代码
   - 确保修复不破坏现有功能

4. **验证** (Verify)
   ```bash
   make test
   ```

5. **提交** (Commit)
   ```bash
   git commit -m "fix: resolve issue #123"
   ```

## 测试要求 (Testing Requirements)

### 必须包含测试 (Tests Required)

所有新功能和 bug 修复都必须包含测试：

All new features and bug fixes must include tests:

1. **单元测试** (Unit Tests)
   - 测试单个函数/类
   - 快速执行

2. **集成测试** (Integration Tests)
   - 测试多个组件交互
   - 端到端测试

3. **回归测试** (Regression Tests)
   - 确保不破坏现有功能

### 测试覆盖率 (Test Coverage)

目标测试覆盖率：80%+

Target test coverage: 80%+

```bash
# 检查覆盖率
make coverage
```

## 文档要求 (Documentation Requirements)

### 代码文档 (Code Documentation)

为公共 API 添加注释：

Add comments for public APIs:

```cpp
/**
 * Parse an expression from token stream
 * @param tokens The token stream to parse
 * @return AST node representing the expression
 * @throws ParseError if syntax is invalid
 */
std::unique_ptr<Expression> parseExpression(const std::vector<Token>& tokens);
```

### 更新文档 (Update Documentation)

如果更改影响用户，更新相关文档：

If changes affect users, update relevant docs:

- `README.md` - 项目概述
- `docs/usage.md` - 使用指南
- `docs/design.md` - 设计文档

## Pull Request 检查清单 (PR Checklist)

在提交 PR 前，确保：

Before submitting a PR, ensure:

- [ ] 代码遵循项目风格指南
- [ ] 所有测试通过
- [ ] 添加了新测试（如果适用）
- [ ] 更新了文档（如果适用）
- [ ] 提交信息清晰明了
- [ ] PR 描述详细说明了更改内容
- [ ] 没有不相关的更改
- [ ] 代码已经过自我审查

## 审查流程 (Review Process)

### 代码审查 (Code Review)

1. 维护者会审查您的 PR
2. 可能会提出修改建议
3. 讨论并进行必要的更改
4. 审查通过后合并

### 审查标准 (Review Criteria)

- **正确性** (Correctness): 代码是否正确实现了功能
- **测试** (Testing): 是否有足够的测试
- **性能** (Performance): 是否有性能问题
- **可维护性** (Maintainability): 代码是否易于理解和维护
- **风格** (Style): 是否遵循项目风格

## 获取帮助 (Getting Help)

如果有问题：

If you have questions:

1. 查看现有文档和 issues
2. 在 GitHub Discussions 中提问
3. 创建 issue

## 行为准则 (Code of Conduct)

### 我们的承诺 (Our Pledge)

我们致力于为每个人提供友好、安全和欢迎的环境。

We are committed to providing a friendly, safe and welcoming environment for all.

### 标准 (Standards)

**积极行为** (Positive Behavior):
- 使用友好和包容的语言
- 尊重不同的观点
- 优雅地接受建设性批评
- 专注于对社区最有利的事情

**不可接受的行为** (Unacceptable Behavior):
- 使用性暗示或贬损性语言
- 人身攻击或政治攻击
- 骚扰行为
- 发布他人的私人信息

## 许可证 (License)

通过贡献，您同意您的贡献将在 MIT 许可证下授权。

By contributing, you agree that your contributions will be licensed under the MIT License.

---

感谢您的贡献！🎉

Thank you for your contributions! 🎉
