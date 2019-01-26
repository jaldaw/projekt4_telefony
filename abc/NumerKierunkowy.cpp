#include "pch.h"
#include "NumerKierunkowy.h"
#include <iostream>


using namespace std;

extern MYSQL *conn;
extern int sprawdz_liczbe(string liczba, char znak_specjalny);
extern int sprawdz_liczbe(int liczbamin,int liczbamax);
extern int sprawdz_slowo(string slowo, int duze_litery);
NumerKierunkowy::NumerKierunkowy()
{
}


NumerKierunkowy::~NumerKierunkowy()
{
}
NumerKierunkowy::NumerKierunkowy(string numerKierunkowy_a, string panstwo_a, string mindlugoscTelefonu_a, string maxDlugoscTelefonu_a)
{
	numerKierunkowy = numerKierunkowy_a;
	panstwo = panstwo_a;
	minDlugoscTelefonu = atoi(mindlugoscTelefonu_a.c_str());
	maxDlugoscTelefonu = atoi(maxDlugoscTelefonu_a.c_str());
}
void NumerKierunkowy::dodaj_Do_Bazy()
{
	stringstream sszapytanie;
	sszapytanie << "INSERT INTO numer_kierunkowy (NUMER_KIERUNKOWY, PANSTWO, MIN_DLUGOSC_NUMERU, MAX_DLUGOSC_NUMERU) VALUES "
		<< "('" << numerKierunkowy << "' , '" << panstwo << "' , '" << minDlugoscTelefonu << "' , '" << maxDlugoscTelefonu << "');";
	int qstate = mysql_query(conn, sszapytanie.str().c_str());
	if (!qstate)
	{
		cout << "Dodano numer kierunkowy do bazy" << endl;
	}
	else
	{
		cout << "Blad wstawiania: " << mysql_error(conn) << endl;
	}

}
int NumerKierunkowy::sprawdz_Poprawnosc_Danych()
{
	int rezultat_jeden=1;
	int rezultat = 1;
	rezultat_jeden=sprawdz_liczbe(numerKierunkowy, '+');
	if (rezultat_jeden == 0)
	{
		cout << "Nieprawidlowo wpisany numer kierunkowy" << endl;
		rezultat = 0;
	}
	else if (numerKierunkowy[0] != '+')
	{
		cout << "Nieprawidlowo wpisany numer kierunkowy" << endl;
		rezultat = 0;
	}

	rezultat_jeden = sprawdz_liczbe(minDlugoscTelefonu, maxDlugoscTelefonu);;
	if (rezultat_jeden == 0)
	{
		cout << "Zle wpisano liczbe maksymalna lub minimalna" << endl;
		rezultat = 0;
	}

	rezultat_jeden = sprawdz_slowo(panstwo, 1);
	if (rezultat_jeden == 0)
	{
		cout << "Nieprawidlowo wpisane panstwo" << endl;
		rezultat = 0;
	}
		return rezultat;
}