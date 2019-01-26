#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <mysql.h>

using namespace std;

class NumerKierunkowy
{
private:
protected:

	string numerKierunkowy;
	string panstwo;
	friend class Telefon;

public:
	int minDlugoscTelefonu;
	int maxDlugoscTelefonu;
	NumerKierunkowy();
	NumerKierunkowy(string numerKierunkowy_a, string panstwo, string mindlugoscTelefonu, string maxDlugoscTelefonu);
	void wyswietl_Numer_Kierunkowy()
	{
		cout << panstwo << ":\t" << numerKierunkowy << endl;
		cout << "\tDopuszczalna dlugosc numeru: " << minDlugoscTelefonu << "-" << maxDlugoscTelefonu << " cyfr." << endl;
	}	
	void wyswietl_Numer_Kierunkowy_Wizytowka()
	{
		cout << panstwo << ":\t" << numerKierunkowy << endl;
	}
	void dodaj_Do_Bazy();
	int sprawdz_Poprawnosc_Danych();
	int sprawdz_kierunkowy(string numerKierunkowy_a)
	{
		if (numerKierunkowy == numerKierunkowy_a)
			return 1;
		else return 0;
	}
	~NumerKierunkowy();
};

