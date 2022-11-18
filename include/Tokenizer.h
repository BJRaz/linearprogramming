
#ifndef STRINGTOKENIZER_H
#define STRINGTOKENIZER_H

#include "STLdef.h"

class StringTokenizer
{
public:

//	StringTokenizer();
	StringTokenizer(const string& source, const string& delimiters = " \n");
	string NextToken();
	string NextToken(const string& delimiters);
	bool HasMoreTokens();
	int CountTokens();

protected:

	bool IsDelimiter(char tegn);
	string tekst;
	string delim;
	int indeks;
};

#endif

