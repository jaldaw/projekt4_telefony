#include "pch.h"
#include "Przelew.h"
#include <string>
#include <iostream>
#include "ZalogowanyPracownik.h"
#include "Konto.h"
#include <mysql.h>
#include <sstream>
using namespace std;

Przelew::Przelew()
{
}


Przelew::~Przelew()
{
}

Przelew::Przelew(string idPrzelew_a, string wartoscPrzelewu_a, string datarealizacjiPrzelewu_a, string numerKontaNadawcy_a, string numerKontaOdbiorcy_a
	, string wykonanie_a, string idPracownika,string nota_a, string imiePracownika, string nazwiskoPracownika, string idKlienta_a, string imieKlienta, string nazwiskoKlienta) 
	: ZalogowanyPracownik(idPracownika, imiePracownika, nazwiskoPracownika) , Klient(idKlienta_a, imieKlienta, nazwiskoKlienta)
{
	idPrzelew = idPrzelew_a;
	wartoscPrzelewu = atof(wartoscPrzelewu_a.c_str());
	dataRealizacjiPrzelewu = datarealizacjiPrzelewu_a;
	numerKontaNadawcy = numerKontaNadawcy_a;
	numerKontaOdbiorcy = numerKontaOdbiorcy_a;
	wykonanie = wykonanie_a;
	nota = nota_a;
}
void Przelew::okresl_rodzajPrzelewu(string id)
{
	if (numerKontaNadawcy == "BANK") rodzajPrzelewu = "wplata";
	else if (numerKontaOdbiorcy == "BANK") rodzajPrzelewu = "wyplata";
	else if (idKlienta != id) rodzajPrzelewu = "przelewdo";
	else rodzajPrzelewu = "przelewod";
	cout << "Przelew o id = " << idPrzelew << "To przelew" << rodzajPrzelewu << endl;

}
void Przelew::wyswietl(MYSQL *conn, Klient logcustomer, int tryb, Konto aktywne)
{
	if (rodzajPrzelewu == "wplata" && (tryb == 1 || tryb == 3 || (tryb == 4 && aktywne.numerKonta==numerKontaOdbiorcy)))
	{
		cout << "**********************************************" << endl;
		cout << "Wplata pieniedzy na konto" << endl;
		cout << "WPLACONO NA KONTO " << numerKontaOdbiorcy << endl;
		cout << "Dnia " << dataRealizacjiPrzelewu << endl;
		cout << wartoscPrzelewu << " PLN" << endl;
		cout << "**********************************************" << endl;
	}
	else if (rodzajPrzelewu == "wyplata" && (tryb == 1 || tryb == 3 || (tryb == 4 && aktywne.numerKonta == numerKontaNadawcy)))
	{
		cout << "**********************************************" << endl;
		cout << "Wyplata pieniedzy z konta" << endl;
		cout << "WPLACONO Z KONTA " << numerKontaNadawcy << endl;
		cout << "Dnia " << dataRealizacjiPrzelewu << endl;
		cout << wartoscPrzelewu << " PLN" << endl;
		cout << "**********************************************" << endl;
	}
	else if (rodzajPrzelewu == "przelewdo" && (tryb == 1 || tryb==2) || (tryb == 4 && aktywne.numerKonta == numerKontaNadawcy))
	{
		cout << "**********************************************" << endl;
		cout << "Przelew na konto uzytkownika" << endl;
		cout << "Wyslano przelew Z konta " << numerKontaNadawcy << endl;
		cout << "Na konto " << numerKontaOdbiorcy << endl;
		cout << "OD: " << imieKlienta << " " << nazwiskoKlienta << endl;
		//SZUKAMY, czy nadawca jest naszym klientem
		logcustomer.wypisanie_przelewu();
		cout << "Dnia " << dataRealizacjiPrzelewu << endl;
		cout << wartoscPrzelewu << " PLN" << endl;
		cout << "**********************************************" << endl;
	}
	else if (rodzajPrzelewu == "przelewod" && (tryb == 1 || tryb == 2) || (tryb == 4 && aktywne.numerKonta == numerKontaOdbiorcy))
	{
		cout << "**********************************************" << endl;
		cout << "Przelew z konta uzytkownika" << endl;
		cout << "Wyslano przelew Z konta " << numerKontaNadawcy << endl;
		cout << "Na konto " << numerKontaOdbiorcy << endl;
		cout << "OD: " << imieKlienta << " " << nazwiskoKlienta << endl;
		cout << "DO: ";
		int qstate;
		stringstream sszapytanie;
		string szapytanie;
		MYSQL_ROW row;
		MYSQL_RES *res;
		sszapytanie << "SELECT imie_klienta, nazwisko_klienta FROM klient,konto WHERE id_klient = id_klient_konto AND numer_konta= " << numerKontaOdbiorcy << ";";
		szapytanie = sszapytanie.str();
		const char *zapytanie = szapytanie.c_str();
		qstate = mysql_query(conn, zapytanie);		//rezultat kwerendy

		if (!qstate)
		{
			res = mysql_store_result(conn);
			row = mysql_fetch_row(res);
			if (row == NULL)
			{
				cout << "Odbiorca z innego banku" << endl;;
			}
			else
				cout << "DO: " << row[0] << " " << row[1] << endl;

		}
		else
			cout << "Blad kwerendy edytowania uzytkownika:" << mysql_error(conn) << endl;

		cout << "Dnia " << dataRealizacjiPrzelewu << endl;
		cout << wartoscPrzelewu << " PLN" << endl;
		cout << "**********************************************" << endl;
	}

}