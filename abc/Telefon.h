#pragma once
#include "NumerKierunkowy.h"

class Telefon : public NumerKierunkowy
{
private:
	int idTelefonu;
	long numerTelefonu;
	friend class Polaczenie;
	friend class Osoba;

public:
	Telefon();
	~Telefon();
	Telefon(string idTelefonu_a, string numerTelefonu_a,
			string numerKierunkowy_a, string panstwo_a, 
			string mindlugoscTelefonu_a, string maxDlugoscTelefonu_a);
	Telefon(int id_a, string numerTelefonu_a, NumerKierunkowy kierunek, int id_osoby);
	Telefon (int i, string numer_telefonu, int min, int max, string numerkierunkowy);
	void wyswietl()
	{
		cout << numerKierunkowy << " " << numerTelefonu << " (" << panstwo << ")" << endl;
	}
	void dodaj_do_bazy(int id_osoby, string numerTelefonu);
	int sprawdz_dlugosc()
	{
		if (idTelefonu == -100) return 0;
		else return 1;
	}
};

