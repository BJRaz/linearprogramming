/* File			 : lpmatrix.cpp
         Programm�r: ToH�
         Rettet		 : 08/11 1999
      */

#include "LPMatrix.h"
#include <iomanip>
#include <sstream>
#include <cmath>

LPMatrix::LPMatrix(size_t LignAndKrit, size_t VarsAndB)
    : Matrix<double>(LignAndKrit + 1, VarsAndB + 1, 0.0)
{
}

void LPMatrix::Resize(size_t LignAndKrit, size_t VarsAndB)
{
  size_t r = LignAndKrit + 1;
  size_t c = VarsAndB + 1;
  rows.resize(r);
  for (size_t i = 0; i < r; ++i)
    rows[i].aRow.resize(c, 0.0); // nye elementer 0.0
}

void LPMatrix::SwapRows(size_t row1, size_t row2)
{
  // vector har swap som member-funktion (se i STL!)
  // Matrix operator [] returnerer en vector, derfor:
  assert((1 <= row1 && row1 < NoOfRows()) && (1 <= row2 && row2 < NoOfRows()));

  rows[row1].aRow.swap(rows[row2].aRow);
}

void LPMatrix::AddToRow(size_t row1, size_t row2, double c)
// row(row1) = row(row1) + c * row(row2);
{
  assert((1 <= row1 && row1 < NoOfRows()) && (1 <= row2 && row2 < NoOfRows()));
  for (size_t col = 1; col < NoOfCols(); ++col)
    rows[row1].aRow[col] += c * rows[row2].aRow[col];
}

void LPMatrix::ScaleRow(size_t row, double c)
// row(row) = c * row(row);
{
  assert(1 <= row && row < NoOfRows());
  for (size_t col = 1; col < NoOfCols(); ++col)
    rows[row].aRow[col] = c * rows[row].aRow[col];
}

void LPMatrix::ReadFromFile(const string &filnavn)
{
  /*
        Til test.
        L�ser f�rst antal ligninger (minus K-max): m
        herefter antal variabler                 : n
        Matrixen resize's herefter til en (m+1) * (n+1) matrix.
        Efter resize indl�ses koeffecienter fra ligningerne p�
        kanonisk form, fx.:
        30 10 1 0 0 2800
         1  2 0 1 0  210
         1  5 0 0 1  450
         1  3 0 0 0    0  (Kriteriefunktionen til sidst)
        */
  ifstream ifs(filnavn.data());

  size_t m, n;
  ifs >> m >> n;

  Resize(m + 1, n + 1);

  size_t row, col;

  for (row = 1; !ifs.eof() && row < NoOfRows(); ++row)
    for (col = 1; !ifs.eof() && col < NoOfCols(); ++col)
      ifs >> operator[](row)[col];
  ifs.close();
}

// fri funktion til udskrivning: (pænere, kolonnejusteret)
ostream &operator<<(ostream &os, const LPMatrix &LPM)
{
  using std::setw;
  using std::right;
  using std::left;
  using std::fixed;
  using std::setprecision;

  const int colWidth = 10; // width used for each column
  const int labelWidth = 10;

  size_t row, col;
  os << "\nColumn:";
  for (col = 1; col < LPM.NoOfCols(); ++col)
  {
    os << setw(colWidth) << right << col;
  }
  os << std::endl;

  // underline
  os << setw(labelWidth) << "";
  for (col = 1; col < LPM.NoOfCols(); ++col)
    os << std::string(colWidth, '-');
  os << std::endl;

  for (row = 1; row < LPM.NoOfRows(); ++row)
  {
    std::ostringstream rowLabel;
    rowLabel << "Row " << row << ":";
    os << left << setw(labelWidth) << rowLabel.str();

    for (col = 1; col < LPM.NoOfCols(); ++col)
    {
      double val = LPM.GetAt(row, col);

      // Print integers without decimal point when possible
      if (std::fabs(val - std::round(val)) < 1e-9)
      {
        os << right << setw(colWidth) << static_cast<long>(std::lround(val));
      }
      else
      {
        os << right << setw(colWidth) << fixed << setprecision(2) << val;
        os.unsetf(std::ios::fixed);
      }
    }
    os << std::endl;
  }
  return os;
}
