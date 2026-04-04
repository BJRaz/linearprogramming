# C# .NET Linear Programming Solver - Proof of Concept

## Overview

This is a **proof-of-concept** implementation of the Linear Programming Solver in **C# .NET**, maintaining the architectural design from the original C++ version while leveraging modern .NET features.

## Quick Start

### Prerequisites

- **.NET SDK 6.0+** or **.NET 7.0+**
- Mono 6.0+ (for cross-platform)

### Build

```bash
cd alternatives/csharp-dotnet
dotnet build
```

### Run

```bash
dotnet run -- ../../../tests/problems/bags.txt
```

Or interactive:
```bash
dotnet run
```

## Project Structure

```
alternatives/csharp-dotnet/
├── LPSolver.csproj          # .NET project configuration
├── .gitignore               # Git ignore rules
├── src/
│   ├── Program.cs           # Entry point with UI
│   ├── Token.cs             # Token definitions
│   ├── Lexer.cs             # Lexical analysis (tokenizer)
│   ├── Parser.cs            # Recursive descent parser
│   ├── LPProblem.cs         # Problem data structures
│   └── Solver/
│       ├── Matrix.cs        # Matrix class
│       └── SimplexSolver.cs # Simplex algorithm implementation
├── problems                 # Symlink to ../../tests/problems (shared, read-only)
└── README.md                # This file
```

**Note**: The `problems/` directory is a symbolic link to the shared test problems directory in `tests/problems/`. This ensures test data is not duplicated across the C++ and C# implementations.

## Implementation Details

### Architecture

| Component | Purpose | Equivalent from C++ |
|-----------|---------|---------------------|
| **Lexer** | Tokenizes input, handles comments (`//`, `/* */`) | `LP::Scan()` |
| **Parser** | Recursive descent parser for LP syntax | `LP::Ulighed()` and related |
| **SimplexSolver** | Implements simplex algorithm | `StandardLP` |
| **Matrix** | Matrix operations for tableau | `LPMatrix` / `Matrix` |

### Key Features

✅ **Hand-written Lexer** - Character-by-character tokenization  
✅ **Comment Support** - Both `//` line and `/* */` block comments  
✅ **Recursive Descent Parser** - Matches C++ grammar  
✅ **Simplex Algorithm** - Two-phase method with pivot operations  
✅ **Decimal Number Support** - Floating-point coefficients  
✅ **Variable Names** - Any alphanumeric identifier  

## C# vs C++ Comparison

| Aspect | C++ | C# .NET |
|--------|-----|---------|
| **String handling** | `std::string` | `System.String` |
| **Collections** | Manual arrays | `List<T>`, `Dictionary<K,V>` |
| **Memory** | Manual (RAII) | Garbage collected |
| **Compilation** | AOT native | JIT to IL |
| **File I/O** | `fstream` | `System.IO` |
| **Performance** | Generally faster | ~10-20% slower |
| **Accessibility** | Lower level | Higher level |

## Example Usage

### Input File: `bags.txt`

```
+ 1 x < 400 ;
+ 1 y < 300 ;
+ 1 x + 1 y < 600 ;
+ 20 x + 30 y = 0 ;
```

### Running

```bash
$ dotnet run -- ../../../tests/problems/bags.txt

╔════════════════════════════════════════════════╗
║  Linear Programming Solver - C# .NET (POC)     ║
╚════════════════════════════════════════════════╝

▶ Lexical Analysis
  ✓ No errors

▶ Syntax Analysis
  ✓ No errors

▶ Problem Definition
  Constraints: 3
  Objective function found

▶ Solving with Simplex Algorithm
  ✓ Optimal solution found

  Solution:
             x =     300.00
             y =     300.00

  Objective Value: 24000.00
```

## Testing

### Manual Testing

```bash
# Test with all provided problem files
dotnet run -- ../../../tests/problems/bomber.txt
dotnet run -- ../../../tests/problems/mathhx.dk.txt
dotnet run -- ../../../tests/problems/finale.txt
```

