      /*
      FILE      : LPmatrix.h
      Programmør: Torben Høirup
      Retttet   : 08/11 1999

      Klassen LPMatrix er dynamisk og er beregnet til anvendelse i programmer 
      til Lineær programmering.
      Klassen arver fra den generelle (template-baserede) Matrix-klasse, 
      men er fast defineret til at indeholde double-values
      Herved kan dennes metoder til:
      - indexering [][]
      - Resize af matrix - større og mindre
      - kopiering (copy constructor)
      - matrix-assignment
      umiddelbart anvendes.

      LPMatrix konstruerer en matrix initialiseret med værdien 0.0 i alle 
      elementer.

      En LPMatrix skal oprettes og Resize's med: 
      antal rækker lig med:
        - antal ligninger (uligheder) + 1 (kriteriefunktionen) 
      antal søjler lig med:
        - antal variabler + antal restvariabler + antal straffevariabler
          + 1 (b-value, ligningens højreside)
      Ved konstruktion og Resize vil LPMatrix sikre en fri række 0 og
      en fri søjle 0, begge med værdien 0.0 indsat.
      Række 0 og søjle 0 anvendes af Simplex-algoritmen til forsk. information!

      Har man fx 4 ligninger med ialt 7 variabler + en b-value, skal
      et LPMatrix objekt instantieres således:
          LPMatrix A(4, 8);

      Skal den Resize's, fordi der er 2 variabler mere, ser Resize sådan ud:
          A.Resize(6, 8);
      
      
      To rækker kan ombyttes med SwapRows(row1, row2)

      Den indeholder desuden de ensbetydende (bi-implikative) metoder:
      - AddToRow, der adderer et multiplum af row2 til row1 
        (for at eliminere en variabel i row1)
      - ScaleRow, der multiplicerer en række med en faktor 
        (isolering af variabel, så den har koefficienten 1)

      Fordelen ved en dynamisk LPMatrix er, at man kan have en instans af
      klassen ved begyndelsen af oversættelsen. Dimensionerne på matrix'en
      kan ændres, når LP-problemets omfang kendes, således at man i det
      følgende pass i oversættelsen kan indsætte koefficienterne til variablerne
      i en korrekt dimensioneret LPMatrix efter en Resize-operation.

      size_t er typedefineret i Matrix.h og er synonym med unsigned int
      */


      #ifndef LPMATRIX_H
      #define LPMATRIX_H

      #include "matrix.h"
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

          //Indlæsning fra tekstfil af ligningssystem:
          void ReadFromFile (const string& filnavn); 
      };

      //Forward erklæring af fri udskrivnings-funktion:
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
          en variabel heri, så den får koefficienten 1
            row(r) = c * row(r);  c != 0

        void ReadFromFile (const char* filnavn);  
          indlæser en LPMatrix fra en tekstfil med navn = filnavn.
          Der indlæses ikke data i række 0 og søjle 0

        fri funktion:
        ostream& operator << (ostream& os, LPMatrix& LPM);
          udskriver LPMatrix fra row 1 og col 1 på standard output.
          Anvendes til test.
      */

      #endif
