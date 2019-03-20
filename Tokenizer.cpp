
#include "Tokenizer.h"


StringTokenizer::StringTokenizer(const string& source, const string& delimiters)
				: tekst(source), delim(delimiters)
{
	indeks=0;
}

string StringTokenizer::NextToken()
{
	string token;
	while(indeks < tekst.size() && IsDelimiter(tekst[indeks]))
		++indeks;
	
		while(indeks < tekst.size() && ! IsDelimiter(tekst[indeks]))
		{
			token = token + tekst[indeks];
			++indeks;
		}

  return token;
}


string StringTokenizer::NextToken(const string& delimiters)
{
	delim = delimiters;
		return NextToken();
}

bool StringTokenizer::HasMoreTokens()
{
	for(int i= indeks;i< tekst.size();++i)
	
		if(!IsDelimiter(tekst[i]))
			return true;

	return false;
}

int StringTokenizer::CountTokens()
{
	int antal=0;
	int ANTAL = indeks;
	while (NextToken() != "")
		++antal;

	indeks = ANTAL;
	return antal;
}

bool StringTokenizer::IsDelimiter(char tegn)
{
	for(int i=0;i < delim.size();++i)
	
		if(tegn==delim[i])
			return true;
		return false;
}

