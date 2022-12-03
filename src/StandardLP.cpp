//------------StandardLP.cpp
#include "StandardLP.h"

StandardLP::StandardLP(LPMatrix &LPM, bool trace)
	: A(LPM) // da A er en reference til LPMatrix-objektet i class LP
{
	bTrace = trace;
	m = A.NoOfRows() - 2;
	n = A.NoOfCols() - 2;
}

StandardLP::~StandardLP()
{
}

bool StandardLP::SolveLP()
{
	// MArker basisvariabler
	size_t p, q; // R�kke og s�jleindex
	while (true)
	{
		if (bTrace) // Testudskrift �nskes
		{
			char tegn;
			cout << "Trace  - Matrix: \n"
				 << A << "Tast tegn: ";
			cin >> tegn;
		} // --------    End trace    -----------
		q = BestemPivotSoejle();
		if (q > 0) // S� findes en s�jle i K med koefficient > 0
		{
			p = BestemPivotRaekke(q);
			if (p == 0) // Ingen raekke fandtes
				return false;
			NySimplexTabel(p, q);
		}
		else // q = 0 der findes ingen koefficienter > 0 i K-funktionen
			return true;
	} // end while
}

void StandardLP::MarkerBasisVariabler()
{
	for (size_t j = 1; j <= n; ++j)
		if (SoejleErEnhedsSoejle(j))
			A[0][j] = j;
}

size_t StandardLP::BestemPivotSoejle()
{
	double c = 1E-10;
	size_t q = 0; // S�jleindex er uden for variablern, der begynder i Soejle 1

	for (size_t j = 1; j <= n; ++j)
	{
		if (A[m + 1][j] > c)
		{
			c = A[m + 1][j];
			q = j;
		}
	}
	return q; // Hvis q = 0 har vi ingen ikke-negative koefficienter fundet i K
}

size_t StandardLP::BestemPivotRaekke(size_t q)
{
	double kvotient = 1E+38; // Meget stor v�rdi
	double broek = 0;
	size_t p = 0;

	for (size_t i = 1; i <= m; ++i)
	{
		// Positive koefficienter i pivotsoejle og b-soejle
		if (A[i][q] > 0.0 && A[i][n + 1] > 0.0)
		{
			broek = A[i][n + 1] / A[i][q];
			if (broek < kvotient)
			{
				kvotient = broek;
				p = i;
			}
		}
	}
	return p;
}

void StandardLP::NySimplexTabel(size_t p, size_t q)
{
	double pivot = A[p][q];
	double c = 0.0;
	SkiftBasisIndex(p, q);
	for (size_t i = 1; i <= m + 1; ++i)
		if (i != p)
		{
			c = -A[i][q] / pivot;
			if (c != 0.0)
			{
				// Eliminer variable i r�kke 'i' ved at addere c*raekke p
				// til raekke 'i'
				A.AddToRow(i, p, c);
			}
		}
	c = 1 / pivot;
	A.ScaleRow(p, c);

	for (int i = 1; i <= m + 1; ++i)
		A[i][q] = 0.0;
	A[p][q] = 1.0;
}

void StandardLP::SkiftBasisIndex(size_t p, size_t q)
{
	for (size_t j = 1; j <= n; ++j)
		if (A[0][j] != 0.0 && A[p][j] == 1.0 && j != q)
		{
			A[0][j] = 0.0;
			A[0][q] = q;
			return;
		}
}

bool StandardLP::SoejleErEnhedsSoejle(size_t j)
{
	if (fabs(A[m + 1][j] > 1E-10)) // Ikke != 0.0
		return false;

	size_t AntalEtTaller = 0;
	size_t AntalNuller = 0;
	for (size_t i = 1; i <= m; ++i)
	{
		if (A[i][j] == 1.0)
			++AntalEtTaller;
		else if (fabs(A[i][j]) < 1E-10)
			++AntalNuller;
		else // Hverken 0 eller 1
			return false;
	}
	return (AntalEtTaller == 1 && AntalNuller == m - 1);
}