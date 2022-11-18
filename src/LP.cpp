//------------LP.cpp
#include "LP.h"
#include "StraffeLP.h"

LP::LP()
{
	ulighedNr = 0;
	
	pSLP	= NULL;
	bOKState = false;
}

LP::~LP()
{
	if(pSLP != NULL)
	{
		delete pSLP;
		pSLP = NULL;	//For at undg� dangling reference
	}
}

bool LP::Run(const string& filnavn, bool trace)
{
	filNavn = filnavn;
	Uligheder.clear();
	bOKState = true;
	FejlTekst = "";

	if(!Scan())
		return false;
	else 
		cout << "\nSCAN LYKKEDES: \n";
	
	A.Resize(m+1, n+1);	//A s�ttes til rigtig size f�r kodegenerering
	
	if(!ParseAndGenerateCode())
	{
		cout << "\nParse fejl: \n";
		return false;
	}

	cout << "HER: " << A << endl;
	pSLP = new StraffeLP(A,trace);
	return (pSLP->SolveLP());

	return bOKState;
}

const LPMatrix& LP::GetMatrix()
{
	return A;
}

const char ETX = '\3';	//Grim variabel for class LP


char LP::NextChar()
{
	char ch;
	//hvis filen ikke er EOF l�ses et ReadAheadChar
	if(TekstFil.get(ch))
		return ch;
	//EOF returner ETX = ASCII '\3'
	return ETX;
}

bool LP::Scan()
{

	TekstFil.open(filNavn.data());

	if(!TekstFil.is_open())
	{
		SetFejl("Filen kan ikke aabnes", false);
		return bOKState;
	}
	
	size_t nSymbols = 0;
	size_t nUnequalities = 0;
	size_t nGreater = 0;
	
	string streng;
	
	string whitespace(" \n\t");
	const int npos = -1; //Markerer no position found
	
	char ReadAheadChar = NextChar();

	
	ulighedNr=1;
	Uligheder.push_back(" ");

	while(ReadAheadChar!=ETX)
	{
	if(whitespace.find(ReadAheadChar) != npos)
		ReadAheadChar = NextChar();
	else
	{
		switch(ReadAheadChar)
			{
			case ';': streng = ReadAheadChar;
				Uligheder[ulighedNr-1] += streng;
				++ulighedNr;
				Uligheder.push_back("");
				ReadAheadChar = NextChar();
				break; //case...

			case '+': streng = ReadAheadChar;
				Uligheder[ulighedNr-1] += streng + ' ';
					ReadAheadChar = NextChar();
					break; //case...
			case '-': streng = ReadAheadChar;
				Uligheder[ulighedNr-1] += streng + ' ';
				ReadAheadChar = NextChar();
				break; //case...
			case '<': streng = ReadAheadChar;
				Uligheder[ulighedNr-1] += streng + ' ';
				//++nGreater;
				++nUnequalities;
				ReadAheadChar = NextChar();
				break;		//case...
			case '>': streng = ReadAheadChar;
				Uligheder[ulighedNr-1] += streng + ' ';
				++nUnequalities;
				++nGreater;
				ReadAheadChar = NextChar();
				break;		//case ...
			case '=': streng = ReadAheadChar;
				Uligheder[ulighedNr-1] += streng + ' ';
				ReadAheadChar = NextChar();
				break;
			case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
				streng = ReadAheadChar;
				Uligheder[ulighedNr-1] += ReadAheadChar;
				ReadAheadChar = NextChar();

				while(ReadAheadChar!=ETX)
				if(isdigit(ReadAheadChar))
				{
					streng += ReadAheadChar;
					Uligheder[ulighedNr-1] += ReadAheadChar;
					ReadAheadChar = NextChar();
				}
				else 
					break; //while...

				if(ReadAheadChar == '.')
				{
					streng += ReadAheadChar;
					Uligheder[ulighedNr-1] += ReadAheadChar;
					ReadAheadChar = NextChar();

					if(!isdigit(ReadAheadChar))//check tegn efter '.'
					{
						Uligheder[ulighedNr-1] += ReadAheadChar;	//inds�t fejltegn
						SetFejl("'.' skal efterfoelges af ciffer!",true);
						return false;
					}
					else
					while(ReadAheadChar!=ETX)
						if(isdigit(ReadAheadChar))
						{
							streng += ReadAheadChar;
							Uligheder[ulighedNr-1] += ReadAheadChar;
							ReadAheadChar = NextChar();
						}
					    else
							break;		//while
				}//if (ReadAheadChar..
				
				Uligheder[ulighedNr-1] += ' ';	//Space efter NUM
				break;	//case

			default: 
				if (isalpha(ReadAheadChar))
				{
					streng = ReadAheadChar;
					Uligheder[ulighedNr-1] += ReadAheadChar;
					ReadAheadChar = NextChar();
					while(ReadAheadChar!=ETX)
					if(isalpha(ReadAheadChar) || isdigit(ReadAheadChar))
					{
						streng += ReadAheadChar;
						Uligheder[ulighedNr-1] += ReadAheadChar;
						ReadAheadChar = NextChar();
					}
				    else
						break;
					
				    Uligheder[ulighedNr-1] += ' ';
					//Inds�t ID i symboltabel
					Symboltabel.InsertID(streng);
				}
				else
				{
					Uligheder[ulighedNr-1] += ReadAheadChar;
					SetFejl("Ulovligt tegn!",true);
					return false;
				}
		}	//switch (ReadAheadChar)
		//lav det token der er indeholdt i streng
		CurrentToken.MakeToken(streng);
		//S�t token i k�en
		TokenQueue.push(CurrentToken);
	}//if/whitespace.find.....
	
	}//while ReadAheadChar != ETX
	
	TekstFil.close();

	if(Uligheder.size() > 0)
		Uligheder.resize(Uligheder.size()-1);

	nSymbols = Symboltabel.GetNoOfVars();
//--------------test udskrift --------------------
	//cout << "Antal variabler i Symboltabel: " << nSymbols;

	m = nUnequalities;
	n = nSymbols + nUnequalities + nGreater;
	//--------------test udskrift --------------------
	//cout << "\nAntal uligheder(m): " << m;
	//cout << "\nAntal symboler(n): " << n;
	//cout << "\n";
	


	restIdx = nSymbols + 1;

	kunstIdx = n - nGreater + 1;
	//--------------test udskrift --------------------
	//cout << "\nrestIdx: " << restIdx;
	//cout << "\nkunstIdx: " << kunstIdx;
	//cout << "\n";

	return true;
}

