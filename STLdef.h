      /*
      File        : STLdef.h
      Programmør  : ToHø
      Rettet      : 08/11 1999

      Beskrivelse :
      Filen er beregnet til include af diverse STL-filer til LP
      med explicite deklarationer af anvendte STL-komponenter.
      Oversættelsesmæssigt kan det nok ikke betale sig, men ved
      udviklingen af LP-klasserne gør det livet lidt lettere.

      Anvendes ved blot at inkludere denne fil i en klasses headerfil, 
      hvor STL bruges i klassen (også cpp-filen).
      */

      //undertryk warning C4786 og 4788!!
      #pragma warning (disable: 4786 4788) 

      #include <vector>
      #include <string>
      #include <queue>
      #include <cassert>  //erstatter assert.h
      #include <fstream>  //erstatter fstream.h
      #include <iostream> //erstatter iostream.h
      #include <cmath>    //erstatter math.h

      using std::string;
      using std::vector;
      using std::queue;
      using std::ostream;
      using std::ifstream;
      using std::cout;
      using std::cin;
      using std::endl;
      using std::flush;

