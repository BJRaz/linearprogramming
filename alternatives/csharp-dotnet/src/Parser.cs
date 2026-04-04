using System;
using System.Collections.Generic;

namespace LPSolver
{
    public class Parser
    {
        private readonly List<Token> _tokens;
        private int _position = 0;
        private List<string> _errors = new();

        public Parser(List<Token> tokens)
        {
            _tokens = tokens;
        }

        public LPProblem Parse()
        {
            var constraints = new List<Constraint>();
            Constraint? objective = null;

            while (!IsAtEnd())
            {
                var constraint = ParseConstraint();
                if (constraint != null)
                {
                    // Last constraint is objective function (= 0)
                    if (constraint.Operator == ConstraintType.Equal)
                    {
                        objective = constraint;
                    }
                    else
                    {
                        constraints.Add(constraint);
                    }
                }
            }

            return new LPProblem(constraints, objective);
        }

        private Constraint? ParseConstraint()
        {
            var terms = new Dictionary<string, double>();

            // Parse left side
            while (CurrentToken.Type != TokenType.Semicolon && !IsAtEnd())
            {
                if (CurrentToken.Type == TokenType.Plus || CurrentToken.Type == TokenType.Minus)
                {
                    var sign = CurrentToken.Type == TokenType.Plus ? 1.0 : -1.0;
                    Advance();

                    if (CurrentToken.Type == TokenType.Number)
                    {
                        var coefficient = sign * double.Parse(CurrentToken.Value);
                        Advance();

                        if (CurrentToken.Type == TokenType.Identifier)
                        {
                            var variable = CurrentToken.Value;
                            Advance();
                            terms[variable] = terms.ContainsKey(variable) 
                                ? terms[variable] + coefficient 
                                : coefficient;
                        }
                    }
                }
                else if (CurrentToken.Type is TokenType.Less or TokenType.Greater or TokenType.Equal)
                {
                    break;
                }
                else
                {
                    Advance();
                }
            }

            // Parse operator
            var op = CurrentToken.Type switch
            {
                TokenType.Less => ConstraintType.Less,
                TokenType.Greater => ConstraintType.Greater,
                TokenType.Equal => ConstraintType.Equal,
                _ => ConstraintType.Equal
            };
            Advance();

            // Parse RHS
            var rhs = 0.0;
            if (CurrentToken.Type == TokenType.Number)
            {
                rhs = double.Parse(CurrentToken.Value);
                Advance();
            }

            if (CurrentToken.Type == TokenType.Semicolon)
            {
                Advance();
            }

            return new Constraint(terms, op, rhs);
        }

        private Token CurrentToken => _position < _tokens.Count ? _tokens[_position] : _tokens[^1];

        private void Advance() => _position++;

        private bool IsAtEnd() => CurrentToken.Type == TokenType.EOF;

        public List<string> Errors => _errors;
    }
}
