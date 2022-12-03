/*
FILE	      : Matrix.h
Programm�r : Torben H�irup
Rettet     : 08/11 1999
Platform   : Visual C++ ver. 5.0

Template-klasse til dynamisk Matrix-klasse implementeret som
en vektor af vektorer ved brug af Standard Template Library
vector-klassen.

Jeg har valgt IKKE at angive: using namespace std; men bruger
scopet std:: for namespace.
Herved kan klassen anvendes i eksisterende 'gamle' programmer
og MFC-library programmer.

Operationer:
 - konstruktion:
   - default constructor til "tom" Matrix, 0 rows, 0 columns
   - Matrix med Row- og Column dimension
   - Matrix med dimension og default element-value
   - copy-constructor
 - nedl�ggelse: destructor
 - indexering med operator []:
    - reference:    value = mat[i][j] (RValue)
    - assignment:   mat[i][j] = value (LValue)
 - acces-metoderne (kan erstatte indexering):
    - GetAt(i, j)
    - SetAt(i, j, value)
 - assignment:  Matrix-X = Matrix-Y
    - dynamisk assignment, X antager samme v�rdier og dimensioner som Y
    - tidligere v�rdier i X frigives
 - Resize af matrix (b�de st�rre og mindre):
    - nye elementer f�r default v�rdien af elementerne (ikke standardtyper)
    - nye elementer f�r en angiven v�rdi
 - inspekt�rer:
   - antal Rows	    : NoOfRows()
   - antal Columns	  : NoOfCols()

Jeg pr�vede i lighed med Tim Budd's "Data Structures in C++ Using the STL"
at anvende attributten:
  vector< vector<T> > rows.
Men det g�r ikke VC++ 5.0, men fungerer i version 6.0!

Netop s. 434 i Budd's bog er utrolig fejlfyldt.
S�ledes is�r hans konstrukt�r. Copy-paste har vist spillet ham et puds!

Derfor har jeg lavet den simple class row, der blot wrapper et
vector<T> objekt (en r�kke i matrix). Id�en opstod som et skud i blinde!
Klassens indhold er public, s� ogs� arvinger af Matrix kan accesse
vector<T> objektet.
*/

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>  //STL-klasse
#include <cassert> //erstatter assert.h

using std::vector;

//-------------- class row er en r�kke i Matrix: -------------------

template <class T>
class row
{
public:
  // klassen indeholder netop �n vector = 1 r�kke i Matrix:
  vector<T> aRow;
};

//-------------------- class Matrix --------------------------------
template <class T>
class Matrix
{
public:
  Matrix(size_t Rows = 0, size_t Cols = 0);
  Matrix(size_t Rows, size_t Cols, const T &Value);
  Matrix(const Matrix<T> &source); // copy-constructor
  virtual ~Matrix(){};

  void Init(const T &Value);

  vector<T> &operator[](size_t index);             // LValue indexering
  const vector<T> &operator[](size_t index) const; // RValue indexering
  const T &GetAt(size_t Row, size_t Col) const;
  void SetAt(size_t Row, size_t Col, const T &Value);

  Matrix<T> &operator=(const Matrix<T> &m); // assignment

  virtual void Resize(size_t Rows, size_t Cols);
  virtual void Resize(size_t Rows, size_t Cols, const T &Value);

  // inspekt�rer:
  size_t NoOfRows() const;
  size_t NoOfCols() const;

protected:
  vector<row<T> > rows; // vector med rows vectorer
};                     // end of class Matrix

