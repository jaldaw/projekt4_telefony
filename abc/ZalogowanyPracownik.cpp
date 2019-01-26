#include "pch.h"
#include "ZalogowanyPracownik.h"


ZalogowanyPracownik::ZalogowanyPracownik()
{
}
ZalogowanyPracownik::ZalogowanyPracownik(string id_pracownika, string imie_pracownika, string nazwisko_pracownika, string login_p, 
											string haslo_p, string stanowisko_p, string adres_p, string telefon_p)
{
	idPracownika = id_pracownika;
	imiePracownika = imie_pracownika;
	nazwiskoPracownika = nazwisko_pracownika;
	login = login_p;
	haslo = haslo_p;
	stanowisko = stanowisko_p;
	adres = adres_p;
	telefon = telefon_p;
}
ZalogowanyPracownik::~ZalogowanyPracownik()
{
}

ZalogowanyPracownik::ZalogowanyPracownik(string idPracownika_a, string imiePracownika_a, string nazwiskoPracownika_a)
{
	idPracownika = idPracownika_a;
	imiePracownika = imiePracownika_a;
	nazwiskoPracownika = nazwiskoPracownika_a;
}
int ZalogowanyPracownik::wyswietl()
{
	cout << "***************************************************\n" << endl;
	cout << "Dane uzytkownika : " << endl;
	cout << "\t ID: " << idPracownika << endl;
	cout << "\t Imie: " << imiePracownika<<endl;
	cout << "\t Nazwisko: " << nazwiskoPracownika << endl;
	cout << "\t Login: " << login << endl;
	cout << "\t Stanowisko: " << stanowisko << endl;
	cout << "\t Adres: " << adres << endl;
	cout << "\t Telefon: " << telefon <<"\n" <<endl;
	cout << "***************************************************\n" << endl;
	return 0;
}
