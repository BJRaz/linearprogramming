# Linear Programming Solver

A C++ implementation of a linear programming solver using the simplex method. Parses LP problems from text files and computes optimal solutions.

## Quick Start

### Build

```bash
make clean && make
```

### Run

```bash
echo "tests/problems/mathhx.dk.txt" | ./bin/lpmatrix
```

## Overview

This project implements a complete linear programming solver with:

- **Lexical Analysis**: Hand-written scanner with comment support
- **Syntax Analysis**: Recursive descent parser
- **Optimization**: Simplex method for solving LP problems
- **Symbol Management**: Symbol table for variable tracking
- **Matrix Operations**: Generic matrix class with arithmetic operations

## Features

✅ Solve linear programming problems (maximize/minimize)  
✅ Support for inequality (`<`, `>`) and equality (`=`) constraints  
✅ Line comments (`//`) and block comments (`/* */`)  
✅ Decimal number support  
✅ Multiple variable names  

## Problem Format

Define LP problems as inequalities with coefficients and variables:

```
// Constraints
+ 1 bukser + 1.5 jakker < 750 ;
+ 2 bukser + 1 jakker < 1000 ;

/* Objective function */
+ 500 bukser + 400 jakker = 0 ;
```

**Syntax**:
- Each constraint ends with `;`
- Use `+` or `-` for term signs
- Inequality constraints: `<` or `>`
- Objective function: `=`
- Comments: `//` (line) or `/* */` (block)

## Building

### Requirements

- C++11 compatible compiler (clang++, g++)
- Make
- macOS or Unix-like environment

### Commands

```bash
# Clean build
make clean

# Build library and executable
make

# Force rebuild of main executable
make -B bin/lpmatrix

# Build test executable
make clean && make build/test
```

Output:
- `bin/lpmatrix` - Main solver executable
- `bin/liblp.a` - Static library
- `bin/*.o` - Object files

## Usage

### Interactive Mode

```bash
./bin/lpmatrix
```

The program prompts for a filename and solves the LP problem.

### With Input Redirection

```bash
echo "tests/problems/mathhx.dk.txt" | ./bin/lpmatrix
```

### Output Example

```
Ingen fejl i lexikalsk analyse:
Ingen fejl i syntaktisk analyse:

Matricen før løsningen er gennemført:
Column:   1    2    3    4    5 
         ---------------------
Row 1:    1   1.5   1    0   750 
Row 2:    2    1    0    1  1000 
Row 3:  500  400    0    0    0 

Den endelige LPMatrix:
Column:   1    2    3    4    5 
         ---------------------
Row 1:    0    1    1  -0.5   250 
Row 2:    1    0  -0.5   0.8   375 
Row 3:    0    0 -150 -175 -287500 

Loesning paa optimeringsproblemet:
BUKSER     =   375.00
JAKKER     =   250.00
K          = 287500.00
```

## Project Structure

```
.
├── README.md                 # This file
├── INSTRUCTIONS.md           # Detailed documentation
├── Makefile                  # Build configuration
├── include/                  # Header files
│   ├── LP.h                 # Main LP solver
│   ├── LPMatrix.h           # Matrix representation
│   ├── Matrix.h             # Generic matrix class
│   ├── StandardLP.h         # Standard form solver
│   ├── StraffeLP.h          # Alternative simplex variant
│   ├── SymbolTabel.h        # Symbol table
│   ├── Token.h              # Token definitions
│   ├── Tokenizer.h          # Tokenizer class
│   └── STLdef.h             # STL definitions
├── src/                      # Source files
│   ├── LP.cpp               # Lexer and parser
│   ├── LPMatrix.cpp         # Matrix implementation
│   ├── StandardLP.cpp       # Standard solver
│   ├── StraffeLP.cpp        # Alternative solver
│   ├── SymbolTabel.cpp      # Symbol table implementation
│   ├── Token.cpp            # Token implementation
│   ├── Tokenizer.cpp        # Tokenizer implementation
│   ├── StringTokenizer.cpp  # String splitting utility
│   ├── test.cpp             # Test/main program
│   └── lpsolve.py           # Python interface
├── tests/                    # Test files
│   ├── matrix/              # Matrix test problems
│   └── problems/            # LP problem examples
└── bin/                      # Build output (generated)

```

## Implementation Details

### Lexical Analysis

The scanner (`LP::Scan()` in `src/LP.cpp`) performs character-by-character tokenization:

- **Whitespace**: Automatically skipped
- **Comments**: 
  - Line comments: `//` skips to end of line
  - Block comments: `/* */` skips until closing `*/`
- **Operators**: `+`, `-`, `<`, `>`, `=`, `;`
- **Numbers**: Integer and decimal support
- **Identifiers**: Variable names (alphabetic + digits)

### Parsing

Recursive descent parser with grammar:

```
<ulighed> ::= <termliste> (<|>) NUM | <termliste> = NUM
<termliste> ::= <term> <hjtermliste>
<hjtermliste> ::= +<termliste> | -<termliste> | ε
<term> ::= +NUM ID | -NUM ID
```

### Solver

Uses the simplex method to optimize linear programming problems:

1. Convert to standard form with slack/artificial variables
2. Build initial tableau
3. Apply simplex algorithm pivoting
4. Extract solution from final tableau

## Test Files

Sample problems in `tests/problems/`:

- `mathhx.dk.txt` - Optimization problem example
- `test_comments.txt` - Demonstration of comment styles
- Additional test cases for various problem types

## Troubleshooting

| Issue | Solution |
|-------|----------|
| "Filen kan ikke aabnes" | Check file path is correct |
| Lexical analysis errors | Verify file contains only valid tokens |
| Parse errors | Check constraint syntax and semicolons |
| Compiler errors | Ensure C++11 support and all headers present |

## Development

### Adding New Test Cases

Create a new `.txt` file in `tests/problems/`:

```
// My test case
+ 2 x + 3 y < 100 ;
+ 1 x + 1 y < 50 ;
+ 10 x + 15 y = 0 ;
```

Then run:

```bash
echo "tests/problems/mytest.txt" | ./bin/lpmatrix
```

### Extending the Solver

Key entry points for modification:

- **Scanner changes**: Modify `LP::Scan()` in `src/LP.cpp`
- **Parser changes**: Modify grammar functions in `src/LP.cpp`
- **Solver changes**: Extend `StandardLP` or `StraffeLP` in `src/`
- **Matrix operations**: Extend `Matrix` class in `include/Matrix.h`

## Language Notes

The output messages are in Danish:
- `Ingen fejl` = No errors
- `Ulighed` = Inequality
- `Løsning` = Solution
- `Fejl` = Error

## License

Educational project for linear programming implementation.

---

**Status**: Active development on `ai` branch

For detailed documentation, see [INSTRUCTIONS.md](INSTRUCTIONS.md)
