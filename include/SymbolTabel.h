//---------SymbolTabel.h
//

#ifndef SYMBOLTABEL_H
#define SYMBOLTABEL_H

#include "STLdef.h"

class SymbolTabel
{
public:
	SymbolTabel();

	void InsertID(const string &IdName);
	bool Find(const string &id, size_t &LPMidx);
	string GetID(size_t LPMidx);
	size_t GetNoOfVars();

protected:
	vector<string> tabel;

	void MakeUpper(string &source);
};

#endif
