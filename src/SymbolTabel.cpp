//----------SymbolTabel.cpp
//
//
//

#include "SymbolTabel.h"

SymbolTabel::SymbolTabel()
{
	tabel.push_back(" ");
}

/*
void SymbolTabel::InsertID(const string& IdName)
{
	size_t id;
	if(!Find(IdName,id))
	{
		tabel.push_back(" ");
		string newElem=IdName;
		MakeUpper(newElem);
		for(size_t i=tabel.size()-2;i>0 && newElem<tabel[i];--i)
			tabel[i+1]=tabel[i];
		tabel[i+1]=newElem;
	}
}*/

void SymbolTabel::InsertID(const string &IdName)
{
	/*

	 size_t i;
	 string local;
	 local = IdName;

	 MakeUpper(local);

	 if(!Find(local,i))
	 {
		 tabel.push_back(" "); //s�t "  " bagest

		 //string temp;
		 int marker1, marker2;
		 for(marker1 = 1; marker1 < tabel.size(); marker1++)
		 {
			 if(tabel[marker1] < tabel[marker1 - 1])
			 {
				 local = tabel[marker1];
				 for(marker2 = marker1 - 1; marker2 >= 0; marker2--)
				 {
					 tabel[marker2 + 1] = tabel[marker2];
					 if(marker2 == 0 || tabel[marker2 - 1] < local)
					 break;
				 }
				 tabel[marker2] = local;
			 }
		 }
	 }*/
	size_t id;
	if (!Find(IdName, id))
	{
		tabel.push_back(" ");
		string newElem = IdName;
		MakeUpper(newElem);
		size_t i = tabel.size() - 2;
		for (; i > 0 && newElem < tabel[i]; --i)
			tabel[i + 1] = tabel[i];
		tabel[i + 1] = newElem;
	}
}

bool SymbolTabel::Find(const string &id, size_t &LPMidx)
{
	string local;
	local = id;
	SymbolTabel::MakeUpper(local);
	for (size_t i = 0; i < tabel.size(); ++i) //Test hvis variablen eksisterer
	{
		if (local == tabel[i])
		{
			LPMidx = i;
			return true;
		}
	}
	return false;
}

string SymbolTabel::GetID(size_t LPMidx)
{
	if (LPMidx <= tabel.size())
	{
		return tabel[LPMidx];
	}
	return 0;
}

size_t SymbolTabel::GetNoOfVars()
{
	return tabel.size() - 1;
}

void SymbolTabel::MakeUpper(string &source)
{
	string::iterator it;
	for (it = source.begin(); it != source.end(); ++it)
		*it = toupper(*it);
}
