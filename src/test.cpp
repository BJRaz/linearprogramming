//--------test.cpp

#include "LP.h"
#include "StraffeLP.h"
//#include "STLdef.h"
//#include "SymbolTabel.h"

using std::string;

void WriteBasisSolution(const vector<loesning> &loesVector);
void WriteUligheder(const vector<string> &ligninger);
void WriteMatrix(const LPMatrix LPM);

int main()
{
	// TestLPMatrix();
	// return 0; 
	
	LP lp;
	string LpProblem = ""; //"problems/bags.txt";
	cout << "Skriv filnavnet med lp-problemet: ";
	cin >> LpProblem;
	cout << endl;
	if (lp.Run(LpProblem))
	{
		cout << "\nlp.Run(" << LpProblem << ") lykkedes: " << endl;
		
		WriteMatrix(lp.GetMatrix());
		WriteUligheder(lp.GetUligheder());
		WriteBasisSolution(lp.GetBasisLoesning());
	}
	else
		cout << "\nlp.Run(" << LpProblem << ") lykkedes ikke: " << lp.GetFejlTekst() << endl;

	return 0;
}

void WriteBasisSolution(const vector<loesning> &loesVector)
{
	cout << endl << "Loesning paa optimeringsproblemet:" << endl;
	if (loesVector.empty())
	{
		cout << "\tIngen loesning tilgaengelig." << endl;
		return;
	}

	for (const auto &ls : loesVector)
	{
		// ls.first assumed to be a string-like name, ls.second a numeric value
		std::cout << std::left << std::setw(10) << ls.first
				  << " = " << std::right << std::setw(8)
				  << std::fixed << std::setprecision(2) << ls.second << std::endl;
	}
}

void WriteMatrix(const LPMatrix LPM)
{
	cout << "Den endelige LPMatrix:" << endl;
	cout << LPM;
}

void WriteUligheder(const vector<string> &ligninger)
{
	int antal = 0;
	cout << "LP-problemets uligheder: " << endl;
	if (!ligninger.empty())
	{
		antal = ligninger.size() - 1;
		for (size_t i = 0; i < antal; ++i)
			cout << "Ulighed nr. " << i + 1 << '\t' << ligninger[i] << endl;
	}
	else
		cout << "K-funktion:" << '\t' << ligninger[antal] << endl;
}
