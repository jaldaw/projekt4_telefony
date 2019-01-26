#include "pch.h"
#include "Polaczenie.h"

extern MYSQL* conn;

Polaczenie::Polaczenie()
{
}


Polaczenie::~Polaczenie()
{
	
}
//Tworzenie polaczenia jesli osoba to nadawca
Polaczenie::Polaczenie(string idPolaczenia_a, string numerOdbiorcy_a, string numerKierunkowyOdbiorcy_a,
	string czasRozpoczecia_a, string czasZakonczenia_a, string dlugoscPolaczenia_a,
	Telefon tel_a, string imie, string nazwisko)
{
	idPolaczenia = atoi(idPolaczenia_a.c_str());
	numerOdbiorcy = atoi(numerOdbiorcy_a.c_str());
	numerKierunkowyOdbiorcy = numerKierunkowyOdbiorcy_a;
	czasRozpoczecia = czasRozpoczecia_a;
	czasZakonczenia = czasZakonczenia_a;
	dlugoscPolaczenia = atoi(dlugoscPolaczenia_a.c_str());
	numerTelefonu = tel_a.numerTelefonu;
	numerKierunkowy = tel_a.numerKierunkowy;
	minDlugoscTelefonu = tel_a.minDlugoscTelefonu;
	maxDlugoscTelefonu = tel_a.maxDlugoscTelefonu;
	panstwo = tel_a.panstwo;
	this->imie = imie;
	this-> nazwisko = nazwisko;


	// Odczyt nadawcy
	stringstream sszapytanie;
	sszapytanie << "SELECT imie, nazwisko, panstwo FROM osoba, telefon, numer_kierunkowy WHERE telefon.NUMER_TELEFONU='" << numerOdbiorcy << "' AND telefon.NUMER_KIERUNKOWY='" << numerKierunkowyOdbiorcy << "' AND telefon.ID_Osoby = osoba.ID_OSOBY AND numer_kierunkowy.NUMER_KIERUNKOWY = telefon.NUMER_KIERUNKOWY;";
	int qstate = mysql_query(conn, sszapytanie.str().c_str());
	if (!qstate)
	{
		MYSQL_ROW row;
		MYSQL_RES *res;
		res = mysql_store_result(conn);
		if ((row = mysql_fetch_row(res)) == NULL)
		{
			imieOdbiorcy = "";
			nazwiskoOdbiorcy = "";
		}
		else
		{
			imieOdbiorcy = row[0];
			nazwiskoOdbiorcy = row[1];
			panstwoOdbiorcy = row[3];
		}
	}
	else
	{
		cout << "Blad kwerendy: " << mysql_error(conn) << endl;
	}
	//Chcemy odczytac panstwo niezidentyfikowanego odbiorcy
	if (panstwoOdbiorcy == "")
	{
		stringstream sszapytanie;
		sszapytanie << "SELECT PANSTWO FROM numer_kierunkowy WHERE NUMER_KIERUNKOWY='" << numerKierunkowyOdbiorcy << "';";
		int qstate = mysql_query(conn, sszapytanie.str().c_str());
		if (!qstate)
		{
			MYSQL_ROW row;
			MYSQL_RES *res;
			res = mysql_store_result(conn);
			if ((row = mysql_fetch_row(res)) != NULL)
			{
				panstwoOdbiorcy = row[0];
			}
		}
		else
		{
			cout << "Blad kwerendy: " << mysql_error(conn);
		}
	}

}
//Tworzenie polaczenia jesli osoba to odbiorca
Polaczenie::Polaczenie(Telefon tel, string imieNadawcy, string nazwiskoNadawcy, string panstwoNadawcy, string numerKierunkowyNadawcy, string numerNadawcy, string idPolaczenia,string czasRozpoczecia_a, string czasZakonczenia_a, string dlugoscPolaczenia_a, string imie, string nazwisko)
{
	this->idPolaczenia = atoi(idPolaczenia.c_str());
	numerOdbiorcy = tel.numerTelefonu;
	numerKierunkowyOdbiorcy = tel.numerKierunkowy;
	panstwoOdbiorcy = tel.panstwo;
	imieOdbiorcy = imie;
	nazwiskoOdbiorcy = nazwisko;
	czasRozpoczecia = czasRozpoczecia_a;
	czasZakonczenia = czasZakonczenia_a;
	dlugoscPolaczenia = atoi(dlugoscPolaczenia_a.c_str());
	numerTelefonu = atoi(numerNadawcy.c_str());
	numerKierunkowy = numerKierunkowyNadawcy;
	this->imie = imieNadawcy;
	this->nazwisko = nazwiskoNadawcy;
	panstwo = panstwoNadawcy;


}
Polaczenie::Polaczenie(string rozpoczecie_rozmowy, string zakonczenie_rozmowy, int id, Telefon tel)
	{
		czasRozpoczecia = rozpoczecie_rozmowy;
		czasZakonczenia = zakonczenie_rozmowy;
		int roznicaczasu = sprawdz_chronologie();
		if (roznicaczasu)
		{
			numerOdbiorcy = tel.numerTelefonu;
			numerKierunkowyOdbiorcy = tel.numerKierunkowy;
			dodaj_Do_Bazy(id);
		}
		system("PAUSE");
 
	}
