//--------test.cpp

#include "LP.h"
//#include "STLdef.h"
//#include "SymbolTabel.h"




void WriteBasisSolution(const vector<loesning>& loesVector);
void WriteUligheder(const vector<string>& ligninger);
void WriteMatrix(const LPMatrix LPM);


int main()
{
	LP lp;
	string LpProblem;
	cout << "Skriv filnavnet med lp-problemet: ";
	cin >> LpProblem;
	cout << endl;
	if(lp.Run(LpProblem))
	{
		cout << "Ingen fejl i lexikalsk analyse: ";
		/*
		WriteMatrix(lp.GetMatrix());
		WriteUligheder(lp.GetUligheder());
		WriteBasisSolution(lp.GetBasisLoesning());*/
	}
	else 
		cout << "\nlp.Run(" << LpProblem << ") lykkedes ikke: " << lp.GetFejlTekst() << endl;

	
	return 0;
}

void WriteBasisSolution(const vector<loesning>& loesVector)
{
	loesning ls;
	char cs[50], vs[50];
	cout << "\nLoesning paa optimeringsproblemet:\n";

	for(int i=0; i<loesVector.size(); ++i)
	{
		ls = loesVector[i];
		sprintf(vs, "%-10s", ls.first.data());
		sprintf(cs, "%8.2f", ls.second);
		cout << vs << " = " << cs << endl;
	}
}

void WriteMatrix(const LPMatrix LPM)
{
	cout << "Den endelige LPMatrix:\n";
	cout << LPM;
}

void WriteUligheder(const vector<string>& ligninger)
{
	int antal;
	cout << "LP-problemets uligheder:\n";
	if(!ligninger.empty())
	{
		antal = ligninger.size()-1;
		for(size_t i=0;i<antal;++i)
			cout << "Ulighed nr. " << i+1 << '\t' << ligninger[i] << endl;
	}
	else
		cout << "K-funktion:" << '\t' << ligninger[antal] << endl;
}

