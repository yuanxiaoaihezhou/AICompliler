# Self-Hosting Language Features (自举语言特性)

## Overview (概述)

This document describes the enhanced language features added to support compiler self-hosting.

本文档描述为支持编译器自举而添加的增强语言特性。

## New Features (新特性)

### 1. Character Type (字符类型)

The compiler now supports the `char` type for single-byte character values.

编译器现在支持 `char` 类型用于单字节字符值。

#### Syntax (语法)

```c
char c;
char c = 'A';
const char newline = '\n';
```

#### Character Literals (字符字面量)

Character literals are enclosed in single quotes and support escape sequences:

字符字面量用单引号括起来，支持转义序列：

- `'A'` - Regular character (普通字符)
- `'\n'` - Newline (换行)
- `'\t'` - Tab (制表符)
- `'\r'` - Carriage return (回车)
- `'\0'` - Null character (空字符)
- `'\\'` - Backslash (反斜杠)
- `'\''` - Single quote (单引号)

#### Example (示例)

```c
int main() {
    char letter;
    letter = 'H';
    char newline = '\n';
    return letter;
}
```

### 2. String Literals (字符串字面量)

String literals are enclosed in double quotes and support escape sequences.

字符串字面量用双引号括起来，支持转义序列。

#### Syntax (语法)

```c
char* msg = "Hello, World!";
char* path = "C:\\Program Files\\";
```

#### Escape Sequences (转义序列)

Same escape sequences as character literals:

与字符字面量相同的转义序列：

- `\"` - Double quote (双引号)
- `\\` - Backslash (反斜杠)
- `\n` - Newline (换行)
- `\t` - Tab (制表符)
- `\r` - Carriage return (回车)
- `\0` - Null character (空字符)

### 3. Pointer Types (指针类型)

The compiler now supports pointer types for all base types.

编译器现在支持所有基本类型的指针类型。

#### Syntax (语法)

```c
int* ptr;           // Pointer to int
char* str;          // Pointer to char (string)
int** pptr;         // Pointer to pointer to int
void* generic_ptr;  // Generic pointer
```

#### Pointer Operators (指针运算符)

##### Address-of Operator (&)

Get the address of a variable.

获取变量的地址。

```c
int x = 42;
int* ptr = &x;
```

##### Dereference Operator (*)

Access the value pointed to by a pointer.

访问指针指向的值。

```c
int x = 42;
int* ptr = &x;
int value = *ptr;  // value = 42
```

#### Function Parameters and Return Types (函数参数和返回类型)

Pointers can be used in function parameters and return types.

指针可以用于函数参数和返回类型。

```c
int* find_max(int* arr, int size) {
    // Implementation
    return arr;
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
```

### 4. Increment and Decrement Operators (自增和自减运算符)

Support for prefix increment and decrement operators.

支持前缀自增和自减运算符。

#### Syntax (语法)

```c
int x = 5;
++x;  // x = 6
--x;  // x = 5
```

#### Usage (用法)

These operators are particularly useful in loops and pointer arithmetic.

这些运算符在循环和指针算术中特别有用。

```c
int i = 0;
while (i < 10) {
    ++i;
}
```

### 5. Multiple Type Support (多类型支持)

Variables and functions can now use `int`, `char`, or `void` types.

变量和函数现在可以使用 `int`、`char` 或 `void` 类型。

#### Examples (示例)

```c
// Function with char return type
char get_char() {
    return 'A';
}

// Function with char pointer parameter
void print_string(char* str) {
    // Implementation
}

// Mixed types
int process(char c, int* count) {
    (*count)++;
    return c;
}
```

## Use Cases for Self-Hosting (自举用例)

### 1. String Processing (字符串处理)

Essential for processing source code and generating output.

对于处理源代码和生成输出至关重要。

```c
// Compare two strings
int strcmp(char* s1, char* s2) {
    while (*s1 && *s1 == *s2) {
        ++s1;
        ++s2;
    }
    return *s1 - *s2;
}
```

### 2. Token Storage (标记存储)

Store tokens with their string values.

存储带有字符串值的标记。

```c
// Token structure representation
int token_type;
char* token_text;
int token_value;
```

### 3. Symbol Table (符号表)

Maintain symbol information using pointers.

使用指针维护符号信息。

```c
char* symbol_name;
int symbol_type;
int* symbol_address;
```

### 4. AST Construction (AST 构建)

Build abstract syntax trees using pointers.

使用指针构建抽象语法树。

```c
// Simplified AST node
int node_type;
int* left_child;
int* right_child;
```

## Limitations (限制)

### Current Limitations (当前限制)

1. **No Dynamic Memory**: `malloc`/`free` not yet supported
   未支持动态内存分配

2. **No Structures**: Struct types not yet implemented
   尚未实现结构体类型

3. **Limited Pointer Arithmetic**: Basic operations only
   仅限基本指针运算

4. **No Function Pointers**: Cannot store function addresses
   无法存储函数地址

5. **No Arrays of Pointers**: Limited array support
   数组支持有限

### Future Enhancements (未来增强)

These features will be added in future versions to complete self-hosting support:

这些特性将在未来版本中添加以完成自举支持：

1. Struct and union types (结构体和联合类型)
2. Dynamic memory allocation (动态内存分配)
3. Function pointers (函数指针)
4. Enhanced pointer arithmetic (增强指针算术)
5. Standard library functions (标准库函数)

## Testing (测试)

### Test Programs (测试程序)

Several test programs are included to verify the new features:

包含多个测试程序以验证新特性：

1. **char_test.sy** - Character literal test
   字符字面量测试

2. **pointer_test.sy** - Pointer operations test
   指针操作测试

3. **string_test.sy** - String and char pointer test
   字符串和字符指针测试

4. **io_functions.sy** - I/O function declarations
   I/O 函数声明

### Running Tests (运行测试)

```bash
# Compile test programs
./bin/sysyc examples/char_test.sy -o char_test.s
./bin/sysyc examples/pointer_test.sy -o pointer_test.s
./bin/sysyc examples/string_test.sy -o string_test.s

# View tokens and IR
./bin/sysyc examples/char_test.sy -tokens -ir
```

## Compiler Implementation Notes (编译器实现说明)

### Lexer Changes (词法分析器变化)

- Added `CHAR`, `TYPEDEF` keywords
- Added `CHAR_LITERAL`, `STRING_LITERAL` token types
- Added `AMPERSAND`, `ARROW`, `INCREMENT`, `DECREMENT`, `DOT` operators
- Implemented escape sequence handling

### Parser Changes (语法分析器变化)

- Added `CharLiteralExpr`, `StringLiteralExpr` AST nodes
- Extended type parsing to support pointers
- Updated function parameter parsing
- Enhanced variable declaration parsing
- Added unary operators for `&`, `*`, `++`, `--`

### IR Generator Changes (IR 生成器变化)

- Added visitors for new expression types
- Basic support for character and string constants

### Code Generator Changes (代码生成器变化)

- Generates code for all supported constructs
- Pointer operations map to memory operations

## Conclusion (结论)

These enhancements provide the foundation for compiler self-hosting by adding essential features like character types, string literals, and pointer support. While not yet complete, the current implementation supports a significant subset of operations needed for a self-hosting compiler.

这些增强通过添加字符类型、字符串字面量和指针支持等基本特性，为编译器自举奠定了基础。虽然尚未完成，但当前实现支持自举编译器所需的大部分操作子集。

---

**Last Updated**: 2025-10-21

**Status**: ✅ Core features implemented and tested
