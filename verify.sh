#!/bin/bash
# Verification script for SYSY Compiler

set -e

echo "=========================================="
echo "SYSY Compiler Verification Script"
echo "=========================================="
echo ""

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Step 1: Clean build
echo "Step 1: Cleaning previous build..."
make clean > /dev/null 2>&1
echo -e "${GREEN}✓${NC} Clean complete"
echo ""

# Step 2: Build compiler
echo "Step 2: Building compiler..."
if make > /dev/null 2>&1; then
    echo -e "${GREEN}✓${NC} Build successful"
else
    echo -e "${RED}✗${NC} Build failed"
    exit 1
fi
echo ""

# Step 3: Run tests
echo "Step 3: Running tests..."
if make test > test_output.txt 2>&1; then
    TEST_COUNT=$(grep -c "passed" test_output.txt || true)
    echo -e "${GREEN}✓${NC} All tests passed (${TEST_COUNT} test groups)"
    rm test_output.txt
else
    echo -e "${RED}✗${NC} Tests failed"
    cat test_output.txt
    rm test_output.txt
    exit 1
fi
echo ""

# Step 4: Compile examples
echo "Step 4: Compiling example programs..."
EXAMPLE_COUNT=0
EXAMPLE_SUCCESS=0

for example in examples/*.sy; do
    EXAMPLE_COUNT=$((EXAMPLE_COUNT + 1))
    filename=$(basename "$example" .sy)
    if ./bin/sysyc "$example" -o "examples/${filename}.s" > /dev/null 2>&1; then
        echo -e "${GREEN}✓${NC} ${filename}.sy compiled"
        EXAMPLE_SUCCESS=$((EXAMPLE_SUCCESS + 1))
    else
        echo -e "${RED}✗${NC} ${filename}.sy failed"
    fi
done
echo ""

# Step 5: Check documentation
echo "Step 5: Checking documentation..."
DOCS_EXPECTED=("README.md" "CONTRIBUTING.md" "PROJECT_SUMMARY.md" 
               "docs/design.md" "docs/usage.md" "docs/testing.md" "docs/self-hosting.md")
DOCS_FOUND=0

for doc in "${DOCS_EXPECTED[@]}"; do
    if [ -f "$doc" ]; then
        DOCS_FOUND=$((DOCS_FOUND + 1))
        echo -e "${GREEN}✓${NC} $doc exists"
    else
        echo -e "${RED}✗${NC} $doc missing"
    fi
done
echo ""

# Summary
echo "=========================================="
echo "Verification Summary"
echo "=========================================="
echo -e "Build:        ${GREEN}✓${NC} Success"
echo -e "Tests:        ${GREEN}✓${NC} All passed"
echo -e "Examples:     ${GREEN}${EXAMPLE_SUCCESS}/${EXAMPLE_COUNT}${NC} compiled"
echo -e "Documentation: ${GREEN}${DOCS_FOUND}/${#DOCS_EXPECTED[@]}${NC} files"
echo ""

# Final status
if [ $EXAMPLE_SUCCESS -eq $EXAMPLE_COUNT ] && [ $DOCS_FOUND -eq ${#DOCS_EXPECTED[@]} ]; then
    echo -e "${GREEN}=========================================="
    echo "✓ ALL VERIFICATIONS PASSED"
    echo "==========================================${NC}"
    echo ""
    echo "The SYSY Compiler is ready to use!"
    echo ""
    echo "Quick Start:"
    echo "  ./bin/sysyc examples/hello.sy -o hello.s"
    echo "  ./bin/sysyc examples/fibonacci.sy -ir"
    echo ""
    echo "Documentation:"
    echo "  cat README.md"
    echo "  cat docs/usage.md"
    exit 0
else
    echo -e "${RED}=========================================="
    echo "✗ SOME VERIFICATIONS FAILED"
    echo "==========================================${NC}"
    exit 1
fi
