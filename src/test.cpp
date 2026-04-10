//--------test.cpp

#include "LP.h"
#include "StraffeLP.h"
//#include "STLdef.h"
//#include "SymbolTabel.h"
#include <iomanip>
#include <sstream>

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
	if (lp.Run(LpProblem, true))  // trace=true to enable simplex pivot visualization
	{
		cout << "lp.Run(" << LpProblem << ") lykkedes: " << endl;
		
		WriteMatrix(lp.GetMatrix());
		WriteUligheder(lp.GetUligheder());
		WriteBasisSolution(lp.GetBasisLoesning());
	}
	else
		cout << "\nlp.Run(" << LpProblem << ") lykkedes ikke: " << lp.GetFejlTekst() << endl;

	return 0;
}

// -----------------------------------------------------------------------------
// Output helpers
// These functions format and print:
// 1) Basis solution values
// 2) Final simplex matrix
// 3) Parsed inequalities and objective function (K-function)
// -----------------------------------------------------------------------------
void WriteBasisSolution(const vector<loesning> &loesVector)
{
	cout << endl << "Loesning paa optimeringsproblemet:" << endl;
	if (loesVector.empty())
	{
		cout << "\tIngen loesning tilgaengelig." << endl;
		return;
	}

	// Compute column widths based on data
	size_t maxName = 0;
	size_t maxVal = 0;

	for (const auto &ls : loesVector)
	{
		if (ls.first.size() > maxName)
			maxName = ls.first.size();

		std::ostringstream oss;
		oss << std::fixed << std::setprecision(2) << ls.second;
		std::string sval = oss.str();
		if (sval.size() > maxVal)
			maxVal = sval.size();
	}

	// Provide minimum widths
	if (maxName < 6) maxName = 6; // e.g., "K" or short names
	if (maxVal < 8) maxVal = 8;   // allow reasonable numeric space

	for (const auto &ls : loesVector)
	{
		std::ostringstream oss;
		oss << std::fixed << std::setprecision(2) << ls.second;
		std::string sval = oss.str();

		std::cout << std::left << std::setw((int)maxName) << ls.first
				  << " = " << std::right << std::setw((int)maxVal) << sval << std::endl;
	}
}

void WriteMatrix(const LPMatrix LPM)
{
	cout << "Den endelige LPMatrix:" << endl;
	cout << LPM;
	cout << endl;
}

void WriteUligheder(const vector<string> &ligninger)
{
	cout << "LP-problemets uligheder: " << std::endl;
	if (ligninger.empty())
	{
		cout << "\tIngen uligheder eller K-funktion fundet." << std::endl;
		return;
	}

	auto trim = [](std::string s) {
		auto l = s.find_first_not_of(" \t\r\n");
		if (l == std::string::npos) return std::string();
		auto r = s.find_last_not_of(" \t\r\n");
		return s.substr(l, r - l + 1);
	};

	auto split_terms = [&](const std::string &left) {
		std::vector<std::string> terms;
		std::string s = trim(left);
		if (s.empty()) return terms;

		if (s[0] != '+' && s[0] != '-') s = std::string("+ ") + s;

		size_t n = s.size();
		size_t start = 0;
		for (size_t i = 1; i < n; ++i)
		{
			if (s[i] == '+' || s[i] == '-')
			{
				terms.push_back(trim(s.substr(start, i - start)));
				start = i;
			}
		}
		if (start < n) terms.push_back(trim(s.substr(start)));
		return terms;
	};

	size_t total = ligninger.size();
	size_t numUligheder = (total > 0) ? total - 1 : 0;

	for (size_t i = 0; i < numUligheder; ++i)
	{
		std::string ineq = ligninger[i];
		size_t pos = ineq.find_first_of("<>=");
		char rel = (pos != std::string::npos) ? ineq[pos] : ' ';
		std::string left = (pos != std::string::npos) ? ineq.substr(0, pos) : ineq;
		std::string right = (pos != std::string::npos) ? ineq.substr(pos + 1) : "";
		left = trim(left);
		right = trim(right);

		auto terms = split_terms(left);

		struct Term { std::string sign; std::string coeff; std::string var; };
		std::vector<Term> parsed;
		size_t maxCoeff = 0, maxVar = 0;
		for (auto &t : terms)
		{
			if (t.empty()) continue;
			char sign = t[0];
			std::string rest = trim(t.substr(1));
			std::istringstream iss(rest);
			std::string numtok;
			iss >> numtok;
			std::string var;
			std::getline(iss, var);
			var = trim(var);
			std::ostringstream coeffoss;
			try {
				double d = std::stod(numtok);
				coeffoss << std::fixed << std::setprecision(2) << d;
			} catch (...) {
				coeffoss << numtok;
			}
			Term P{std::string(1, sign), coeffoss.str(), var};
			if (P.coeff.size() > maxCoeff) maxCoeff = P.coeff.size();
			if (P.var.size() > maxVar) maxVar = P.var.size();
			parsed.push_back(P);
		}

		cout << std::left << std::setw(14) << ("Ulighed nr. " + std::to_string(i + 1));

		for (size_t j = 0; j < parsed.size(); ++j)
		{
			auto &p = parsed[j];
			cout << std::right << std::setw(2) << p.sign << ' ';
			cout << std::right << std::setw((int)maxCoeff) << p.coeff << ' ';
			cout << std::left << std::setw((int)maxVar + 2) << p.var;
		}

		if (rel != ' ')
		{
			cout << ' ' << rel << ' ' << trim(right);
		}
		cout << std::endl;
	}

	if (total > 0)
	{
		std::string ineq = ligninger[total - 1];
		size_t pos = ineq.find_first_of("<>=");
		std::string left = (pos != std::string::npos) ? ineq.substr(0, pos) : ineq;
		std::string right = (pos != std::string::npos) ? ineq.substr(pos + 1) : "";
		left = trim(left);
		right = trim(right);
		auto terms = split_terms(left);

		cout << std::left << std::setw(14) << "K-funktion:";
		for (auto &t : terms) if (!t.empty()) cout << t << ' ';
		if (!right.empty()) cout << "= " << right;
		cout << std::endl;
	}
}
