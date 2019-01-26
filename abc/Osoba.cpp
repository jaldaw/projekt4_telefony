#include "pch.h"
#include "Osoba.h"
#include "Polaczenie.h"
#include <algorithm>
using namespace std;

extern MYSQL* conn;
extern int zmiennastatystyczna1;
extern int zmiennastatystyczna2;
extern int zmiennastatystyczna3;
extern int zmiennastatystyczna4;
Osoba::Osoba()
{
	stanowisko = 0;
}

bool sorting(const Polaczenie& a, const Polaczenie& b)
{
	return a.czasRozpoczecia < b.czasRozpoczecia;
}



Osoba::~Osoba()
{
}

Osoba::Osoba(string idOsoby_a, string imie_a, string nazwisko_a, string adres_a, string plec_a
	, string stanowisko_a, string PESEL_a, string login_a, string haslo_a)
{
	idOsoby = atoi(idOsoby_a.c_str());
	imie = imie_a;
	nazwisko = nazwisko_a;
	adres = adres_a;
	plec = plec_a[0];
	if (stanowisko_a == "ADM")
		stanowisko = 1;
	else stanowisko = 2;
	PESEL = PESEL_a;
	login = login_a;
	haslo = haslo_a;
	stringstream sszapytanie;
	sszapytanie << "SELECT * FROM telefon, numer_kierunkowy WHERE "
		<< "telefon.ID_OSOBY = '" << idOsoby << "' AND telefon.NUMER_KIERUNKOWY = numer_kierunkowy.NUMER_KIERUNKOWY;";
	int qstate = mysql_query(conn, sszapytanie.str().c_str());
	if (!qstate)
	{
		MYSQL_ROW row;
		MYSQL_RES *res;
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			Telefon temp(row[0], row[1], row[3], row[5], row[6], row[7]);
			telefony.push_back(temp);
		}
	}
	else
	{
		cout << "Blad zapytania: " << mysql_error(conn) << endl;
	}
}
void Osoba::wyswietl()
{
	cout << "************************************" << endl;
	if(idOsoby!=0)
		cout << "ID = " << idOsoby << endl;
	cout << "\t" << imie << " " << nazwisko << endl;
	cout << "\tAdres: " << adres << endl;
	cout << "\tPESEL : " << PESEL << endl;
	cout << "\tLogin : " << login << endl;
	cout << "\tTelefony:" << endl;
	if(idOsoby!=0)
		if (telefony.size() == 0)	cout << "\tUzytkownik nie ma telefonu w bazie danych" << endl;
		else
			for (int i = 0; i < telefony.size(); i++)
			{
				cout<<"\t\t";
				telefony[i].wyswietl();
			}
	cout << "************************************" << endl;

}
void Osoba::dodaj_Do_Bazy()
{
	stringstream sszapytanie;
	sszapytanie << "INSERT INTO osoba (IMIE, NAZWISKO, ADRES, PLEC, STANOWISKO, PESEL, LOGIN, HASLO) VALUES "
				<<"('"<<imie<<"' ,'"<<nazwisko<<"' , '"<<adres<<"', '"<<plec<<"' , 'USR', '"<<PESEL<<"', '"<<login<<"' , '"<<haslo<<"');";

	int qstate = mysql_query(conn, sszapytanie.str().c_str());
	if (!qstate)
	{
		cout << "Osoba zostala dodana" << endl;
	}
	else
		cout << "Blad kwerendy: " << mysql_error(conn) << endl;

}
void Osoba::polaczenia(int tryb)
{
	int zestawienie_przychodzacych = 0;
	vector<Polaczenie> polaczenia;
	if (tryb == 1 || tryb == 3 || tryb == 4 || tryb == 5 || tryb == 6 || tryb == 7 || tryb == 8 || tryb == 9 || tryb == 10 || tryb == 11)
	{
		//jako nadawce
		for (int i = 0; i < telefony.size(); i++)
		{
			stringstream sszapytanie;
			sszapytanie << "SELECT *, ceil(TIME_TO_SEC(timediff(`CZAS_ZAKONCZENIA`,`CZAS_ROZPOCZECIA`))/60) FROM `polaczenie` WHERE ID_TELEFONU=" << telefony[i].idTelefonu;
			int qstate = mysql_query(conn, sszapytanie.str().c_str());
			if (!qstate)
			{
				MYSQL_ROW row;
				MYSQL_RES *res;
				res = mysql_store_result(conn);
				while (row = mysql_fetch_row(res))
				{
					Polaczenie temp(row[0], row[2], row[5], row[3], row[4], row[6], telefony[i], imie, nazwisko);
					polaczenia.push_back(temp);
				}
			}
			else
			{
				cout << "Blad kwerendy: " << mysql_error(conn) << endl;
			}
		}
	}
	if (tryb == 2 || tryb == 3 || tryb == 4 || tryb == 5 || tryb == 6 || tryb == 7 || tryb == 8 || tryb == 9 )
	{
		//jako odbiorca
		for (int i = 0; i < telefony.size(); i++)
		{
			stringstream sszapytanie;
			sszapytanie << "SELECT *, round(TIME_TO_SEC(timediff(`CZAS_ZAKONCZENIA`,`CZAS_ROZPOCZECIA`))/60) FROM `polaczenie` WHERE ODBIORCA_TELEFON='" << telefony[i].numerTelefonu << "' AND ODBIORCA_KIERUNKOWY='"<<telefony[i].numerKierunkowy<<"';";
			int qstate = mysql_query(conn, sszapytanie.str().c_str());
			if (!qstate)
			{
				MYSQL_ROW row;
				MYSQL_RES *res;
				res = mysql_store_result(conn);
				while (row = mysql_fetch_row(res))
				{	
					stringstream sszapytanie2;
					sszapytanie2 << "SELECT imie,nazwisko,panstwo, numer_kierunkowy.NUMER_KIERUNKOWY, telefon.NUMER_TELEFONU FROM osoba,telefon,numer_kierunkowy WHERE osoba.ID_OSOBY=telefon.ID_OSOBY AND telefon.NUMER_KIERUNKOWY=numer_kierunkowy.NUMER_KIERUNKOWY AND ID_TELEFONU = '"<<row[1]<<"';";
					int qstate = mysql_query(conn, sszapytanie2.str().c_str());
					if (!qstate)
					{
						MYSQL_ROW row2;
						MYSQL_RES *res;
						res = mysql_store_result(conn);
						if ((row2 = mysql_fetch_row(res)) == NULL)
						{
							cout << "Wystapil blad" << endl;
						}
						else
						{
							Polaczenie temp(telefony[i], row2[0], row2[1], row2[2], row2[3], row2[4], row[0],row[3],row[4], row[6], imie, nazwisko);
							polaczenia.push_back(temp);
							zestawienie_przychodzacych++;
						}
					}
					else
					{
						cout << "Blad kwerendy: " << mysql_error(conn) << endl;
					}

				}
			}
			else
			{
				cout << "Blad kwerendy: " << mysql_error(conn) << endl;
			}
		}


	}
	if (tryb == 3)
	{
		sort(polaczenia.begin(), polaczenia.end(), sorting);
	}
	if(tryb == 1||tryb == 2 || tryb == 3)
		for (int i = 0; i < polaczenia.size(); i++)
			polaczenia[i].wyswietl();
	//srednia
	if (tryb == 4 ||tryb == 8 || tryb == 9)
	{
		if (polaczenia.size() == 0 && tryb == 4)
		{
			cout << "Nie zarejestrowano polaczen" << endl;
		}
		else if(polaczenia.size()!=0)
		{
			int srednia = 0;
			for (int i = 0; i < polaczenia.size(); i++)
			{
				srednia += polaczenia[i].dlugoscPolaczenia;
			}
			if(tryb == 4)
				cout << "Srednia dlugosc rozmow wynosi: " << int(srednia / polaczenia.size()) << "minut" << endl;
			else if (tryb == 8)
			{
				zmiennastatystyczna1 += int(srednia / polaczenia.size());
				zmiennastatystyczna2++;
			}
			else if (tryb == 9)
			{
				if (plec == 'K')
				{
					cout << "Kobieta" << imie << endl;
					zmiennastatystyczna1 += int(srednia / polaczenia.size());
					zmiennastatystyczna2++;
				}
				else if (plec == 'M')
				{
					cout << "Mezczyzna" <<imie<< endl;
					zmiennastatystyczna3 += int(srednia / polaczenia.size());
					zmiennastatystyczna4++;
				}
			}
		}
	}
	if (tryb == 5)
	{
		int polaczenie_miedzynarodowy = 0;
		for (int i = 0; i < polaczenia.size(); i++)
		{
			polaczenie_miedzynarodowy += polaczenia[i].miedzynarodowe();
		}

		int po = double(polaczenie_miedzynarodowy) / double(polaczenia.size())*100;
		cout << "Polaczenia miedzynarodowe stanowia " << po << " % polaczen , zas polaczenia panstwowe " << 100 - po << " % polaczen" << endl;
	}
	if (tryb == 6)
	{
		cout << zestawienie_przychodzacych << " " << polaczenia.size() << endl;
		int po = double(zestawienie_przychodzacych) / double(polaczenia.size()) * 100;
		cout << "Polaczenia przychodzace stanowia " << po << " % polaczen, a wychodzace " << 100 - po << " % polaczen." << endl;
	}
	if (tryb == 7)
	{
		int liczba_poza_siecia = 0;
		for (int i = 0; i < polaczenia.size(); i++)
		{
			liczba_poza_siecia +=polaczenia[i].polaczenie_poza_siec();


		}
		int po = double(liczba_poza_siecia) / double(polaczenia.size()) * 100;
		cout << po << "% wszystkich polaczen wychodzi poza siec" << endl;
	}
	if (tryb == 10)
	{
		for (int i = 0; i < polaczenia.size(); i++)
		{
			zmiennastatystyczna1 += polaczenia[i].dlugoscPolaczenia;
			zmiennastatystyczna2++;
		}
	}
	if (tryb == 11)
	{
		for (int i = 0; i < polaczenia.size(); i++)
		{
			if (plec == 'K')
			{
				zmiennastatystyczna1 += polaczenia[i].dlugoscPolaczenia;
				zmiennastatystyczna2++;
			}
			else if (plec == 'M')
			{
				zmiennastatystyczna3 += polaczenia[i].dlugoscPolaczenia;
				zmiennastatystyczna4++;
			}
		}
	}
}
void Osoba::ilosc_telefonow_wzgledem_plci(int& ilosc_kobiet, int& ilosc)
{
	for (int i = 0; i < telefony.size(); i++)
	{
		ilosc++;
		if (plec == 'K')
			ilosc_kobiet++;
		else
		{
			cout << "M: " << imie << " " << nazwisko << endl;
		}
	}
}
int Osoba::liczba_kierunkowych(vector<pair<string, int>>& numer)
{

	for (int i = 0; i < telefony.size(); i++)
	{
		int rezultat = 1;
		for (int k = 0; k < numer.size(); k++)
		{
			if (telefony[i].panstwo == numer[k].first)
			{
				numer[k].second++;
				rezultat = 0;
				break;
			}
		}
		if (rezultat == 1)
		{
			string numerk = telefony[i].panstwo;
			pair<string, int> temp;
			temp = make_pair(numerk, 1);
			numer.push_back(temp);
		}
	}
}
int Osoba::wyszukaj_id_uzytkownika(string numerkierunkowynadawcy, string numernadawcy)
{
	for (int i = 0; i < telefony.size(); i++)
	{
		if (telefony[i].numerKierunkowy == numerkierunkowynadawcy && telefony[i].numerTelefonu == atol(numernadawcy.c_str()))
			return telefony[i].idTelefonu;
		else return 0;
	}
}