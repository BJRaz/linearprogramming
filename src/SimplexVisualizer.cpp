/*
SimplexVisualizer.cpp

Implementation of simplex pivot visualization routines.
*/

#include "SimplexVisualizer.h"

SimplexVisualizer::SimplexVisualizer(bool enabled)
	: bEnabled(enabled), iteration_count(0), has_before(false), has_after(false)
{
	current_pivot = {0, 0, 0, 0.0, 0.0, "", ""};
}

void SimplexVisualizer::StartIteration(size_t iteration, size_t row, size_t col,
                                       double pivot, double obj_val)
{
	if (!bEnabled) return;

	iteration_count = iteration;
	current_pivot.iteration = iteration;
	current_pivot.pivot_row = row;
	current_pivot.pivot_col = col;
	current_pivot.pivot_element = pivot;
	current_pivot.objective_value = obj_val;
	current_pivot.entering_var = "";
	current_pivot.leaving_var = "";

	has_before = false;
	has_after = false;
}

void SimplexVisualizer::RecordTableauBefore(const LPMatrix &A, const string &label)
{
	if (!bEnabled) return;
	tableau_before = A;
	has_before = true;
}

void SimplexVisualizer::RecordTableauAfter(const LPMatrix &A, const string &label)
{
	if (!bEnabled) return;
	tableau_after = A;
	has_after = true;
}

void SimplexVisualizer::PrintIterationHeader()
{
	if (!bEnabled) return;

	cout << "\n";
	PrintSeparator(80);
	cout << "SIMPLEX ITERATION " << iteration_count << endl;
	PrintSeparator(80);
}

void SimplexVisualizer::PrintPivotInfo()
{
	if (!bEnabled) return;

	cout << "\nPivot Operation:" << endl;
	cout << "  Entering variable:  " << (current_pivot.entering_var.empty() ? 
		"(Col " + std::to_string(current_pivot.pivot_col) + ")" : 
		current_pivot.entering_var) << endl;
	cout << "  Leaving variable:   " << (current_pivot.leaving_var.empty() ? 
		"(Row " + std::to_string(current_pivot.pivot_row) + ")" : 
		current_pivot.leaving_var) << endl;
	cout << "  Pivot element:      " << std::fixed << std::setprecision(6) 
	     << current_pivot.pivot_element << endl;
	cout << "  Objective value:    " << std::fixed << std::setprecision(2)
	     << current_pivot.objective_value << endl;
}

void SimplexVisualizer::PrintTableauSnapshot(const LPMatrix &A, const string &title)
{
	if (!bEnabled) return;

	cout << "\n" << title << ":" << endl;
	cout << A;
}

void SimplexVisualizer::PrintIterationSummary()
{
	if (!bEnabled) return;

	PrintIterationHeader();
	PrintPivotInfo();

	if (has_before)
		PrintTableauSnapshot(tableau_before, "Tableau Before Pivot");

	if (has_after)
		PrintTableauSnapshot(tableau_after, "Tableau After Pivot");

	// Record this iteration
	pivot_history.push_back(current_pivot);

	PrintSeparator(80);
}

void SimplexVisualizer::PrintAllIterations()
{
	if (!bEnabled || pivot_history.empty())
	{
		cout << "No iterations recorded." << endl;
		return;
	}

	cout << "\n";
	PrintSeparator(80);
	cout << "SIMPLEX ALGORITHM SUMMARY" << endl;
	PrintSeparator(80);
	cout << "Total iterations: " << pivot_history.size() << endl << endl;

	cout << std::left << std::setw(5) << "Iter"
	     << std::setw(8) << "Row"
	     << std::setw(8) << "Col"
	     << std::setw(12) << "Pivot"
	     << std::setw(15) << "Objective"
	     << "Entering / Leaving" << endl;
	PrintSeparator(80);

	for (const auto &p : pivot_history)
	{
		cout << std::left << std::setw(5) << p.iteration
		     << std::setw(8) << p.pivot_row
		     << std::setw(8) << p.pivot_col
		     << std::setw(12) << std::fixed << std::setprecision(4) << p.pivot_element
		     << std::setw(15) << std::fixed << std::setprecision(2) << p.objective_value;
		
		if (!p.entering_var.empty() || !p.leaving_var.empty())
		{
			cout << p.entering_var << " / " << p.leaving_var;
		}
		cout << endl;
	}

	PrintSeparator(80);
}

void SimplexVisualizer::PrintSeparator(size_t width)
{
	cout << std::string(width, '-') << endl;
}

string SimplexVisualizer::FormatNumber(double val, int width, int precision)
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(precision) << val;
	string s = oss.str();
	if (s.length() < (size_t)width)
		s = std::string(width - s.length(), ' ') + s;
	return s;
}