void LP::SetFejl(const string& errStr, bool medLinieNr)
{
	bOKState = false;


	if(medLinieNr && Uligheder.size() >= ulighedNr)
	{
		char format[100];
		sprintf(format," i ulighed nr. %lu ", ulighedNr);
		FejlTekst = errStr + format + ":\n" + Uligheder[ulighedNr-1];
	}
	else 
		FejlTekst = errStr;
}

string LP::GetFejlTekst()
{
	return FejlTekst;
}

void LP::LpUlighedsliste()
{
	
	// original fra noter !! (BJR)
	ulighedNr = 1;
	while(!TokenQueue.empty() && !InError())
	{
		NextToken();
		if(!InError())
			Ulighed();
		++ulighedNr;
	}
}

void LP::Ulighed()
{
	cout << " - LP::Ulighed() koeres !!\n";	//TEST udskrift
	Termliste();
	if(!InError())
		RelOp();
	if(!InError())
		Num();
}

void LP::Termliste()
{
	cout << " - Termliste() koeres !!\n";	//TEST udskrift 
	Term();
	if(!InError())
		HjTermliste();

}

void LP::HjTermliste()
{
	cout << "LP::HjTermliste koeres!!!\n";	//TEST udskrift
	if(CurrentToken.tType == PLUS || CurrentToken.tType == MINUS)
		Termliste();
	else
	{
		//epsilon .. g�r intet
	}
	//else
	//SetFejl...
	//
	//
}

void LP::Term()
{
	AddOp();
	if(!InError())
		Num();
	if(!InError())
		Identifier();
}

void LP::AddOp()
{
	cout << " - LP::AddOP koeres !! \n";	//TEST udskrift
	cout << "	-	CurrentToken type: (" << CurrentToken.theToken << ")\n";
		cout << "	-	LastToken type: (" << LastToken.theToken << ")\n";
	if(CurrentToken.tType == PLUS)
		DoPLUS();
	else if(CurrentToken.tType == MINUS)
		DoMINUS();
	else
		SetFejl("'+' eller '-' forventet ", true);
}

