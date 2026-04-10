# Simplex Visualizer - Implementation Summary

## Overview

The SimplexVisualizer is a debugging and educational tool that provides detailed visualization of the simplex algorithm's execution. It tracks each pivot operation, records tableau state before and after each pivot, and produces a comprehensive summary of all iterations.

## Features

### 1. Iteration-by-Iteration Tracking
- Displays each simplex iteration with clear section headers
- Shows which variable is entering (becoming basic) and leaving (becoming non-basic)
- Reports the pivot element value and current objective function value

### 2. Tableau Snapshots
- Captures the entire tableau state before each pivot operation
- Captures the tableau state after each pivot operation
- Both snapshots use aligned column formatting for easy comparison

### 3. Algorithm Summary
- After the simplex algorithm terminates, prints a summary table
- Shows total number of iterations performed
- Lists all pivots in tabular format with:
  - Iteration number
  - Pivot row and column
  - Pivot element value
  - Objective value after iteration

### 4. Formatted Output
- Uses dynamic column alignment matching the main matrix pretty-printer
- Maintains consistent numeric precision (6 decimal places)
- Clear visual separators between iterations

## Implementation Files

### `include/SimplexVisualizer.h`
- **PivotInfo struct**: Stores information about each pivot operation
  - Iteration number
  - Pivot row and column indices
  - Pivot element value
  - Objective value
  - Optional variable names (for enhanced reporting)

- **SimplexVisualizer class**: Main visualization engine
  - `StartIteration()`: Marks the beginning of a new iteration
  - `RecordTableauBefore()`: Captures tableau before pivot
  - `RecordTableauAfter()`: Captures tableau after pivot
  - `PrintIterationSummary()`: Prints current iteration details
  - `PrintAllIterations()`: Prints algorithm summary table at termination

### `src/SimplexVisualizer.cpp`
- Full implementation of visualization methods
- Helper function `FormatNumber()` for consistent numeric formatting
- Maintains iteration history in vector of PivotInfo structures
- Tableau snapshots stored as LPMatrix copies

### Integration Points

1. **`include/StandardLP.h`**
   - Added forward declaration: `class SimplexVisualizer;`
   - Added member: `SimplexVisualizer *visualizer;`

2. **`src/StandardLP.cpp`**
   - Constructor: Creates visualizer instance with trace flag
   - Destructor: Cleans up visualizer pointer
   - SolveLP() method: Calls visualization methods at appropriate points:
     - `StartIteration()` at iteration start
     - `RecordTableauBefore()` before pivot
     - `RecTableauAfter()` after pivot
     - `PrintIterationSummary()` after each iteration
     - `PrintAllIterations()` at algorithm termination

### `Makefile`
- Updated OBJS list to include `SimplexVisualizer.o`
- Ensures SimplexVisualizer.cpp is compiled and linked

### `src/test.cpp`
- Modified to enable trace mode by default: `lp.Run(LpProblem, true)`
- Users can disable visualization by passing `false` if needed

## Usage

### Enable Visualization (Default)
```cpp
LP lp;
lp.Run("path/to/problem.txt", true);  // trace=true enables visualization
```

### Disable Visualization
```cpp
LP lp;
lp.Run("path/to/problem.txt", false);  // trace=false disables visualization
```

### Example Output

```
================================================================================
SIMPLEX ITERATION 1
================================================================================

Pivot Operation:
  Entering variable:  (Col 2)
  Leaving variable:   (Row 3)
  Pivot element:      1.000000
  Objective value:    -15000.00

Tableau Before Pivot:
[Matrix display...]

Tableau After Pivot:
[Matrix display...]

================================================================================
SIMPLEX ALGORITHM SUMMARY
================================================================================
Total iterations: 2

Iter Row     Col     Pivot       Objective      Entering / Leaving
--------------------------------------------------------------------------------
1    2       2       1.0000      0.00           
2    3       1       1.0000      -15000.00      
--------------------------------------------------------------------------------
```

## Performance Considerations

- Visualization has minimal performance impact on typical problems
- Each iteration stores a copy of the tableau (memory trade-off for educational value)
- Can be disabled completely by passing `trace=false` to LP::Run()

## Educational Value

The SimplexVisualizer helps developers and students:
1. **Understand the simplex algorithm** step-by-step
2. **Debug LP solvers** by observing pivot operations
3. **Verify correctness** by inspecting tableau transformations
4. **Learn optimization** through visualization of basis changes

## Future Enhancements

Potential improvements:
- Add variable names to entering/leaving variable display
- Track and display reduced costs
- Highlight negative reduced cost that drove variable selection
- Support for graphical visualization (2D/3D plotting)
- Statistics (iterations to convergence, computation time)
- Export visualization to CSV/JSON format
