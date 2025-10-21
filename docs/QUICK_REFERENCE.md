# Quick Reference - Enhanced Features (快速参考 - 增强特性)

## New Data Types (新数据类型)

### Character Type (字符类型)
```c
char c = 'A';
char newline = '\n';
const char tab = '\t';
```

### Pointer Types (指针类型)
```c
int* ptr;           // Pointer to int
char* str;          // Pointer to char (string)
int** pptr;         // Pointer to pointer
void* generic;      // Generic pointer
```

## Literals (字面量)

### Character Literals (字符字面量)
```c
'A'      // Letter A (65)
'0'      // Digit 0 (48)
'\n'     // Newline (10)
'\t'     // Tab (9)
'\r'     // Carriage return (13)
'\0'     // Null character (0)
'\\'     // Backslash (92)
'\''     // Single quote (39)
```

### String Literals (字符串字面量)
```c
"Hello"              // Simple string
"Hello, World!\n"    // String with newline
"Path: C:\\dir"      // String with backslash
"Say \"Hi\""         // String with quotes
```

## Operators (运算符)

### Pointer Operators (指针运算符)
```c
int x = 42;
int* ptr = &x;      // & = address-of
int value = *ptr;   // * = dereference
```

### Increment/Decrement (自增/自减)
```c
int i = 0;
++i;                // Prefix increment
--i;                // Prefix decrement
```

## Function Examples (函数示例)

### Simple Function (简单函数)
```c
char get_char() {
    return 'A';
}
```

### Pointer Parameters (指针参数)
```c
int add_indirect(int* a, int* b) {
    return *a + *b;
}
```

### Pointer Return Type (指针返回类型)
```c
char* get_message() {
    char* msg;
    return msg;
}
```

## Common Patterns (常用模式)

### Array Iteration with Pointers (指针遍历数组)
```c
void process_array(int* arr, int size) {
    int i = 0;
    while (i < size) {
        int value = *(arr + i);
        ++i;
    }
}
```

### Character Processing (字符处理)
```c
int is_digit(char c) {
    if (c >= '0') {
        if (c <= '9') {
            return 1;
        }
    }
    return 0;
}
```

### Swap Function (交换函数)
```c
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
```

### String Length (字符串长度)
```c
int strlen(char* s) {
    int len = 0;
    while (*s != '\0') {
        ++len;
        ++s;
    }
    return len;
}
```

## Compilation (编译)

### Basic Compilation (基本编译)
```bash
./bin/sysyc input.sy -o output.s
```

### View Tokens (查看标记)
```bash
./bin/sysyc input.sy -tokens
```

### View IR (查看中间表示)
```bash
./bin/sysyc input.sy -ir
```

### Disable Optimization (禁用优化)
```bash
./bin/sysyc input.sy -O0 -o output.s
```

## Example Programs (示例程序)

### Character Test (字符测试)
```c
// examples/char_test.sy
int main() {
    int a;
    a = 'A';
    return a;
}
```

### Pointer Test (指针测试)
```c
// examples/pointer_test.sy
int test_ptr(int* p) {
    return *p;
}

int main() {
    int x;
    int* ptr;
    x = 42;
    ptr = &x;
    return test_ptr(ptr);
}
```

### Mini Tokenizer (迷你词法分析器)
```c
// examples/mini_tokenizer.sy
int is_digit(char c) {
    if (c >= '0' && c <= '9') {
        return 1;
    }
    return 0;
}

int char_to_int(char c) {
    return c - '0';
}
```

## Type Compatibility (类型兼容性)

### Allowed (允许)
```c
int x = 'A';        // char literal to int (ASCII value)
char c = 65;        // int literal to char
int* p = &x;        // address of int
```

### Function Types (函数类型)
```c
// Return types
int func1();
char func2();
void func3();
int* func4();
char* func5();

// Parameters
void func(int x);
void func(char c);
void func(int* p);
void func(char* s);
```

## Common Mistakes (常见错误)

### Missing Pointer Star (缺少指针星号)
```c
// Wrong
int ptr;
ptr = &x;  // Error: can't assign pointer to int

// Correct
int* ptr;
ptr = &x;  // OK
```

### Character vs String (字符与字符串)
```c
// Wrong
char c = "A";  // Error: string to char

// Correct
char c = 'A';  // OK: character literal
```

### Dereference Non-Pointer (解引用非指针)
```c
// Wrong
int x = 42;
int y = *x;  // Error: x is not a pointer

// Correct
int* ptr = &x;
int y = *ptr;  // OK
```

## Tips (提示)

1. **Use pointers for passing large data** (使用指针传递大数据)
   ```c
   void process(int* arr, int size) { ... }
   ```

2. **Check for null before dereferencing** (解引用前检查空值)
   ```c
   if (ptr != 0) {
       value = *ptr;
   }
   ```

3. **Use character literals for ASCII** (使用字符字面量表示ASCII)
   ```c
   int newline = '\n';  // Better than: int newline = 10;
   ```

4. **Prefer char* for strings** (字符串使用char*)
   ```c
   char* msg = "Hello";  // Standard C idiom
   ```

## Next Steps (下一步)

To fully support self-hosting, the following features are planned:

为了完全支持自举，计划添加以下特性：

1. **Dynamic Memory** (动态内存)
   - `malloc(size)` - Allocate memory
   - `free(ptr)` - Free memory

2. **Struct Types** (结构体类型)
   - `struct Token { int type; char* text; }`
   - Member access with `.` and `->`

3. **File I/O** (文件输入输出)
   - `fopen(name, mode)` - Open file
   - `fread(buf, size, count, file)` - Read
   - `fwrite(buf, size, count, file)` - Write
   - `fclose(file)` - Close file

4. **Standard Library** (标准库)
   - String functions: `strcmp`, `strcpy`, `strlen`
   - Memory functions: `memcpy`, `memset`
   - Character functions: `isalpha`, `isdigit`

## Resources (资源)

- **Design Document**: `docs/design.md`
- **Usage Guide**: `docs/usage.md`
- **Self-Hosting Features**: `docs/self-hosting-features.md`
- **Enhancement Summary**: `docs/ENHANCEMENT_SUMMARY.md`

---

**Last Updated**: 2025-10-21  
**Compiler Version**: AICompliler with self-hosting enhancements
