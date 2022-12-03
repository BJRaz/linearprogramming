//-------------StraffeLP.cpp
#include "StraffeLP.h"

StraffeLP::StraffeLP(LPMatrix &LPM, bool trace)
	: StandardLP(LPM, trace)
{
	nKV = 0;	  // antal kunstige variabler
	M = 1;		  // M s�ttes til en begyndelse til 1
	M_KONST = 10; // jvf. p 67M s�ttes typisk til 10 gange st�rste forhold
}

bool StraffeLP::SolveLP()
{
	OmformRaekkerMedKunstVar();
	// Benyttes hvis r�kker med kunstige variabler oppstiles med negative b'er
	BestemAntalKunstVar();
	if (nKV > 0)
	{
		BestemM();
		StrafKmedM();
		EliminerMiK();
	}
	return StandardLP::SolveLP();
}

void StraffeLP::OmformRaekkerMedKunstVar()
{
	for (size_t i = 1; i <= m; ++i)
		if (A[i][n + 1] < 0.0)
		{
			A.ScaleRow(i, -1.0);
			A[i][0] = 1.0;
		}
}

void StraffeLP::BestemAntalKunstVar()
{
	for (size_t i = 1; i <= m; ++i)
		if (A[i][0] != 0.0)
			++nKV;
}

void StraffeLP::BestemM()
{
	double kvotient = 1;
	double maxForhold = 1;
	for (size_t i = 1; i <= m; ++i)
		for (size_t j = 1; j <= n; ++j)
			if (A[i][j] != 0)
			{
				maxForhold = fabs(A[m + 1][j] / A[i][j]);
				if (maxForhold > kvotient)
					kvotient = maxForhold;
			}
	M = M_KONST * size_t(kvotient);
}

void StraffeLP::StrafKmedM()
{
	size_t antal = nKV;

	for (size_t j = n; antal > 0; --j)
	{
		A[m + 1][j] = -1.0 * M;
		--antal;
	}
}

void StraffeLP::EliminerMiK()
{
	for (size_t i = 1; i <= m; ++i)
		if (A[i][0] == 1.0)
			A.AddToRow(m + 1, i, M);
}
