# GitHub Copilot Instructions for Linear Programming Solver

## Overview

This file contains instructions for using GitHub Copilot effectively with the Linear Programming Solver project. Copilot can assist with code generation, debugging, documentation, and extending this C++ linear programming solver.

## Project Context for Copilot

**Project Type**: C++ Linear Programming Solver  
**Architecture**: Hand-written lexer → Recursive descent parser → Simplex solver  
**Language**: C++11  
**Build System**: Make  
**Build Command**: `make clean && make`

### Key Files Copilot Should Know

- `src/LP.cpp` - Lexical analysis & parsing (main logic); contains corrected BNF grammar
- `include/LP.h` - LP class definition
- `src/StandardLP.cpp` - Simplex solver implementation
- `include/Matrix.h` - Matrix operations
- `src/test.cpp` - Output formatting (matrices, inequalities, solutions)
- `src/LPMatrix.cpp` - Matrix pretty-printing with aligned columns
- `tests/problems/*.txt` - Problem file examples

### Recent Improvements (April 2026)

**Output Formatting**:
- Matrix output now uses aligned columns with dynamic column widths (see `src/LPMatrix.cpp`).
- Inequality/constraint printing is parsed and aligned for readability (see `WriteUligheder` in `src/test.cpp`).
- Solution printing uses right-aligned numeric values (see `WriteBasisSolution` in `src/test.cpp`).

**Parser Grammar**:
- The BNF grammar in `src/LP.cpp` has been corrected and clarified (see header comment).
- Current parser requires explicit '+' or '-' before each term. Future enhancement: support implicit leading '+'.

## How to Use Copilot with This Project

### 1. Code Generation

**Ask Copilot to**:
- Generate new problem solver variants (extending `StandardLP`)
- Create helper functions for matrix operations
- Add new token types to the lexer
- Generate test cases for constraint parsing

**Example prompt**:
```
Generate a new solver class called "DualLP" that implements 
the dual simplex algorithm, extending from StandardLP class.
```

### 2. Bug Fixes

**When debugging**:
- Show Copilot the error message and surrounding code
- Ask for specific fixes to lexical or syntax errors
- Request validation of parser logic
- Help optimize matrix operations

**Example prompt**:
```
The parser is incorrectly handling negative coefficients 
in constraints. Debug the Ulighed() method in LP.cpp.
```

### 3. Code Refactoring

**Ask Copilot to**:
- Simplify the main lexer loop in `Scan()`
- Extract helper methods from large functions
- Improve code documentation
- Modernize C++ code (C++11 features)

**Example prompt**:
```
Refactor the switch statement in LP::Scan() to use 
a more maintainable approach for token classification.
```

### 4. Documentation

**Copilot can help create**:
- Function documentation (Doxygen style)
- Code comments explaining complex algorithms
- README sections
- Algorithm explanations

**Example prompt**:
```
Add Doxygen documentation comments to the simplex algorithm 
methods in StandardLP.cpp.
```

### 5. Feature Implementation

**Common requests**:
- Add support for new constraint types
- Implement problem file format variations
- Create visualization/output formatters
- Add optimization enhancements
- Improve parser usability (e.g., implicit leading '+' for first term)

**Example prompt**:
```
Add support for variable bounds (e.g., x >= 5, y <= 10) 
in the LP problem file format. Update lexer and parser.
```

### 6. Output & Formatting

**Ask Copilot to**:
- Improve matrix, constraint, or solution output alignment
- Add CSV or JSON export formatters
- Create visualization routines for simplex pivots
- Enhance error message formatting

**Example prompt**:
```
Improve the matrix pretty-printer in LPMatrix.cpp to show 
column headers and use consistent numeric precision.
```

## Copilot Prompt Patterns

### Pattern 1: Code Completion
Place cursor in code and use:
```
Ctrl+Enter (or Cmd+Enter on Mac)
```
Copilot suggests completions based on context.

### Pattern 2: Explaining Code
**Prompt**: "Explain the simplex pivoting logic in this code"  
Copilot will break down complex algorithms.

### Pattern 3: Creating Tests
**Prompt**:
```
Create a C++ test function that validates the parser 
correctly handles the constraint:
+ 2 x + 3.5 y < 100 ;
```

### Pattern 4: Extending Functionality
**Prompt**:
```
Add a method to the LP class that reads multiple 
problem files and solves them sequentially.
```

