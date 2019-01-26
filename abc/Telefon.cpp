#include "pch.h"
#include "Telefon.h"


extern int sprawdz_liczbe(string liczba1, int dlugoscmin, int dlugoscmax);
extern MYSQL*conn;
Telefon::Telefon()
{
}


Telefon::~Telefon()
{
}
Telefon::Telefon(string idTelefonu_a, string numerTelefonu_a,
	string numerKierunkowy_a, string panstwo_a,
	string mindlugoscTelefonu_a, string maxDlugoscTelefonu_a)
	: NumerKierunkowy(numerKierunkowy_a, panstwo_a, mindlugoscTelefonu_a, maxDlugoscTelefonu_a)
{
	idTelefonu = atoi(idTelefonu_a.c_str());
	numerTelefonu = atol(numerTelefonu_a.c_str());
}
Telefon::Telefon(int id_a, string numerTelefonu_a, NumerKierunkowy kierunek, int id_osoby)
{
	cout << "NUMER TEL:" << numerTelefonu_a << endl;
	idTelefonu = id_a;
	numerKierunkowy = kierunek.numerKierunkowy;
	minDlugoscTelefonu = kierunek.minDlugoscTelefonu;
	maxDlugoscTelefonu = kierunek.maxDlugoscTelefonu;
	if (!sprawdz_liczbe(numerTelefonu_a, minDlugoscTelefonu, maxDlugoscTelefonu))
	{
		cout << "Numer nie spelnia wymogow dlugosci w podanym panstwie." << endl;
		cout << "Operacja zostala przerwana" << endl;
	}
	else
	{

		dodaj_do_bazy(id_osoby, numerTelefonu_a);
		system("PAUSE");
	}
}

void Telefon::dodaj_do_bazy(int id_osoby,string numerTelefonu)
{
	stringstream sszapytanie;
	sszapytanie << "SELECT * FROM telefon WHERE NUMER_TELEFONU='" << numerTelefonu << "' AND NUMER_KIERUNKOWY='" << numerKierunkowy << "';";
	int qstate = mysql_query(conn, sszapytanie.str().c_str());
	if (!qstate)
	{
		MYSQL_ROW row;
		MYSQL_RES *res;
		res = mysql_store_result(conn);
		if ((row = mysql_fetch_row(res)) == NULL)
		{
			stringstream sszapytanie2;
			sszapytanie2 << "INSERT INTO telefon (NUMER_TELEFONU, ID_OSOBY, NUMER_KIERUNKOWY ) VALUES "
				<< "('" << numerTelefonu << "', '" << id_osoby << "', '" << numerKierunkowy << "');";
			qstate = mysql_query(conn, sszapytanie2.str().c_str());
			if(!qstate)
			{
				cout << "Dodano numer telefonu" << endl;
			}
			else
			{
				cout << "Wystapil blad przy dodawaniu: " << mysql_error(conn) << endl;
			}
		}
		else
		{
			cout << "Nie mozna dodac takiego numeru! Ten numer jest juz zajety" << endl;
		}
	}
	else
	{
		cout << "Blad zapytania: " << mysql_error(conn) << endl;
	}


}
Telefon::Telefon(int i, string numer_telefonu, int min, int max, string numerkierunkowy)
{
	idTelefonu = i;
	numerTelefonu = atol(numer_telefonu.c_str());
	minDlugoscTelefonu = min;
	maxDlugoscTelefonu = max;
	numerKierunkowy = numerkierunkowy;
	if (!sprawdz_liczbe(numer_telefonu, min, max))
		idTelefonu = -100;
}
