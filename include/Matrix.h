     /*
     FILE	      : Matrix.h
     Programmør : Torben Høirup
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
      - nedlæggelse: destructor
      - indexering med operator []:
         - reference:    value = mat[i][j] (RValue)
         - assignment:   mat[i][j] = value (LValue)
      - acces-metoderne (kan erstatte indexering):
         - GetAt(i, j)
         - SetAt(i, j, value)
      - assignment:  Matrix-X = Matrix-Y
         - dynamisk assignment, X antager samme værdier og dimensioner som Y
         - tidligere værdier i X frigives
      - Resize af matrix (både større og mindre):
         - nye elementer får default værdien af elementerne (ikke standardtyper)
         - nye elementer får en angiven værdi
      - inspektører:
        - antal Rows	    : NoOfRows()
        - antal Columns	  : NoOfCols()

     Jeg prøvede i lighed med Tim Budd's "Data Structures in C++ Using the STL"
     at anvende attributten: 
       vector< vector<T> > rows. 
     Men det går ikke VC++ 5.0, men fungerer i version 6.0!

     Netop s. 434 i Budd's bog er utrolig fejlfyldt. 
     Således især hans konstruktør. Copy-paste har vist spillet ham et puds!

     Derfor har jeg lavet den simple class row, der blot wrapper et
     vector<T> objekt (en række i matrix). Idéen opstod som et skud i blinde!
     Klassens indhold er public, så også arvinger af Matrix kan accesse
     vector<T> objektet.
     */

     #ifndef MATRIX_H
     #define MATRIX_H

    #include <vector>  //STL-klasse
    #include <cassert> //erstatter assert.h

		using std::vector;

    //-------------- class row er en række i Matrix: -------------------

    template <class T>
    class row
    {
    public:            
     //klassen indeholder netop én vector = 1 række i Matrix:
     vector<T> aRow;
    };


    //-------------------- class Matrix --------------------------------
    template <class T>
    class Matrix
    {
    public:
     Matrix  (size_t Rows=0, size_t Cols=0); 
     Matrix  (size_t Rows, size_t Cols, const T& Value);  
     Matrix  (const Matrix<T>& source);       //copy-constructor
     virtual ~Matrix() {};

     void Init(const T& Value);

     vector<T>& operator[] (size_t index);       //LValue indexering
     const vector<T>& operator[] (size_t index) const; //RValue indexering
     const T& GetAt (size_t Row, size_t Col) const;
     void  SetAt (size_t Row, size_t Col, const T& Value);

     Matrix<T>& operator= (const Matrix<T>& m); //assignment

     virtual void Resize(size_t Rows, size_t Cols);  
     virtual void Resize(size_t Rows, size_t Cols, const T& Value);  

     //inspektører:
     size_t NoOfRows () const;
     size_t NoOfCols () const;

    protected:

     vector< row<T> > rows;  //vector med rows vectorer
    }; //end of class Matrix

    /* ------------------------ Kontrakter -----------------------------

     ---------------  Constructor, evt. uinitialiseret Matrix: ---------
    Matrix (size_t Rows=0, size_t Cols=0)
     PRE : true
     POST: konstruerer Matrix med Rows rækker og Cols søjler
           Der kan konstrueres en matrix (0,0), som senere skal
           Resize's. Uden argumenter er det default constructor          

     ---------------  Constructor, initialiseret Matrix: ----------
    Matrix  (size_t Rows, size_t Cols, const T& Value)
     PRE : Value def.
     POST: konstruerer Matrix med Rows rækker og Cols søjler og alle
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
     POST: returnerer reference til række-vektor udpeget af index, LValue

    const vector <T>& operator[] (size_t index) const
     PRE : index < NoOfRows()
     POST: returnerer reference til række-vektor udpeget af index, RValue

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
     POST: Matrix assigned værdien af m, returnerer Matrix = m

     -----------------  Resize(): ---------------------------------   
    void Resize(size_t Rows, size_t Cols);  
     PRE : 0 < Rows, 0 < Cols
     POST: omdanner matrix til en Rows*Cols matrix. Nye elementer
           er initialiseret med default-værdi af elementtypen.

    void Resize(size_t Rows, size_t Cols, const T& Value);  
     PRE : 0 < Rows, 0 < Cols, Value def.
     POST: omdanner matrix til en Rows*Cols matrix. Nye elementer
           er initialiseret med Value.

     ---------------  NoOfRows (): ----------------------------
    size_t NoOfRows ()
     PRE : true
     POST: returnerer antal rækker i Matrix

     ---------------  NoOfCols (): ----------------------------
    size_t NoOfCols ()
     PRE : true
     POST: returnerer antal søjler i Matrix
     */

   
   template <class T> 
   Matrix<T>::Matrix (size_t Rows, size_t Cols)
		            : rows(Rows)
   {
     for (size_t i = 0; i < Rows; i++)
	      rows[i].aRow.resize(Cols);
   }
 
   template <class T> 
   Matrix<T>::Matrix  (size_t Rows, size_t Cols, const T& Value)
		: rows(Rows)
   {
	    for (size_t i = 0; i < Rows; i++)
	      rows[i].aRow.resize(Cols, Value);
   }

   template <class T> 
   Matrix<T>::Matrix  (const Matrix<T>& source)
		: rows(source.NoOfRows() )
   {
	   for (size_t i = 0; i < source.NoOfRows(); i++)
	   {
	     rows[i].aRow = source.rows[i].aRow;   //vector assignment
	   }
   }
   
   template <class T> 
   void Matrix<T>::Init  (const T& Value)
   {
     size_t i, j;
     for (i=0; i < NoOfRows(); ++i)
       for (j=0; j < NoOfCols(); ++j)
         rows[i].aRow[j] = Value;
   }

   template <class T>
   vector<T>& Matrix<T>::operator[] (size_t index)  //LValue
   {
     assert (0 <= index && index < NoOfRows());
     return rows[index].aRow; 
     //aRow er en vector<T>
     //Ved indexering med [i][j] på en matrix, vil [i]
     //returnere vector<T> i den i'te række. 
     //På denne bruges [j], således at det j'te element i den i'te række returneres.
     //Og det er netop element [i][j] i matrix'en!
   }

   template <class T>
   const vector<T>& Matrix<T>::operator[] (size_t index) const //RValue
   {
     assert (index < NoOfRows());
     return rows[index].aRow;
   }

   template <class T>
   const T& Matrix<T>::GetAt (size_t Row, size_t Col) const
   {
     return rows[Row].aRow[Col];
   }

   template <class T>
   void Matrix<T>::SetAt (size_t Row, size_t Col, const T& Value)
   {
     rows[Row].aRow[Col] = Value;
   }
   
   template <class T>
   Matrix<T>& Matrix<T>::operator= (const Matrix<T>& m)
   {
     if (this != &m) //check for selv-assignment
     {
       rows.resize(m.NoOfRows()); //frigiv eller alloker memory

       //kopier rækkerne fra m, en række ad gangen:
	     for (size_t i = 0; i < m.NoOfRows(); ++i) 
         rows[i].aRow = m.rows[i].aRow; //vector assignment 
     }
     return *this;
   }
   
   template <class T>
   void Matrix<T>::Resize(size_t Rows, size_t Cols)
   {
     rows.resize(Rows);
     for (size_t i=0; i < Rows; ++i)
       rows[i].aRow.resize(Cols);
   }

   template <class T>
   void Matrix<T>::Resize(size_t Rows, size_t Cols, const T& Value)  
   {
     rows.resize(Rows);
     for (size_t i=0; i < Rows; ++i)
       rows[i].aRow.resize(Cols, Value);
   }

   template <class T>
   size_t Matrix<T>::NoOfRows () const
   {
     return rows.size();
   }

   template <class T>
   size_t Matrix<T>::NoOfCols () const
   {
     return rows[0].aRow.size();
   }

  #endif

	