## Code Areas Where Copilot Excels

### Lexer Improvements
- Adding new token types
- Handling edge cases in character processing
- Comment style variations (// and /* */ fully supported)
- Error recovery strategies

### Parser Enhancement
- Grammar rule additions (see corrected BNF in `src/LP.cpp`)
- Error messages and user-friendly diagnostics
- Operator precedence and associativity handling
- Variable validation and symbol table management

### Output Formatting
- Matrix pretty-printing with aligned columns
- Constraint expression parsing and alignment
- Solution display with dynamic column widths
- Numeric precision and alignment logic (iomanip-based)

### Solver Extensions
- Alternative algorithms (Dual Simplex, Interior Point)
- Constraint type variations
- Performance optimizations
- Numerical stability improvements

### Testing & Validation
- Test case generation
- Validation functions
- Problem file format checkers
- Result verification and output comparison

## Common Copilot Requests

### Request 1: Add Logging
```
Add debug logging throughout the LP::Scan() method 
to trace token recognition. Use a LOG macro.
```

### Request 2: Input Validation
```
Create a validation function that checks if a loaded 
problem is in proper standard form for the solver.
```

### Request 3: Performance Analysis
```
Identify potential performance bottlenecks in the 
Matrix class and suggest optimizations.
```

### Request 4: Parser Usability Enhancement
```
Modify the parser to accept constraints where the first 
term does not require an explicit '+' sign (implicit leading '+'). 
Update LP::AddOp() and LP::Term() in src/LP.cpp.
```

### Request 5: Format Conversion
```
Create a converter function that reads LP files in 
standard mathematical notation and converts them 
to our internal format.
```

### Request 6: Output Export
```
Add functions to export the final matrix and solution 
in CSV or JSON format.
```

## Tips for Effective Copilot Usage

1. **Provide Context**: Show Copilot relevant code sections
2. **Be Specific**: Use exact class/method names from the project
3. **Reference Files**: Mention specific file paths (e.g., `src/LP.cpp`)
4. **Show Examples**: Include sample problem files or expected outputs
5. **Clarify Requirements**: Explain what you want to achieve, not just what to code

## What Copilot Might Struggle With

- Deep understanding of the simplex algorithm details
- Maintaining consistency across multiple files
- Domain-specific optimization knowledge
- Mathematical correctness verification

**Solution**: Guide Copilot by providing algorithm references or constraints.

## Copilot Chat Workflows

### Workflow 1: Understanding the Codebase
```
User: Explain how the lexer handles comments in LP.cpp
Copilot: [Explains comment processing logic]
User: Can you show me the exact code section?
Copilot: [Shows code snippet]
User: How would I add support for nested comments?
Copilot: [Suggests implementation]
```

### Workflow 2: Feature Development
```
User: I want to add bounds checking for variables
Copilot: [Suggests approach]
User: Can you generate the parsing code for bounds?
Copilot: [Generates code]
User: How do I integrate this into the solver?
Copilot: [Provides integration guidance]
```

### Workflow 3: Bug Fixing
```
User: The parser crashes on this input: [shows file]
Copilot: [Identifies potential issue]
User: Can you show me a fix?
Copilot: [Provides code fix]
User: Are there other places this might occur?
Copilot: [Checks codebase, suggests preventive fixes]
```

## Best Practices

✅ **DO**:
- Ask Copilot to explain algorithms before modifying them
- Use Copilot for boilerplate code generation
- Ask for code reviews of complex sections
- Request performance improvement suggestions
- Use it to generate test cases

❌ **DON'T**:
- Accept code without understanding it
- Rely on Copilot for mathematical correctness
- Use generated code without testing
- Ignore compilation warnings it might suggest
- Skip manual review of algorithm changes

## Example Prompts by Task

| Task | Prompt |
|------|--------|
| Add new operator | "Add support for the `<=` operator to the lexer" |
| Optimize matrix | "Optimize the matrix multiplication in Matrix.h using cache locality" |
| Handle errors | "Add comprehensive error handling to the parser" |
| Create formatter | "Create a function to output the solution in CSV format" |
| Document code | "Add detailed comments explaining the recursive descent parser" |
| Extend solver | "Implement the Two-Phase Simplex Method" |
| Generate tests | "Create test cases for edge cases in decimal number parsing" |

---

**Remember**: Copilot is a productivity tool. Always verify generated code correctness, especially for algorithms and mathematical operations.
