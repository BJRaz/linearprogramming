//---------StraffeLP.h
//Af BJR
//21/11-1999
//Efter TOHO's note

#include "StandardLP.h"

#ifndef STRAFFELP_H
#define STRAFFELP_H

class StraffeLP : public StandardLP
{
public:

	StraffeLP (LPMatrix& LPM, bool trace = false);
	//StandardLP indeholder reference til Matrix

	virtual bool SolveLP();

protected:
	void OmformRaekkerMedKunstVar();	//overfl�dig efter fase 5
	void BestemAntalKunstVar();
	void BestemM();
	void StrafKmedM();
	void EliminerMiK();

protected:

	size_t nKV;		//antal kunstige variabler
	long M;			//Store M

	long M_KONST;	//Den konstant som "st�rste" forhold multiplicers med,
					//jvf. p. 67
};

#endif