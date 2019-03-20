      /*
      FILE      : LPmatrix.h
      Programm�r: Torben H�irup
      Retttet   : 08/11 1999

      Klassen LPMatrix er dynamisk og er beregnet til anvendelse i programmer 
      til Line�r programmering.
      Klassen arver fra den generelle (template-baserede) Matrix-klasse, 
      men er fast defineret til at indeholde double-values
      Herved kan dennes metoder til:
      - indexering [][]
      - Resize af matrix - st�rre og mindre
      - kopiering (copy constructor)
      - matrix-assignment
      umiddelbart anvendes.

      LPMatrix konstruerer en matrix initialiseret med v�rdien 0.0 i alle 
      elementer.

      En LPMatrix skal oprettes og Resize's med: 
      antal r�kker lig med:
        - antal ligninger (uligheder) + 1 (kriteriefunktionen) 
      antal s�jler lig med:
        - antal variabler + antal restvariabler + antal straffevariabler
          + 1 (b-value, ligningens h�jreside)
      Ved konstruktion og Resize vil LPMatrix sikre en fri r�kke 0 og
      en fri s�jle 0, begge med v�rdien 0.0 indsat.
      R�kke 0 og s�jle 0 anvendes af Simplex-algoritmen til forsk. information!

      Har man fx 4 ligninger med ialt 7 variabler + en b-value, skal
      et LPMatrix objekt instantieres s�ledes:
          LPMatrix A(4, 8);

      Skal den Resize's, fordi der er 2 variabler mere, ser Resize s�dan ud:
          A.Resize(6, 8);
      
      
      To r�kker kan ombyttes med SwapRows(row1, row2)

      Den indeholder desuden de ensbetydende (bi-implikative) metoder:
      - AddToRow, der adderer et multiplum af row2 til row1 
        (for at eliminere en variabel i row1)
      - ScaleRow, der multiplicerer en r�kke med en faktor 
        (isolering af variabel, s� den har koefficienten 1)

      Fordelen ved en dynamisk LPMatrix er, at man kan have en instans af
      klassen ved begyndelsen af overs�ttelsen. Dimensionerne p� matrix'en
      kan �ndres, n�r LP-problemets omfang kendes, s�ledes at man i det
      f�lgende pass i overs�ttelsen kan inds�tte koefficienterne til variablerne
      i en korrekt dimensioneret LPMatrix efter en Resize-operation.

      size_t er typedefineret i Matrix.h og er synonym med unsigned int
      */


      #ifndef LPMATRIX_H
      #define LPMATRIX_H

      #include "Matrix.h"
      #include "STLdef.h"

      class LPMatrix: public Matrix<double>
      {
        public:
          LPMatrix(size_t LignAndKrit=0, size_t VarsAndB=0);
          ~LPMatrix() {};

          //redefinition af Resize:
          void Resize(size_t LignAndKrit, size_t VarsAndB);  
          void Resize(size_t LignAndKrit, size_t VarsAndB, const double Value) {/*tom*/}; 
          
          //bi-implikative lignings operationer:
          void SwapRows (size_t row1, size_t row2);
          void AddToRow (size_t row1, size_t row2, double c);
          void ScaleRow (size_t row, double c);

          //Indl�sning fra tekstfil af ligningssystem:
          void ReadFromFile (const string& filnavn); 
      };

      //Forward erkl�ring af fri udskrivnings-funktion:
      ostream& operator << (ostream& os, const LPMatrix& LPM);

      /*
        LPMatrix(size_t LignAndKrit=0, size_t VarsAndB=0);
          Konstruerer en (LignAndKrit+1) * (VarsAndB+1) LPMatrix 
          initialiseret med 0.0 i alle elementer

        void Resize(size_t LignAndKrit, size_t VarsAndB);  
          resizer LPMatrix til en (LignAndKrit+1) * (VarsAndB+1)

        void SwapRows (size_t row1, size_t row2);
          Ombytter to ligninger:
          row(row1') = row(row2) OG row(row2') = row(row1)

        void AddToRow (size_t row1, size_t row2, double c);
          Adderer et multiplum af ligningen i row2 til ligningen i row1. 
          Anvendes til eliminering af en variabel i ligningen i row1.
          row(row1) = row(row1) + c * row(row2);  c != 0

        void ScaleRow (size_t row, double c);
          multiplicerer ligningen i row(row) med konstanten c for at isolere 
          en variabel heri, s� den f�r koefficienten 1
            row(r) = c * row(r);  c != 0

        void ReadFromFile (const char* filnavn);  
          indl�ser en LPMatrix fra en tekstfil med navn = filnavn.
          Der indl�ses ikke data i r�kke 0 og s�jle 0

        fri funktion:
        ostream& operator << (ostream& os, LPMatrix& LPM);
          udskriver LPMatrix fra row 1 og col 1 p� standard output.
          Anvendes til test.
      */

      #endif
