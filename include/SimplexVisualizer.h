/*
SimplexVisualizer.h

Visualization routines for simplex algorithm pivoting operations.
Tracks pivot history, displays tableau snapshots, and generates 
iteration summaries for debugging and educational purposes.

Usage:
  SimplexVisualizer viz;
  viz.StartIteration(pivot_row, pivot_col, pivot_element);
  viz.RecordTableau(A, m, n);  // Record state before/after pivot
  viz.PrintIterationSummary();
*/

#ifndef SIMPLEX_VISUALIZER_H
#define SIMPLEX_VISUALIZER_H

#include "STLdef.h"
#include "LPMatrix.h"
#include <iomanip>
#include <sstream>

struct PivotInfo
{
	size_t iteration;
	size_t pivot_row;
	size_t pivot_col;
	double pivot_element;
	double objective_value;
	string entering_var;
	string leaving_var;
};

class SimplexVisualizer
{
public:
	SimplexVisualizer(bool enabled = true);
	~SimplexVisualizer() {}

	// Enable/disable visualization
	void SetEnabled(bool enabled) { bEnabled = enabled; }

	// Record pivot information
	void StartIteration(size_t iteration, size_t row, size_t col, 
	                     double pivot, double obj_val = 0.0);
	
	// Record tableau state (before or after pivot)
	void RecordTableauBefore(const LPMatrix &A, const string &label = "Before Pivot");
	void RecordTableauAfter(const LPMatrix &A, const string &label = "After Pivot");

	// Set variable names for reporting
	void SetEnteringVar(const string &var) { current_pivot.entering_var = var; }
	void SetLeavingVar(const string &var) { current_pivot.leaving_var = var; }

	// Print visualizations
	void PrintIterationHeader();
	void PrintIterationSummary();
	void PrintPivotInfo();
	void PrintTableauSnapshot(const LPMatrix &A, const string &title);
	void PrintAllIterations();

	// Get pivot history
	const vector<PivotInfo> &GetPivotHistory() const { return pivot_history; }
	size_t GetIterationCount() const { return pivot_history.size(); }

private:
	bool bEnabled;
	size_t iteration_count;
	PivotInfo current_pivot;
	vector<PivotInfo> pivot_history;
	LPMatrix tableau_before;
	LPMatrix tableau_after;
	bool has_before;
	bool has_after;

	// Helper methods
	void PrintSeparator(size_t width = 80);
	string FormatNumber(double val, int width = 10, int precision = 2);
};

#endif
