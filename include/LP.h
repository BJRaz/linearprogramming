//------------LP.h
// Af Brian Juul Rasmussen
//

#ifndef LP_H
#define LP_H

#include "STLdef.h"
#include "LPMatrix.h"
#include "StandardLP.h"
#include "SymbolTabel.h"
#include "Token.h"

using std::pair;

typedef pair<string, double> loesning;

class LP
{
public:
	LP();
	~LP();

	bool Run(const string &filnavn, bool trace = false);
	string GetFejlTekst();
	const vector<string> &GetUligheder();
	const LPMatrix &GetMatrix();
	const vector<loesning> &GetBasisLoesning();

protected:
	bool Scan();
	void SetFejl(const string &errStr, bool medLinieNr);
	char NextChar();

	bool InError();
	bool ParseAndGenerateCode();

	void NextToken();

	void LpUlighedsliste();
	void Ulighed();
	void Termliste();
	void HjTermliste();
	void Term();
	void AddOp();
	void RelOp();
	void Num();
	void Identifier();

	void DoID();
	void DoNUM();
	void DoLESS();
	void DoGREATER();
	void DoEQUAL();
	void DoPLUS();
	void DoMINUS();
	void DoSEMICOLON();

	// Attributter

	StandardLP *pSLP;
	LPMatrix A;

	SymbolTabel Symboltabel;
	queue<Token> TokenQueue;
	vector<string> Uligheder;

	Token CurrentToken;
	Token LastToken;

	bool bOKState;
	bool itis_b_Now;

	string FejlTekst;
	size_t m;
	size_t n;

	size_t ulighedNr;
	size_t restIdx;
	size_t kunstIdx;

	vector<loesning> Loesning;

	string filNavn; // navn p� tekst-fil, der indeholder ulighederne

	ifstream TekstFil; // input-fil objektet
};

#endif
