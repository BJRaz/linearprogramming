using System;
using System.Linq;

namespace LPSolver.Solver
{
    public class Matrix
    {
        private double[,] _data;

        public int Rows { get; }
        public int Columns { get; }

        public Matrix(int rows, int cols)
        {
            Rows = rows;
            Columns = cols;
            _data = new double[rows, cols];
        }

        public double this[int row, int col]
        {
            get => _data[row, col];
            set => _data[row, col] = value;
        }

        public void PrintTableau(string title = "Matrix")
        {
            Console.WriteLine($"\n{title}:");
            Console.WriteLine("Columns: " + string.Join(" ", Enumerable.Range(1, Columns).Select(i => $"{i,4}")));
            for (int i = 0; i < Rows; i++)
            {
                Console.Write($"Row {i + 1}:  ");
                for (int j = 0; j < Columns; j++)
                {
                    Console.Write($"{_data[i, j],8:F2}");
                }
                Console.WriteLine();
            }
        }
    }
}