void LP::RelOp()
{
	if(CurrentToken.tType == LESS)
		DoLESS();
	else if(CurrentToken.tType == GREATER)
		DoGREATER();
	else if (CurrentToken.tType == EQUAL)
		DoEQUAL();
	else
		SetFejl("'<', '>' eller '=' forventet ) ",true);
}

void LP::Num()
{
	if(CurrentToken.tType == NUM)
	{
		DoNUM();
	}
	else 
		SetFejl("Numerisk value forventet",true);

}

void LP::Identifier()
{
	if(CurrentToken.tType == ID)
	{
		DoID();
	}
	else 
		SetFejl("ID(bogstav) value forventet",true);
}

void LP::DoID()
{
	size_t j;
	//finder strValue i Symboltabel, ret. Index
	if(Symboltabel.Find(CurrentToken.strValue, j))
	{
		A[ulighedNr][j] = LastToken.numValue;
		NextToken();
	}
	else
		SetFejl("Syntaxfejl. Ukendt variabel", true);
}

void LP::DoNUM()
{
	if(LastToken.tType == MINUS) //venstre side, minus foran koefficient
		CurrentToken.numValue = - CurrentToken.numValue;
	//ved fortegn PLUS foretages intet
	if(itis_b_Now) //tallet p� h�jre side
	{
		A[ulighedNr][n+1] = CurrentToken.numValue;
		itis_b_Now = false;
	}
	NextToken();
		cout << " DoNUM k�rt !! \n";

}

void LP::DoLESS()
{
		cout << " DoLESS k�rt !! \n";

	A[ulighedNr][restIdx] = 1;
	restIdx++;
	itis_b_Now = true;
	NextToken();
}

void LP::DoGREATER()
{
	cout << " DoGREATER k�rt !! \n";
	A[ulighedNr][restIdx] = -1.0;
	restIdx++;
	A[ulighedNr][kunstIdx] = 1.0;
	kunstIdx++;
	A[ulighedNr][0] = 1.0; //marker kunstig variabel i s�jle 0
	itis_b_Now = true;
	NextToken();
}

void LP::DoEQUAL()
{
	cout << " DoEQUAL koeres !! \n";
	itis_b_Now = true;
	NextToken();
}

void LP::DoPLUS()
{
	cout << " DoPLUS koeres !! \n";
	NextToken();
}

void LP::DoMINUS()
{
cout << " DoMINUS koeres !! \n";
	NextToken();
}

void LP::DoSEMICOLON()
{
	cout << "\n DoSEMICOLON koeres !! \n";
	++ulighedNr;
	NextToken();
}

bool LP::InError()
{
	return bOKState == false;
}

void LP::NextToken()
{
	if(TokenQueue.empty())
		CurrentToken.tType = END;
	else
	{
		LastToken = CurrentToken;
		CurrentToken = TokenQueue.front();
		TokenQueue.pop();
	}
}

bool LP::ParseAndGenerateCode()
{
	// ulighedNr = 1; // indeks til f�rste r�kke i LPMatrix-objektet
	// NextToken();	//symbol-lookahead
	// if(CurrentToken.tType == END)
	// 	SetFejl("Der er ingen symboler der kan parses",false);   //minus linienummer
	// if(!InError())
		LpUlighedsliste();
	return bOKState;
}

const vector<string>& LP::GetUligheder()
{
	return Uligheder;
}


const vector<loesning>& LP::GetBasisLoesning()
{
	Loesning.clear();
	loesning enVariabel;
	size_t antalVar = Symboltabel.GetNoOfVars();

	for(size_t varnr = 1; varnr <= antalVar; ++varnr)
	{
		enVariabel.first = Symboltabel.GetID(varnr);
		if(A[0][varnr] == 0)
			enVariabel.second = 0.0;
		else
		{
			size_t row = 1;
			for(; A[row][varnr] != 1.0; ++row);
			enVariabel.second = A[row][n+1];
		}
		Loesning.push_back(enVariabel);
	}
	enVariabel.first = "K";
	enVariabel.second = -1*A[m+1][n+1];
	Loesning.push_back(enVariabel);

	return Loesning;
}
