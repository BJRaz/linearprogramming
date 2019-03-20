//--------Token.h

#ifndef TOKEN_H
#define TOKEN_H
#include "STLdef.h"

typedef enum {BEGIN, ID, NUM, PLUS = '+', MINUS = '-', LESS = '<',GREATER = '>', EQUAL = '=', SEMICOLON = ';', END} 
			Tokentype;

class Token
{
public:
	Token();
	
	void MakeToken(const string& streng);
	//string GetFejlTekst();
public:	//Repræsentationen gøres public for direkte access
	Tokentype tType;	//Tokens type
	string theToken;	//tokenstrengen fra scan()
	string strValue;	//tokensværdi hvis det er en streng dvs id
	double numValue;	//Tokens værdi	hvis der er en numerisk værdi, dvs num
	//string FejlTekst;	//Indeholder evt en fejltekst
	
};

#endif