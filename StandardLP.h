//----------StandardLP.h
//
//
//
#ifndef STANDARDLP_H
#define STANDARDLP_H

#include "STLdef.h"
#include "LPMatrix.h"

class StandardLP
{
public:
	StandardLP(LPMatrix& LPM, bool trace = false);
	//Benytter r�kkerne 1 til m+1
	//og s�jlerne 1 til n+1
	virtual ~StandardLP();
	virtual bool SolveLP();
protected:
	void MarkerBasisVariabler();
	bool SoejleErEnhedsSoejle(size_t j);
	size_t	BestemPivotSoejle();
	size_t	BestemPivotRaekke(size_t q);
	void SkiftBasisIndex(size_t p, size_t q);
	void NySimplexTabel(size_t p, size_t q);
	
	//Atributter
	LPMatrix& A;
	size_t m;
	size_t n;
	bool bTrace;	//true: matrix udskrives p� cout ved hver iteration
};

#endif
