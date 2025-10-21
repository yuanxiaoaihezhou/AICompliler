#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "lexer.h"
#include "parser.h"
#include "ir_generator.h"
#include "codegen.h"

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not write to file: " + filename);
    }
    file << content;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input.sy> [-o output.s] [-ir] [-tokens]\n";
        std::cerr << "Options:\n";
        std::cerr << "  -o <file>   Specify output assembly file (default: a.s)\n";
        std::cerr << "  -ir         Output intermediate representation\n";
        std::cerr << "  -tokens     Output tokens from lexical analysis\n";
        return 1;
    }
    
    std::string input_file = argv[1];
    std::string output_file = "a.s";
    bool show_ir = false;
    bool show_tokens = false;
    
    // Parse command line arguments
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-o" && i + 1 < argc) {
            output_file = argv[++i];
        } else if (arg == "-ir") {
            show_ir = true;
        } else if (arg == "-tokens") {
            show_tokens = true;
        }
    }
    
    try {
        // Read source file
        std::string source = readFile(input_file);
        
        // Lexical analysis
        std::cout << "=== Lexical Analysis ===\n";
        Lexer lexer(source);
        std::vector<Token> tokens = lexer.tokenize();
        
        if (show_tokens) {
            for (const auto& token : tokens) {
                std::cout << token.toString() << "\n";
            }
        }
        std::cout << "Tokens: " << tokens.size() << "\n\n";
        
        // Syntax analysis
        std::cout << "=== Syntax Analysis ===\n";
        Parser parser(tokens);
        auto ast = parser.parse();
        std::cout << "Parsing completed successfully\n\n";
        
        // Intermediate code generation
        std::cout << "=== Intermediate Code Generation ===\n";
        IRGenerator ir_gen;
        IRModule ir_module = ir_gen.generate(ast.get());
        
        if (show_ir) {
            std::cout << ir_module.toString();
        }
        std::cout << "IR generation completed\n\n";
        
        // Code generation
        std::cout << "=== Code Generation ===\n";
        CodeGenerator codegen;
        std::string assembly = codegen.generate(ir_module);
        
        // Write output
        writeFile(output_file, assembly);
        std::cout << "Assembly code written to " << output_file << "\n";
        
        std::cout << "\nCompilation successful!\n";
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
