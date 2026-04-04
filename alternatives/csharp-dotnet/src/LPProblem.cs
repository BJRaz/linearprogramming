using System.Collections.Generic;

namespace LPSolver
{
    public enum ConstraintType
    {
        Less,    // <
        Greater, // >
        Equal    // =
    }

    public class Constraint
    {
        public Dictionary<string, double> Coefficients { get; }
        public ConstraintType Operator { get; }
        public double RHS { get; }

        public Constraint(Dictionary<string, double> coefficients, ConstraintType op, double rhs)
        {
            Coefficients = coefficients;
            Operator = op;
            RHS = rhs;
        }
    }

    public class LPProblem
    {
        public List<Constraint> Constraints { get; }
        public Constraint? ObjectiveFunction { get; }

        public LPProblem(List<Constraint> constraints, Constraint? objective)
        {
            Constraints = constraints;
            ObjectiveFunction = objective;
        }
    }
}
