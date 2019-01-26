#pragma once
#include "Telefon.h"
#include "Osoba.h"
#include <mysql.h>
class Polaczenie : public Telefon , private Osoba
{
private:
	int idPolaczenia;
	int numerOdbiorcy;
	string numerKierunkowyOdbiorcy;

	string czasZakonczenia;

	string imieOdbiorcy;
	string nazwiskoOdbiorcy;
	string panstwoOdbiorcy;
	int sprawdz_chronologie();
	void dodaj_Do_Bazy(int id);

public:
	string czasRozpoczecia;
	int dlugoscPolaczenia;
	Polaczenie();
	~Polaczenie();
	Polaczenie(string idPolaczenia, string numerOdbiorcy, string numerKierunkowyOdbiorcy, string czasRozpoczecia,string czasZakonczenia,string dlugoscPolaczenia, Telefon tel, string imie, string nazwisko);
	Polaczenie(Telefon tel, string imieNadawcy, string nazwiskoNadawcy, string panstwoNumeruNadawcy, string numerKierunkowyNadawcy, string numerNadawcy, string idPolaczenia, string czasRozpoczecia_a, string czasZakonczenia_a, string dlugoscPolaczenia_a,string imie,string nazwisko);
	void wyswietl();
	int miedzynarodowe()
	{
		cout << "Odbiorca " << numerKierunkowyOdbiorcy << " i nadawca" << numerKierunkowy << endl;
		if (numerKierunkowyOdbiorcy != numerKierunkowy)
		{
			cout << "Zwracam 1" << endl;
			return 1;
		}
		else
		{
			cout << "Zwracam 0" << endl;
			return 0;
		}
	}
	int polaczenie_poza_siec()
	{
		if (imieOdbiorcy == "")
			return 1;
		else return 0;
	}
	Polaczenie(string rozpoczecie_rozmowy,string zakonczenie_rozmowy,int id,Telefon tel);
};

