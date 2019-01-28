#pragma once
#include <iostream>
#include <sstream>
#include <mysql.h>
#include <vector>
#include "Telefon.h"
#include <sstream>
#include <utility>
using namespace std;

class Osoba
{
protected:
	string imie;
	string nazwisko;
private:


	string adres;
	char plec;
	string PESEL;
	string login;
	string haslo;
	vector<Telefon> telefony;
public:
	int idOsoby;
	int stanowisko;
	Osoba();
	Osoba(string idOsoby_a, string imie_a, string nazwisko_a,string adres, string plec_a
			,string stanowisko_a, string PESSEL_a, string login_a, string haslo_a);
	void dodaj_Do_Bazy(string stanowisko_s);
	~Osoba();
	void wyswietl();
	void polaczenia(int tryb);
	void wyswietl_wizytowke()
	{
		cout << imie << " " << nazwisko << " " << login << " " << PESEL << endl;
	}
	int plec_uzytkownika()
	{
		if (plec == 'K') return 1;
		else return 0;
	}
	void ilosc_telefonow_wzgledem_plci(int& ilosc_kobiet, int& ilosc);
	int liczba_kierunkowych(vector<pair<string, int>>& numer);
	void dodajpolaczenie();
	int wyszukaj_id_uzytkownika(string numerkierunkowynadawcy, string numernadawcy);
	int sprawdz_login(string login)
	{
		if (login == this->login) return 0;
		else return 1;
	}
};