void Polaczenie::wyswietl()
{
	cout << "POLACZENIE*********************************\n" << endl;
	cout << "Nadawca:" << endl;
	cout << "\t" <<imie<<" "<<nazwisko<< endl;
	cout << "\t TEL: " << numerKierunkowy << " " << numerTelefonu << endl;
	cout << "Odbiorca: " << endl;
	if(imieOdbiorcy!="")
		cout << "\t " << imieOdbiorcy << " " << nazwiskoOdbiorcy << endl;
	else cout << "\tOdbiorca spoza bazy danych" << endl;
	cout << "\t TEL: "<<numerKierunkowyOdbiorcy << " " << numerOdbiorcy << endl;
	cout << "Data: " << czasRozpoczecia << endl;
	cout << "\tRozmowa trwala: " << dlugoscPolaczenia << " minut" << endl;
	if (numerKierunkowy == numerKierunkowyOdbiorcy)
		cout << "Rozmowa w obrebie panstwa " << panstwo << endl;
	else
		cout << "Rozmowa miedzynarodowa " << panstwo << " - " << ((panstwoOdbiorcy == "") ? "Kraj, ktorego nie ma w bazie" : panstwoOdbiorcy) << endl;
	cout << "*******************************************\n" << endl;
}

int Polaczenie::sprawdz_chronologie()
{
	stringstream sszapytanie;
	sszapytanie << "SELECT TIMEDIFF('" << czasZakonczenia << "' , '" << czasRozpoczecia << "');";
	int qstate = mysql_query(conn, sszapytanie.str().c_str());
	if (!qstate)
	{
		MYSQL_ROW row;
		MYSQL_RES *res;
		res = mysql_store_result(conn);
		row = mysql_fetch_row(res);
		string roznica = row[0];
		if (roznica[0] == '-')
		{
			cout << "Data zakonczenia jest chronologicznie wczesniej niz zakonczenia!" << endl;
			return 0;
		}
			
		else return 1;
	}
	else
	{
		cout <<  "Blad zapytania: " << mysql_error(conn) << endl;
	}
}
void Polaczenie::dodaj_Do_Bazy(int id)
{
	stringstream sszapytanie;
	sszapytanie << "INSERT INTO polaczenie (ID_TELEFONU, ODBIORCA_TELEFON, CZAS_ROZPOCZECIA, CZAS_ZAKONCZENIA, ODBIORCA_KIERUNKOWY) "
		<< " VALUES('"<<id<<"' , '"<<numerOdbiorcy<<"', '"<<czasRozpoczecia<<"', '"<< czasZakonczenia <<"' , '"<<numerKierunkowyOdbiorcy<<"');";
	int qstate = mysql_query(conn, sszapytanie.str().c_str());
	if (!qstate)
	{
		cout << " Polaczenie zostalo dodane" << endl;
	}
	else
	{
		cout << "Blad zapytania: " << mysql_error(conn) << endl;
	}
}