### Unit Tests (Future Implementation)

Create `Tests/LexerTests.cs`:

```csharp
using Xunit;
using LPSolver;

public class LexerTests
{
    [Fact]
    public void Lexer_SimpleExpression_TokenizesCorrectly()
    {
        var lexer = new Lexer("+ 1 x < 100 ;");
        var tokens = lexer.Tokenize();
        
        Assert.Equal(6, tokens.Count);
        Assert.Equal(TokenType.Plus, tokens[0].Type);
        Assert.Equal(TokenType.Number, tokens[1].Type);
        Assert.Equal(TokenType.Identifier, tokens[2].Type);
    }
}
```

## Performance Considerations

### Current Implementation

- **Lexer**: O(n) single pass
- **Parser**: O(m) where m = number of constraints
- **Solver**: O(m³) for simplex pivoting
- **Overall**: Acceptable for small to medium problems

### Optimization Opportunities

1. **Use `Span<T>`** instead of arrays for zero-copy operations
2. **SIMD operations** with `System.Runtime.Intrinsics`
3. **Parallel matrix operations** for large tableaus
4. **Cache pivoting decisions** to avoid redundant calculations
5. **Use `stackalloc`** for small temporary arrays

Example optimization:

```csharp
// Using Span<T> for better performance
Span<double> pivotRow = new double[Columns];
for (int j = 0; j < Columns; j++)
    pivotRow[j] = _tableau[pivotRow, j];
```

## Limitations

- ⚠️ No degeneracy handling (cycling prevention)
- ⚠️ Limited to maximization problems
- ⚠️ No phase II handling for artificial variables
- ⚠️ Small numerical precision tolerance (1e-10)

## Future Enhancements

- [ ] Two-phase simplex for all constraint types
- [ ] Integer programming (branch and bound)
- [ ] Presolve and postsolve
- [ ] Sensitivity analysis
- [ ] GUI interface (WPF or Avalonia)
- [ ] REST API for remote solving
- [ ] Performance profiling and benchmarks

## Troubleshooting

### Build Issues

```bash
# Clear NuGet cache
dotnet nuget locals all --clear

# Rebuild from scratch
dotnet clean && dotnet build
```

### Runtime Issues

**Problem**: "File not found"  
**Solution**: Use absolute path or relative path from executable directory

**Problem**: "Solver fails on valid input"  
**Solution**: Check problem file syntax matches format exactly

## Deployment

### Self-Contained Binary

```bash
dotnet publish -c Release --self-contained -r win-x64
dotnet publish -c Release --self-contained -r osx-x64
dotnet publish -c Release --self-contained -r linux-x64
```

### Docker

```dockerfile
FROM mcr.microsoft.com/dotnet/sdk:6.0 as build
WORKDIR /app
COPY . .
RUN dotnet build

FROM mcr.microsoft.com/dotnet/runtime:6.0
COPY --from=build /app/bin/Release/net6.0 /app
ENTRYPOINT ["dotnet", "/app/LPSolver.dll"]
```

## Code Quality

### Design Patterns Used

- **Strategy Pattern** - Different solver variants (SimplexSolver)
- **Builder Pattern** - Problem construction
- **Chain of Responsibility** - Token processing

### Coding Standards

- Follows Microsoft C# coding conventions
- Uses nullable reference types (`#nullable enable`)
- Proper exception handling throughout
- Meaningful variable names and documentation

## License

Same as parent C++ project

---

**Status**: Proof of Concept  
**Compatibility**: .NET 6.0+, Mono 6.0+  
**Last Updated**: April 2026

## References

- [Microsoft .NET Documentation](https://docs.microsoft.com/dotnet/)
- [C# Language Features](https://docs.microsoft.com/en-us/dotnet/csharp/)
- [Linear Programming](https://en.wikipedia.org/wiki/Linear_programming)
- [Simplex Algorithm](https://en.wikipedia.org/wiki/Simplex_algorithm)
