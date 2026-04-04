using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using LPSolver.Solver;

namespace LPSolver
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("╔════════════════════════════════════════════════╗");
            Console.WriteLine("║  Linear Programming Solver - C# .NET (POC)     ║");
            Console.WriteLine("╚════════════════════════════════════════════════╝\n");

            string filepath;

            if (args.Length > 0)
            {
                filepath = args[0];
            }
            else
            {
                Console.Write("Enter problem file path: ");
                filepath = Console.ReadLine() ?? "";
            }

            if (string.IsNullOrWhiteSpace(filepath))
            {
                Console.WriteLine("Error: No file path provided.");
                return;
            }

            // Resolve relative paths from project root
            if (!Path.IsPathRooted(filepath))
            {
                // Try relative path from current directory
                if (!File.Exists(filepath))
                {
                    // Try relative to project root
                    var projectRoot = FindProjectRoot();
                    var resolvedPath = Path.Combine(projectRoot, filepath);
                    if (File.Exists(resolvedPath))
                    {
                        filepath = resolvedPath;
                    }
                }
            }

            if (!File.Exists(filepath))
            {
                Console.WriteLine($"Error: File '{filepath}' not found.");
                return;
            }

            try
            {
                var content = File.ReadAllText(filepath);

                // Lexical analysis
                Console.WriteLine("▶ Lexical Analysis");
                var lexer = new Lexer(content);
                var tokens = lexer.Tokenize();

                if (lexer.Errors.Count == 0)
                {
                    Console.WriteLine("  ✓ No errors");
                }
                else
                {
                    Console.WriteLine("  ✗ Errors found:");
                    foreach (var error in lexer.Errors)
                        Console.WriteLine($"    {error}");
                    return;
                }

                // Parsing
                Console.WriteLine("\n▶ Syntax Analysis");
                var parser = new Parser(tokens);
                var problem = parser.Parse();

                if (parser.Errors.Count == 0)
                {
                    Console.WriteLine("  ✓ No errors");
                }
                else
                {
                    Console.WriteLine("  ✗ Errors found:");
                    foreach (var error in parser.Errors)
                        Console.WriteLine($"    {error}");
                    return;
                }

                // Display problem
                Console.WriteLine("\n▶ Problem Definition");
                Console.WriteLine($"  Constraints: {problem.Constraints.Count}");
                if (problem.ObjectiveFunction != null)
                {
                    Console.WriteLine("  Objective function found");
                }

                // Solving
                Console.WriteLine("\n▶ Solving with Simplex Algorithm");
                var solver = new SimplexSolver(problem);
                var result = solver.Solve();

                Console.WriteLine();
                if (result.IsOptimal)
                {
                    Console.WriteLine("  ✓ Optimal solution found\n");
                    Console.WriteLine("  Solution:");
                    foreach (var kvp in result.Solution.OrderBy(x => x.Key))
                    {
                        Console.WriteLine($"    {kvp.Key,15} = {kvp.Value,10:F2}");
                    }
                    Console.WriteLine($"\n  Objective Value: {result.ObjectiveValue:F2}\n");
                }
                else
                {
                    Console.WriteLine($"  ✗ Failed: {result.Message}\n");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
            }
        }

        static string FindProjectRoot()
        {
            // Walk up directory tree to find csharp-dotnet directory
            var current = Directory.GetCurrentDirectory();
            while (current != Path.GetPathRoot(current))
            {
                if (Directory.Exists(Path.Combine(current, "src")) &&
                    Directory.Exists(Path.Combine(current, "problems")))
                {
                    return current;
                }
                current = Path.GetDirectoryName(current) ?? current;
            }
            return Directory.GetCurrentDirectory();
        }
    }
}
