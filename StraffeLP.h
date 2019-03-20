//---------StraffeLP.h
//Af BJR
//21/11-1999
//Efter TOHO's note

#include "StandardLP.h"



class StraffeLP : public StandardLP
{
public:

	StraffeLP (LPMatrix& LPM, bool trace = false);
	//StandardLP indeholder reference til Matrix

	virtual bool SolveLP();

protected:
	void OmformRaekkerMedKunstVar();	//overflødig efter fase 5
	void BestemAntalKunstVar();
	void BestemM();
	void StrafKmedM();
	void EliminerMiK();

protected:

	size_t nKV;		//antal kunstige variabler
	long M;			//Store M

	long M_KONST;	//Den konstant som "største" forhold multiplicers med,
					//jvf. p. 67
};