/* ------------------------ Kontrakter -----------------------------

 ---------------  Constructor, evt. uinitialiseret Matrix: ---------
Matrix (size_t Rows=0, size_t Cols=0)
 PRE : true
 POST: konstruerer Matrix med Rows r�kker og Cols s�jler
       Der kan konstrueres en matrix (0,0), som senere skal
       Resize's. Uden argumenter er det default constructor

 ---------------  Constructor, initialiseret Matrix: ----------
Matrix  (size_t Rows, size_t Cols, const T& Value)
 PRE : Value def.
 POST: konstruerer Matrix med Rows r�kker og Cols s�jler og alle
       elementer = Value. Value skal have en default-constructor

 ---------------  Copy Constructor: ---------------------------
Matrix  (const Matrix<T>& source)
 PRE : source def.
 POST: konstruerer kopi af Matrix source

 ---------------  Init: ---------------------------------------
void Init(const T& Value)
 PRE : true
 POST: Matrix initialiseret med Value

 ---------------  Operator []: --------------------------------
vector <T>& operator[] (size_t index)
 PRE : index < NoOfRows()
 POST: returnerer reference til r�kke-vektor udpeget af index, LValue

const vector <T>& operator[] (size_t index) const
 PRE : index < NoOfRows()
 POST: returnerer reference til r�kke-vektor udpeget af index, RValue

 ---------------  GetAt(..): --------------------------------
const T&   GetAt (size_t Row, size_t Col) const;
PRE : Row < NoOfRows(), 0 <= Col < NoOfCols()
POST: returnerer en const reference til elementet i [Row][Col], RValue

 ---------------  SetAt(..): --------------------------------
void SetAt (size_t Row, size_t Col, const T& Value);
PRE : Row < NoOfRows(), 0 <= Col < NoOfCols(), Value def.
POST: matrix elementet [Row][Col] = Value, LValue

 ---------------  Operator =: ---------------------------------
Matrix<T>& operator= (const Matrix<T>& m)
 PRE : right-operand m defineret
 POST: Matrix assigned v�rdien af m, returnerer Matrix = m

 -----------------  Resize(): ---------------------------------
void Resize(size_t Rows, size_t Cols);
 PRE : 0 < Rows, 0 < Cols
 POST: omdanner matrix til en Rows*Cols matrix. Nye elementer
       er initialiseret med default-v�rdi af elementtypen.

void Resize(size_t Rows, size_t Cols, const T& Value);
 PRE : 0 < Rows, 0 < Cols, Value def.
 POST: omdanner matrix til en Rows*Cols matrix. Nye elementer
       er initialiseret med Value.

 ---------------  NoOfRows (): ----------------------------
size_t NoOfRows ()
 PRE : true
 POST: returnerer antal r�kker i Matrix

 ---------------  NoOfCols (): ----------------------------
size_t NoOfCols ()
 PRE : true
 POST: returnerer antal s�jler i Matrix
 */

template <class T>
Matrix<T>::Matrix(size_t Rows, size_t Cols)
    : rows(Rows)
{
  for (size_t i = 0; i < Rows; i++)
    rows[i].aRow.resize(Cols);
}

template <class T>
Matrix<T>::Matrix(size_t Rows, size_t Cols, const T &Value)
    : rows(Rows)
{
  for (size_t i = 0; i < Rows; i++)
    rows[i].aRow.resize(Cols, Value);
}

template <class T>
Matrix<T>::Matrix(const Matrix<T> &source)
    : rows(source.NoOfRows())
{
  for (size_t i = 0; i < source.NoOfRows(); i++)
  {
    rows[i].aRow = source.rows[i].aRow; // vector assignment
  }
}

template <class T>
void Matrix<T>::Init(const T &Value)
{
  size_t i, j;
  for (i = 0; i < NoOfRows(); ++i)
    for (j = 0; j < NoOfCols(); ++j)
      rows[i].aRow[j] = Value;
}

template <class T>
vector<T> &Matrix<T>::operator[](size_t index) // LValue
{
  assert(0 <= index && index < NoOfRows());
  return rows[index].aRow;
  // aRow er en vector<T>
  // Ved indexering med [i][j] p� en matrix, vil [i]
  // returnere vector<T> i den i'te r�kke.
  // P� denne bruges [j], s�ledes at det j'te element i den i'te r�kke returneres.
  // Og det er netop element [i][j] i matrix'en!
}

template <class T>
const vector<T> &Matrix<T>::operator[](size_t index) const // RValue
{
  assert(index < NoOfRows());
  return rows[index].aRow;
}

template <class T>
const T &Matrix<T>::GetAt(size_t Row, size_t Col) const
{
  return rows[Row].aRow[Col];
}

template <class T>
void Matrix<T>::SetAt(size_t Row, size_t Col, const T &Value)
{
  rows[Row].aRow[Col] = Value;
}

template <class T>
Matrix<T> &Matrix<T>::operator=(const Matrix<T> &m)
{
  if (this != &m) // check for selv-assignment
  {
    rows.resize(m.NoOfRows()); // frigiv eller alloker memory

    // kopier r�kkerne fra m, en r�kke ad gangen:
    for (size_t i = 0; i < m.NoOfRows(); ++i)
      rows[i].aRow = m.rows[i].aRow; // vector assignment
  }
  return *this;
}

template <class T>
void Matrix<T>::Resize(size_t Rows, size_t Cols)
{
  rows.resize(Rows);
  for (size_t i = 0; i < Rows; ++i)
    rows[i].aRow.resize(Cols);
}

template <class T>
void Matrix<T>::Resize(size_t Rows, size_t Cols, const T &Value)
{
  rows.resize(Rows);
  for (size_t i = 0; i < Rows; ++i)
    rows[i].aRow.resize(Cols, Value);
}

template <class T>
size_t Matrix<T>::NoOfRows() const
{
  return rows.size();
}

template <class T>
size_t Matrix<T>::NoOfCols() const
{
  return rows[0].aRow.size();
}

#endif
