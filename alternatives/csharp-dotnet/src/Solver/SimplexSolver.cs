using System;
using System.Collections.Generic;
using System.Linq;

namespace LPSolver.Solver
{
    public class SimplexSolver
    {
        private readonly LPProblem _problem;
        private Matrix _tableau;
        private Dictionary<int, string> _variableMap;
        private const double Epsilon = 1e-10;

        public SimplexSolver(LPProblem problem)
        {
            _problem = problem;
            _variableMap = new Dictionary<int, string>();
            _tableau = null!;
        }

        public SolverResult Solve()
        {
            try
            {
                BuildTableau();

                int iterations = 0;
                int maxIterations = 1000;

                // Simplex algorithm iterations
                while (CanImprove() && iterations < maxIterations)
                {
                    var pivotCol = FindEnteringVariable();
                    var pivotRow = FindLeavingVariable(pivotCol);

                    if (pivotRow < 0)
                        return new SolverResult { IsOptimal = false, Message = "Problem is unbounded" };

                    Pivot(pivotRow, pivotCol);
                    iterations++;
                }

                if (iterations >= maxIterations)
                    return new SolverResult { IsOptimal = false, Message = "Max iterations exceeded" };

                return ExtractSolution();
            }
            catch (Exception ex)
            {
                return new SolverResult { IsOptimal = false, Message = $"Solver error: {ex.Message}" };
            }
        }

        private void BuildTableau()
        {
            // Collect all variables
            var variables = new HashSet<string>();
            foreach (var constraint in _problem.Constraints)
            {
                foreach (var var in constraint.Coefficients.Keys)
                    variables.Add(var);
            }

            if (_problem.ObjectiveFunction != null)
            {
                foreach (var var in _problem.ObjectiveFunction.Coefficients.Keys)
                    variables.Add(var);
            }

            var varList = variables.OrderBy(v => v).ToList();
            int rows = _problem.Constraints.Count + 1; // +1 for objective
            int cols = varList.Count + _problem.Constraints.Count + 1; // +slack vars + RHS

            _tableau = new Matrix(rows, cols);

            // Build constraint rows
            for (int i = 0; i < _problem.Constraints.Count; i++)
            {
                var constraint = _problem.Constraints[i];
                for (int j = 0; j < varList.Count; j++)
                {
                    var coefficient = constraint.Coefficients.ContainsKey(varList[j])
                        ? constraint.Coefficients[varList[j]]
                        : 0.0;
                    _tableau[i, j] = coefficient;
                }

                // Add slack variables
                _tableau[i, varList.Count + i] = constraint.Operator == ConstraintType.Less ? 1 : -1;
                _tableau[i, cols - 1] = constraint.RHS;
            }

            // Build objective row
            int objRow = rows - 1;
            if (_problem.ObjectiveFunction != null)
            {
                for (int i = 0; i < varList.Count; i++)
                {
                    var coefficient = _problem.ObjectiveFunction.Coefficients.ContainsKey(varList[i])
                        ? _problem.ObjectiveFunction.Coefficients[varList[i]]
                        : 0.0;
                    _tableau[objRow, i] = -coefficient; // Negative for maximization
                }
            }

            // Map variables
            for (int i = 0; i < varList.Count; i++)
                _variableMap[i] = varList[i];
        }

        private bool CanImprove()
        {
            // Check if any negative values in objective row (last row)
            int objRow = _tableau.Rows - 1;
            for (int j = 0; j < _tableau.Columns - 1; j++)
            {
                if (_tableau[objRow, j] < -Epsilon)
                    return true;
            }
            return false;
        }

        private int FindEnteringVariable()
        {
            int enteringCol = 0;
            double minValue = 0;
            int objRow = _tableau.Rows - 1;

            for (int j = 0; j < _tableau.Columns - 1; j++)
            {
                if (_tableau[objRow, j] < minValue)
                {
                    minValue = _tableau[objRow, j];
                    enteringCol = j;
                }
            }

            return enteringCol;
        }

        private int FindLeavingVariable(int pivotCol)
        {
            int leavingRow = -1;
            double minRatio = double.MaxValue;

            for (int i = 0; i < _tableau.Rows - 1; i++)
            {
                if (_tableau[i, pivotCol] > Epsilon)
                {
                    var ratio = _tableau[i, _tableau.Columns - 1] / _tableau[i, pivotCol];
                    if (ratio >= -Epsilon && ratio < minRatio)
                    {
                        minRatio = ratio;
                        leavingRow = i;
                    }
                }
            }

            return leavingRow;
        }

        private void Pivot(int pivotRow, int pivotCol)
        {
            var pivotElement = _tableau[pivotRow, pivotCol];

            // Divide pivot row by pivot element
            for (int j = 0; j < _tableau.Columns; j++)
                _tableau[pivotRow, j] /= pivotElement;

            // Eliminate column
            for (int i = 0; i < _tableau.Rows; i++)
            {
                if (i != pivotRow)
                {
                    var factor = _tableau[i, pivotCol];
                    for (int j = 0; j < _tableau.Columns; j++)
                        _tableau[i, j] -= factor * _tableau[pivotRow, j];
                }
            }
        }

        private SolverResult ExtractSolution()
        {
            var solution = new Dictionary<string, double>();
            int objRow = _tableau.Rows - 1;
            double objectiveValue = _tableau[objRow, _tableau.Columns - 1];

            // Extract variable values
            foreach (var kvp in _variableMap)
            {
                double value = 0.0;
                // Check if this column is a basic variable
                bool isBasic = false;
                int basicRow = -1;

                for (int i = 0; i < _tableau.Rows - 1; i++)
                {
                    if (Math.Abs(_tableau[i, kvp.Key]) > Epsilon &&
                        Math.Abs(_tableau[i, kvp.Key] - 1.0) < Epsilon)
                    {
                        // Check if this is the only non-zero entry in the column
                        bool onlyOne = true;
                        for (int j = 0; j < _tableau.Rows; j++)
                        {
                            if (j != i && Math.Abs(_tableau[j, kvp.Key]) > Epsilon)
                            {
                                onlyOne = false;
                                break;
                            }
                        }
                        if (onlyOne)
                        {
                            isBasic = true;
                            basicRow = i;
                            break;
                        }
                    }
                }

                if (isBasic && basicRow >= 0)
                {
                    value = _tableau[basicRow, _tableau.Columns - 1];
                }

                solution[kvp.Value] = Math.Max(0, value); // Non-negative solution
            }

            return new SolverResult
            {
                IsOptimal = true,
                Solution = solution,
                ObjectiveValue = objectiveValue
            };
        }
    }

    public class SolverResult
    {
        public bool IsOptimal { get; set; }
        public Dictionary<string, double> Solution { get; set; } = new();
        public double ObjectiveValue { get; set; }
        public string Message { get; set; } = "";
    }
}
