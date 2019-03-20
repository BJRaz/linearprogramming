//-------Token.cpp

#include "Token.h"

Token::Token()
{
	tType = BEGIN;	//Bruges ikke, initializerer blot en veldefineret værdi
	numValue = 0.0;
	//string - attributterne bliver default tomme
}

void Token::MakeToken(const string& streng)
{
	theToken = streng;	//strengværdien

	switch(streng[0])
	{
	case '<': tType = LESS;	
		break;
	case '>': tType = GREATER;	
		break;
	case '=': tType = EQUAL; 
		break;
	case ';': tType = SEMICOLON; 
		break;
	case '+': tType = PLUS; 
		break;
	case '-': tType = MINUS; 
		break;
	case '0':case '1':case '2':case'3':case '4':case '5':case '6':case '7':case '8':
	case '9': 
		numValue = atof(streng.data());	//alphnumeric to float
		tType = NUM;
		 break;
	default:  
		strValue = streng;
		tType = ID;
		break;
	}
}

