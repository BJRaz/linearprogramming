//----------------StringTokenizer.h

#ifndef STRINGTOKENIZER_H
#define STRINGTOKENIZER_H

#include "STLdef.h"

class StringTokenizer
{
public:
	StringTokenizer(const string &source, const string &delimiters = "\n");

	string NextToken();
	string NextToken(const string &delimiters);
	bool HasMoreTokens();
	int CountTokens();

protected:
	bool IsDelimiter(char tegn); // Privat funktion

	string tekst; // Den streng der skal opdeles i tokens
	string delim; // delimiters hvorefter strengen skal opdeles
	int indeks;	  // udpeger f�rste ubehandlede tegn i tekst
};

#endif