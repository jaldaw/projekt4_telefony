#include "pch.h"
#include <mysql.h>
#include <iostream>
#include <sstream>
#include "Osoba.h"
#include <utility>
#include "Polaczenie.h"
using namespace std;

MYSQL *conn;
Osoba zalogowany;
int zmiennastatystyczna1=0;
int zmiennastatystyczna2=0;
int zmiennastatystyczna3 = 0;
int zmiennastatystyczna4 = 0;
//funkcje
inline void czyszczenie_ekranu();
int logowanie();
int sprawdz_liczbe(string liczba, char znak_specjalny);
int sprawdz_liczbe(int liczba1, int liczba2);
int sprawdz_liczbe(string liczba1, int dlugoscmin, int dlugoscmax);
int sprawdz_slowo(string slowo, int duze_litery);
int sprawdz_slowo(string slowo, int duze_litery, char znak_specjalny);
int sprawdz_slowo(string slowo);
int poprawnosc_adres(string zmienna);
int sprawdz_date(string data);
vector<Osoba> wczytaj_wszystkie_osoby();
//MAIN
int main()
{
	int errorcin = 0;
	//Laczenie z baza danych
	cout << "Conecting to the database.." << endl;

	conn = mysql_init(0);
	conn = mysql_real_connect(conn, "localhost", "root", "", "historiapolaczen", 3306, NULL, 0);
	//POLACZONO Z BAZA DANYCH
	if (conn)	//database connected
	{
		cout << "database has been connected" << endl;
		system("cls");
		//WIZYTOWKA PROJEKTU
		{
			cout << "\n**********************************************************************\n" << endl;
			cout << "*\t Student:    Dawid Jalowski" << endl;
			cout << "*\t Grupa:      I7Y2S1" << endl;
			cout << "*\t Projekt:    4" << endl;
			cout << "*\t Temat:      Historia polaczen" << endl;
			cout << "\n**********************************************************************\n" << endl;
			czyszczenie_ekranu();
		}
		int logowanie_status = 0;
		int opcja0 = 0;
		do
		{
			if (errorcin)
			{
				cin.clear();
				cin.ignore(1, '\n');
			}
			system("cls");
			cout << "HISTORIA POLACZEN" << endl;
			if (logowanie_status == 0)	//nie ma zalogowanego uzytkownika
			{
				logowanie_status = logowanie();	//zwroci 1 jesli logowanie powiedzie sie
				//AUTORYZACJA -> BLEDNE DANE
				if (logowanie_status == 0)
				{
					char zgoda;
					cout << "Wystapil blad logowania. Czy chcesz powtorzyc operacje? (y/n)" << endl;
					cin >> zgoda;
					if (zgoda == 'y')
						opcja0 = 1;
					else
						opcja0 = 0;

				}
				else opcja0 = 1;
			}
				//AUTORYZACJA PRZEBIEGLA POPRAWNIE
			else
			{
				czyszczenie_ekranu();
				cin.ignore(1000, '\n');
				cin.clear();
				//MAIN MENU
				{
					cout << "->MAIN MENU***********************************************\n" << endl;
					cout << "Wybierz opcje:" << endl;
					cout << "[1] -> Zobacz swoje dane" << endl;
					cout << "[2] -> Wyswietl polaczenia (jako nadawca) uzytkownika" << endl;
					cout << "[3] -> Wyswietl polaczenie (jako odbiorca) uzytkownika" << endl;
					cout << "[4] -> Wyswietl polaczenia uzytkownika(wszystkie)" << endl;
					cout << "[5] -> statystyki uzytkownika" << endl;
					if (zalogowany.stanowisko == 2)
					{
						cout << "[10] ->Wyswietl numery kierunkowe figurujace w bazie" << endl;

					}
					if (zalogowany.stanowisko == 1)
					{
						cout << "PANEL ADMINISTRATORA--------------" << endl;
						cout << "[10] -> MENU numerow kierunkowych" << endl;
						cout << "[11] -> Wyswietl polaczenia danego uzytkownika" << endl;
						cout << "[12] -> Statystyki bazy danych" << endl;
						cout << "[87] -> Dodaj polaczenie dla okreslonego uzytkownika" << endl;
						cout << "[88] -> Dodaj numer telefonu dla danego uzytkownika" << endl;
						cout << "[89] -> Dodaj uzytkownika" << endl;
						cout << "----------------------------------" << endl;
					}
					cout << "[99] -> Wyloguj" << endl;
					cout << "[0] -> Wyjdz z programu\n" << endl;
					cout << "->MAIN MENU***********************************************\n" << endl;
				}
				cin >> opcja0;
				//wyswietl informacje nt. zalogowanego uzytkownika
				switch (opcja0)
				{
					case 1:
					{
						zalogowany.wyswietl();
						czyszczenie_ekranu();
						break;
					}
					case 2:
					{
						zalogowany.polaczenia(1);
						czyszczenie_ekranu();
						break;
					}
					case 3:
					{
						zalogowany.polaczenia(2);
						czyszczenie_ekranu();
						break;
					}
					case 4:
					{
						zalogowany.polaczenia(3);
						czyszczenie_ekranu();
						break;
					}
					//STATYSTYKI UZYTKOWNIKA
					case 5:
					{
						int opcja1 = 0;
						system("cls");
						cout << "[1] - Wyswietl sredni czas rozmowy" << endl;
						cout << "[2] - Procent rozmow miedzynarodowych" << endl;
						cout << "[3] - Zestawienie polaczen przychodzacych i wychodzacych" << endl;
						cout << "[4] - Zestawienie wzgledem sieci operatora" << endl;
						cout << "[0] - Powrot" << endl;
						cin >> opcja1;
						switch (opcja1)
						{
							case 1:
							{
								zalogowany.polaczenia(4);
								czyszczenie_ekranu();
								break;
							}
							case 2:
							{
								zalogowany.polaczenia(5);
								czyszczenie_ekranu();
								break;
							}
							case 3:
							{
								zalogowany.polaczenia(6);
								czyszczenie_ekranu();
								break;
							}
							case 4:
							{
								zalogowany.polaczenia(7);
								czyszczenie_ekranu();
								break;
							}
							case 0:
							{
								break;
							}
						}
						break;
					}
					//NUMERY KIERUNKOWE WYSWIETL /MENU
					case 10:
					{
						vector<NumerKierunkowy> numerykierunkowe;

						stringstream sszapytanie;
						sszapytanie << "SELECT * FROM numer_kierunkowy";
						int qstate = mysql_query(conn, sszapytanie.str().c_str());
						if (!qstate)
						{
							MYSQL_ROW row;
							MYSQL_RES *res;
							res = mysql_store_result(conn);
							while (row = mysql_fetch_row(res))
							{
								NumerKierunkowy temp(row[0], row[1], row[2], row[3]);
								numerykierunkowe.push_back(temp);
							}
						}
						else
						{
							cout << "Blad kwerendy: " << mysql_error(conn) << endl;
						
						}
						if (zalogowany.stanowisko == 2)
							for (int i = 0; i < numerykierunkowe.size(); i++)
								numerykierunkowe[i].wyswietl_Numer_Kierunkowy_Wizytowka();
						else
						{
							//MENU NUMEROW KIERUNKOWYCH dla administratora
							system("cls");
							{
								cout << "[1] -> Wyswietl numery kierunkowe" << endl;
								cout << "[2] -> Dodaj numer kierunkowy" << endl;
							}
							int opcja1;
							cin >> opcja1;
							if (opcja1 == 1)	//wyswietl
							{
								for (int i = 0; i < numerykierunkowe.size(); i++)
									numerykierunkowe[i].wyswietl_Numer_Kierunkowy();
							}
							else if (opcja1 == 2)	//dodaj
							{
								int rezultat = 1;
								string numerKierunkowy, panstwo, minDlugoscTelefonu, maxDlugoscTelefonu;
								cout << "Podaj numer kierunkowy (+<liczba>)" << endl;
								getline(cin >> ws, numerKierunkowy);
								cout << "Podaj panstwo)" << endl;
								getline(cin >> ws, panstwo);
								cout << "Podaj mininimalna dlugosc telefonu" << endl;
								getline(cin >> ws, minDlugoscTelefonu);
								rezultat = sprawdz_liczbe(minDlugoscTelefonu, 1, 2);
								cout << "Podaj maksymalna dlugosc telefonu" << endl;
								getline(cin >> ws, maxDlugoscTelefonu);
								rezultat = sprawdz_liczbe(maxDlugoscTelefonu, 1, 2);
								if (rezultat)
								{
									NumerKierunkowy temp(numerKierunkowy, panstwo, minDlugoscTelefonu, maxDlugoscTelefonu);
									rezultat = temp.sprawdz_Poprawnosc_Danych();
									if (rezultat)
									{
										rezultat = 0;
										//sprawdzamy, czy podanu numer kierunkowy juz sie nie pojawil
										for (int i = 0; i < numerykierunkowe.size(); i++)
										{
											rezultat = numerykierunkowe[i].sprawdz_kierunkowy(numerKierunkowy);
											if (rezultat)
											{
												cout << "Podany numer kierunkowy wystepuje juz w bazie" << endl;
												break;
											}

										}
										if (!rezultat)
										{
											string zgoda;
											cout << "Czy na pewno chcesz dodac ponizszy numer kierunkowy?(y/n)" << endl;
											temp.wyswietl_Numer_Kierunkowy();
											getline(cin >> ws, zgoda);
											if (zgoda[0] == 'y')
												temp.dodaj_Do_Bazy();
											else
												cout << "Operacja zostala przerwana" << endl;
										}
									}
									else
										cout << "Operacja zostala przerwana" << endl;
								}
								else
								{
									cout << "Jedna z podanych liczb byla nieprawidlowa" << endl;
								}
							}
						}
						czyszczenie_ekranu();
						break;
					}
					//wyswietl historie polaczen dla danego uzytkownika
					case 11:
					{
						if (zalogowany.stanowisko == 1)
						{
							int pozycja;
							vector<Osoba>osoby = wczytaj_wszystkie_osoby();
							cout << "Wybierz osobe, ktorej polaczenia chcesz wyswietlic" << endl;
							if (osoby.size() == 0)
							{
								cout << "Nie znaleziono osob w bazie danych" << endl;
								czyszczenie_ekranu();
								break;

							}
							else
							{
								for (int pozycja = 0; pozycja < osoby.size(); pozycja++)
								{
									cout << pozycja + 1<<".\t";
									osoby[pozycja].wyswietl_wizytowke();
								}
								cin >> pozycja;
								pozycja--;
								if (pozycja >= 0 && pozycja < osoby.size())
								{
									osoby[pozycja].polaczenia(3);
									czyszczenie_ekranu();
								}
								else
								{
									cout << "Wybrano zla opcje!" << endl;
									czyszczenie_ekranu();
									break;
								}
							}
							break;
						}
						else
						{
							cout << "Wybrano zla opcje" << endl;
							break;
						}
					}
					//STATYSTYKI ADMINISTRATORA
					case 12:
					{
						if (zalogowany.stanowisko == 1)
						{
							vector<Osoba> uzytkownicy = wczytaj_wszystkie_osoby();
							int opcja1 = 0;
							zmiennastatystyczna1 = 0;
							zmiennastatystyczna2 = 0;
							zmiennastatystyczna3 = 0;
							zmiennastatystyczna4 = 0;

							system("cls");
							cout << "[1] - Sredni czas rozmowy na uzytkownika (srednia srednich uzytkownikow)" << endl;
							cout << "[2] - Sredni czas rozmowy na uzytkownika (srednia srednich uzytkownikow) z uwzglednieniem plci" << endl;
							cout << "[3] - Sredni czas rozmowy na polaczenie" << endl;
							cout << "[4] - Sredni czas rozmowy na polaczenie z uwzglednieniem plci" << endl;
							cout << "[5] - Stosunek [%] uzytkownikow wzgledem plci" << endl;
							cout << "[6] - Stosunek [%] ilosci zarejestrowanych numerow telefonow pod wzgledem plci" << endl;
							cout << "[7] - Stosunek [%] zarejestrowanych numerow telefonow pod wzgledem panstwa" << endl;
							cout << "[8] - Stosunek mowiacy, ile osob przypada na 1 administratora" << endl;
							cout << "[0] - Powrot" << endl;
							cin >> opcja1;
							switch (opcja1)
							{
							case 1:
							{
								for (int i = 0; i < uzytkownicy.size(); i++)
								{
									uzytkownicy[i].polaczenia(8);
								}
								cout << "Srednia wynosi " << int(round(double(zmiennastatystyczna1) / double(zmiennastatystyczna2)))<<" minut.";
								czyszczenie_ekranu();
								break;
							}
							case 2:
							{
								for (int i = 0; i < uzytkownicy.size(); i++)
								{
									uzytkownicy[i].polaczenia(9);
								}
								cout << "Srednia kobiet wynosi " << int(round(double(zmiennastatystyczna1) / double(zmiennastatystyczna2))) << " minut."<<endl;
								cout << "Srednia mezczyzn wynosi " << int(round(double(zmiennastatystyczna3) / double(zmiennastatystyczna4))) << " minut."<<endl;
								czyszczenie_ekranu();
								break;
							}
							case 3:
							{
								for (int i = 0; i < uzytkownicy.size(); i++)
								{
									uzytkownicy[i].polaczenia(10);
								}
								cout << "Srednia wynosi " << int(round(double(zmiennastatystyczna1) / double(zmiennastatystyczna2))) << " minut.";
								czyszczenie_ekranu();
								break;
							}
							case 4:
							{
								for (int i = 0; i < uzytkownicy.size(); i++)
								{
									uzytkownicy[i].polaczenia(11);
								}
								cout << "Srednia kobiet wynosi " << int(round(double(zmiennastatystyczna1) / double(zmiennastatystyczna2))) << " minut." << endl;
								cout << "Srednia mezczyzn wynosi " << int(round(double(zmiennastatystyczna3) / double(zmiennastatystyczna4))) << " minut." << endl;
								czyszczenie_ekranu();
								break;
							}
							case 5:
							{
								int ilosc_kobiet=0;
								for (int i = 0; i < uzytkownicy.size(); i++)
								{
									ilosc_kobiet += uzytkownicy[i].plec_uzytkownika();

								}
								ilosc_kobiet = int(round(double(ilosc_kobiet) / double(uzytkownicy.size()) * 100));
								cout << "Kobiety stanowia " << ilosc_kobiet << " % zarejestrowanych uzytkownikow, a mezczyzni " << 100 - ilosc_kobiet << " %." << endl;;
								czyszczenie_ekranu();
								break;
							}
							case 6:
							{
								int ilosc_kobiet=0;
								int ilosc=0;
								for (int i = 0; i < uzytkownicy.size(); i++)
								{
									uzytkownicy[i].ilosc_telefonow_wzgledem_plci(ilosc_kobiet, ilosc);
								}
								cout << ilosc_kobiet << " " << ilosc << endl;
								ilosc_kobiet = int(round(double(ilosc_kobiet) / double(ilosc) * 100));
								cout << ilosc_kobiet << " % telefonow nalezy do kobiet, zas mezczyzni " << 100 - ilosc_kobiet << "%. " << endl;
								czyszczenie_ekranu();
								break;
							}
							case 7:
							{

								vector<pair<string,int>> numer;			//paraodwzierciedla numer kierunkowy oraz liczbe wystapien
								int suma = 0;
								for (int i = 0; i < uzytkownicy.size(); i++)
								{
									uzytkownicy[i].liczba_kierunkowych(numer);
								}
								for (int k = 0; k < numer.size(); k++)
								{
									suma += numer[k].second;
								}
								int suma2 = 0;
								for (int k = 0; k < numer.size(); k++)
								{

									//if (k != numer.size() - 1)
									//{
										cout << int(round(double(numer[k].second) / double(suma) * 100)) << " % zarejestrowanych telefonow pochodzi z " << numer[k].first << endl;
										suma2 += int(round(double(numer[k].second) / double(suma) * 100));

									//}
									//else
									//	cout << 100-suma2 << " % zarejestrowanych telefonow pochodzi z " << numer[k].first << endl;
								
								}
								czyszczenie_ekranu();
								break;

							}
							case 8:
							{
								int ilosc_adm = 0, ilosc_us = 0;
								for (int i = 0; i < uzytkownicy.size(); i++)
								{

									if (uzytkownicy[i].stanowisko == 1) ilosc_adm++;
									if (uzytkownicy[i].stanowisko == 2) ilosc_us++;

								}
								cout << "Na jednego administratora przypada ok." << round(ilosc_us / ilosc_adm) << " uzytkownikow" << endl;
								czyszczenie_ekranu();
							}
							case 0:
							{
								break;
							}
							}
							break;
						}
						else
						{
							cout << "Wybrano niewlasciwa opcje" << endl;
							break;
						}
					}
					//dodaj polaczenie
					case 87:
					{
						if (zalogowany.stanowisko == 1)
						{
							int id = 0;
							vector<Osoba> uzytkownicy = wczytaj_wszystkie_osoby();
							string numerkierunkowynadawcy, numernadawcy;
							cout << "Podaj numer kierunkowy nadawcy" << endl;
							getline(cin>>ws, numerkierunkowynadawcy);
							cout << "Podaj numer nadawcy" << endl;
							getline(cin >> ws, numernadawcy);
							for (int i = 0; i < uzytkownicy.size(); i++)
							{
								id=uzytkownicy[i].wyszukaj_id_uzytkownika(numerkierunkowynadawcy, numernadawcy);
								if (id != 0)
									break;
							}
							if (id == 0)
							{
								cout << "Podano numer, ktory nie nalezy do sieci" << endl;
								czyszczenie_ekranu();
								break;
							}
							else
							{
								vector<NumerKierunkowy> numerykierunkowe;
								stringstream sszapytanie;
								sszapytanie << "SELECT * FROM numer_kierunkowy";
								int qstate = mysql_query(conn, sszapytanie.str().c_str());
								if (!qstate)
								{
									MYSQL_ROW row;
									MYSQL_RES *res;
									res = mysql_store_result(conn);
									while (row = mysql_fetch_row(res))
									{
										NumerKierunkowy temp(row[0], row[1], row[2], row[3]);
										numerykierunkowe.push_back(temp);
									}
								}
								else
								{
									cout << "Blad kwerendy: " << mysql_error(conn) << endl;
									system("PAUSE");
									break;

								}
								cout << "Podaj numer kierunkowy odbiorcy" << endl;
								string numerkierunkowy;
								int rezultat = 0, i;
								getline(cin >> ws, numerkierunkowy);
								for (i = 0; i < numerykierunkowe.size(); i++)
								{
									if (rezultat = numerykierunkowe[i].sprawdz_kierunkowy(numerkierunkowy))
									{
										break;
									}
								}
								string numer_telefonu;

								if (!rezultat)
								{
									cout << "Wprowadz nume telefonu odbiorcy" << endl;
									getline(cin >> ws, numer_telefonu);
									string rozpoczecie_rozmowy, zakonczenie_rozmowy;
									int rezultat = 1;
									Telefon tel(0, numer_telefonu, 5, 20, numerkierunkowy);
									rezultat = tel.sprawdz_dlugosc();
									if (!rezultat)
									{
										cout << "Wielkosc numeru jest wieksza od tej przewidywanej" << endl;
										czyszczenie_ekranu();
										break;
									}
									cout << "Podaj czas rozpoczecia rozmowy RRRR:MM:DD GG:MM:SS" << endl;
									getline(cin >> ws, rozpoczecie_rozmowy);
									rezultat = sprawdz_date(rozpoczecie_rozmowy);
									if (!rezultat)
									{
										cout << "Zle wpisano dane!" << endl;
										czyszczenie_ekranu();
										break;
									}
									cout << "Podaj czas zakonczenia rozmowy RRRR:MM:DD GG:MM:SS" << endl;
									getline(cin >> ws, zakonczenie_rozmowy);
									rezultat = sprawdz_date(zakonczenie_rozmowy);
									if (!rezultat)
									{
										cout << "Zle wpisano dane!" << endl;
										czyszczenie_ekranu();
										break;
									}
									cout << "Tworzymy polaczenie..." << endl;
									Polaczenie temp(rozpoczecie_rozmowy, zakonczenie_rozmowy, id, tel);
									czyszczenie_ekranu();
								}
								else
								{
									cout << "Wprowadz numer telefonu odbiorcy" << endl;
									getline(cin >> ws, numer_telefonu);
									string rozpoczecie_rozmowy, zakonczenie_rozmowy;
									int rezultat = 1;
									Telefon tel(0, numer_telefonu, numerykierunkowe[i].minDlugoscTelefonu, numerykierunkowe[i].maxDlugoscTelefonu, numerkierunkowy);
									rezultat = tel.sprawdz_dlugosc();
									if (!rezultat)
									{
										cout << "Wielkosc numeru jest wieksza od tej przewidywanej" << endl;
										czyszczenie_ekranu();
										break;
									}
									cout << "Podaj czas rozpoczecia rozmowy RRRR-MM-DD GG:MM:SS" << endl;
									getline(cin >> ws, rozpoczecie_rozmowy);
									rezultat = sprawdz_date(rozpoczecie_rozmowy);
									if (!rezultat)
									{
										cout << "Zle wpisano dane!" << endl;
										system("PAUSE");
										break;
									}
									cout << "Podaj czas zakonczenia rozmowy RRRR-MM-DD GG:MM:SS" << endl;
									getline(cin >> ws, zakonczenie_rozmowy);
									rezultat = sprawdz_date(zakonczenie_rozmowy);
									if (!rezultat)
									{
										cout << "Zle wpisano dane!" << endl;
										system("PAUSE");
										break;
									}
									cout << "Tworzymy polaczenie..." << endl;
									Polaczenie temp(rozpoczecie_rozmowy, zakonczenie_rozmowy, id, tel);
									system("PAUSE");
									czyszczenie_ekranu();
								}

							}

							break;
						
						}
						else
						{
							cout << "Wybrano niewlasciwa opcje" << endl;
							czyszczenie_ekranu();
							break;
						}
					}
					//dodawanie telefonu do uzytkownika
					case 88:
					{
						if (zalogowany.stanowisko == 1)
						{
							string login;
							cout << "Podaj login uzytkownika" << endl;
							getline(cin >> ws, login);
							if (!zalogowany.sprawdz_login(login))
							{
								cout << "Nie mozna dodawac sobie numerow telefonu!" << endl;
								czyszczenie_ekranu();
								break;
							}
							Osoba temp=[](string login)->Osoba
							{
								stringstream sszapytanie;
								sszapytanie << "SELECT * FROM osoba WHERE LOGIN = '" << login << "';";
								int qstate = mysql_query(conn, sszapytanie.str().c_str());
								if (!qstate)
								{
									MYSQL_RES *res;
									MYSQL_ROW row;
									res = mysql_store_result(conn);
									row = mysql_fetch_row(res);
									if (row == NULL)
									{
										cout << "Nie ma uzytkownika o podanym loginie" << endl;
										return Osoba();
									}
									else
									{
										return Osoba(row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8]);
									}

								}
								else
								{
									cout << "Blad zapytania: " << mysql_error(conn) << endl;
									return Osoba();

								}
							}(login);
							if (temp.stanowisko == 0)
							{
								cout << "Brak osoby w bazie" << endl;
								system("PAUSE");
								break;
							}
							else
							{
								cout << "Osoba o podanym loginie to:" << endl;
								temp.wyswietl();
								string zgoda;
								cout << "Czy chcesz dodac uzytkownikowi nowy numer telefonu?(y/n)" << endl;
								getline(cin >> ws, zgoda);
								if (zgoda[0] == 'y')
								{
									vector<NumerKierunkowy> numerykierunkowe;
									stringstream sszapytanie;
									sszapytanie << "SELECT * FROM numer_kierunkowy";
									int qstate = mysql_query(conn, sszapytanie.str().c_str());
									if (!qstate)
									{
										MYSQL_ROW row;
										MYSQL_RES *res;
										res = mysql_store_result(conn);
										while (row = mysql_fetch_row(res))
										{
											NumerKierunkowy temp(row[0], row[1], row[2], row[3]);
											numerykierunkowe.push_back(temp);
										}
									}
									else
									{
										cout << "Blad kwerendy: " << mysql_error(conn) << endl;
										system("PAUSE");
										break;

									}
									cout << "Podaj numer kierunkowy, ktorego dotyczy numer telefonu" << endl;
									string numerkierunkowy;
									int rezultat=0, i;
									getline(cin >> ws, numerkierunkowy);
									for (i = 0; i < numerykierunkowe.size(); i++)
									{
										if (rezultat=numerykierunkowe[i].sprawdz_kierunkowy(numerkierunkowy))
										{

											break;
										}
									}
									if (!rezultat)
									{
										cout << "W bazie nie ma takiego numeru kierunkowego. Operacjca zostala przerwana" << endl;
										czyszczenie_ekranu();
										break;
									}
									else
									{
										string zgoda;
										cout << "Czy to jest to panstwo ktorego oczekiwales? (y/n)" << endl;
										numerykierunkowe[i].wyswietl_Numer_Kierunkowy_Wizytowka();
										getline(cin >> ws, zgoda);
										if (zgoda[0] == 'y')
										{
											string numer_telefonu;
											cout << "Wprowadz nowy numer telefonu" << endl;
											getline(cin >> ws, numer_telefonu);

												Telefon tel(0, numer_telefonu, numerykierunkowe[i], temp.idOsoby);
												system("PAUSE");


										}
										else
										{
											cout << "Operacja zostala przerwana" << endl;
											system("PAUSE");
										}
									}

								}
								else
								{
									cout << "Operacja zostala przerwana" << endl;
									system("PAUSE");
								}
							}
							break;
						}
						else
						{
							cout << "Wybrano zla opcje" << endl;
							system("PAUSE");
							break;
						}
					}
					//dodawanie nowego uzytkownika przez administratora
					case 89:
					{
						if (zalogowany.stanowisko == 1)
						{
							string imie, nazwisko, adres, plec, PESEL, login, haslo;
							cout << "Podaj imie" << endl;
							getline(cin >> ws, imie);
							if (!sprawdz_slowo(imie, 1))
							{
								cout << "Imie zostalo wprowadzone w sposob niewlasciwy" << endl;
								system("PAUSE");
								break;
							}
							cout << "Podaj nazwisko" << endl;
							getline(cin >> ws, nazwisko);
							if (!sprawdz_slowo(nazwisko, 1, '-'))
							{
								cout << "Nazwisko zostalo wprowadzone w sposob niewlasciwy" << endl;
								system("PAUSE");
								break;
							}
							cout << "Podaj adres" << endl;
							getline(cin >> ws, adres);
							if (!poprawnosc_adres(adres))
							{
								cout << "Adres zostal wprowadzony w sposob nieprawidlowy" << endl;
								system("PAUSE");
								break;
							}
							cout << "Podaj numer PESEL" << endl;
							getline(cin >> ws, PESEL);
							if (!sprawdz_liczbe(PESEL, 11, 11))
							{
								cout << "Pesel zostal sprowadzony w sposob niewlasciwy" << endl;
								system("PAUSE");
								break;
							}
							cout << "Podaj login" << endl;
							getline(cin >> ws, login);
							if (!sprawdz_slowo(login))
							{
								cout << "Wprowadzono login w sposob niewlasciwy" << endl;
								system("PAUSE");
								break;
							}//LAMBDA
							int rezultat = [](string login)->int
							{
								stringstream sszapytanie;
								sszapytanie << "SELECT login FROM osoba WHERE LOGIN ='" << login << "';";
								int qstate = mysql_query(conn, sszapytanie.str().c_str());
								if (!qstate)
								{
									MYSQL_ROW row;
									MYSQL_RES *res;
									res = mysql_store_result(conn);
									if ((row = mysql_fetch_row(res)) == NULL)
									{
										return 1;
									}
									else
									{
										cout << "Podany adres login niestety jest juz zajety" << endl;
										system("PAUSE");
										return 0;
									}
								}
								else
								{
									cout << "Blad kwerendy: " << mysql_error(conn)<<endl;
									czyszczenie_ekranu();
								}
							}(login);
							if (!rezultat) break;
							cout << "Podaj haslo" << endl;
							getline(cin >> ws, haslo);
							if (!sprawdz_slowo(haslo))
							{
								cout << "W sposob niewlasciwy wprowadzono haslo" << endl;
								system("PAUSE");
								break;
							}
							int liczba = PESEL[9];
							cout << ((liczba % 2 == 0) ? "K" : "M");
							cout << liczba << endl;
							char stanowisko;
							cout << "Czy dana osoba to administrator (y / n)" << endl;
							cin >> stanowisko;
							string stanowisko_s;
							if (stanowisko == 'y')
								stanowisko_s = "ADM";
							else
							stanowisko_s = "USR";
								Osoba temp("0", imie, nazwisko, adres,((liczba%2==0) ? "K": "M") , stanowisko_s, PESEL, login, haslo);
							cout << "Czy na pewno chcesz dodac ponizsza osobe?(y/n)" << endl;
							temp.wyswietl();
							string zgoda;
							getline(cin >> ws, zgoda);
							if (zgoda[0] == 'y')
								temp.dodaj_Do_Bazy(stanowisko_s);
							system("PAUSE");
							break;//adfdafad
						}
						else
						{
							cout << "niewlasciwa opcja" << endl;
							system("PAUSE");
							break;
						}
					}
					//WYLOGOWANIE
					case 99:
					{
						logowanie_status = 0;
					}
					case 0:		//wyjscie z programu
						break;
					//NIEPRAWIDLOWA OPCJA
					default:
					{
						cout << "Wprowadzono nieprawidlowa opcje!" << endl;
						opcja0 = 1;
						errorcin = 1;
						czyszczenie_ekranu();
					}
				}
			}
			
		} while (opcja0);
	}
	//BLAD -> POLACZENIE Z BAZA DANYCH
	else
	{
		cout << "ERROR -> Cannot connect with database" << endl;
		czyszczenie_ekranu();
	}
}
//FUNKCJE
inline void czyszczenie_ekranu()
{
	system("PAUSE");
	system("cls");
}
int logowanie()
{
	string login, haslo;
	stringstream sszapytanie;
	cout << "LOGOWANIE " << endl;
	cout << "\tPodaj login: ";
	cin >> login;
	cout << "\tPodaj haslo: ";
	cin >> haslo;
	sszapytanie << "SELECT * FROM osoba WHERE LOGIN = '" << login << "' AND HASLO = '" << haslo << "';";
	int qstate = mysql_query(conn, sszapytanie.str().c_str());
	if (!qstate)
	{
		MYSQL_RES *res;
		MYSQL_ROW row;
		res = mysql_store_result(conn);
		row = mysql_fetch_row(res);
		if (row == NULL)
		{
			system("cls");
			cout << "Wprowadzono ze dane logowania." << endl;
			czyszczenie_ekranu();
			return 0;
		}
		else
		{
			system("cls");
			zalogowany = Osoba(row[0], row[1], row[2],row[3],row[4],row[5], row[6], row[7], row[8]);
			cout << "Pomyslnie dokonano autoryzacji" << endl;
			system("PAUSE");
		}

	}
	else
	{
		cout << "Blad zapytania: " << mysql_error(conn) << endl;
		return 0;

	}
}

int sprawdz_liczbe(string liczba, char znak_specjalny)
{
	for (int i = 0; i < liczba.length(); i++)
	{
		if ((liczba[i] >= '0'&&liczba[i] <= '9') || liczba[i] == znak_specjalny)
			continue;
		else return 0;
	}
	return 1;
}
int sprawdz_liczbe(int liczba1, int liczba2)
{
	if ((liczba1 > liczba2) || liczba1<5||liczba2>20)
		return 0;
	else return 1;
}
int sprawdz_liczbe(string liczba,int dlugoscmin, int dlugoscmax)
{
	if (liczba.length() < dlugoscmin || liczba.length() > dlugoscmax)
		return 0;
	for (int i = 0; i < liczba.length(); i++)
	{
		if (liczba[i] >= '0'&&liczba[i] <= '9')
			continue;
		else return 0;
	}
	return 1;
}
int sprawdz_slowo(string slowo, int duze_litery)
{
	for (int i = 0; i < slowo.length(); i++)
	{
		if (slowo[i] >= 'a'&&slowo[i] <= 'z')
			continue;
		else if (slowo[i] >= 'A' && slowo[i] <= 'Z' && duze_litery == 1)
			continue;
		else return 0;
	}
	return 1;
}
int sprawdz_slowo(string slowo)
{
	for (int i = 0; i < slowo.length(); i++)
	{
		if ((slowo[i] >= 'a'&&slowo[i] <= 'z')||(slowo[i]>='1' && slowo[i] <='9'))
			continue;
		else return 0;
	}
	return 1;
}
int sprawdz_slowo(string slowo, int duze_litery, char znak_specjalny)
{
	for (int i = 0; i < slowo.length(); i++)
	{
		if ((slowo[i] >= 'a'&&slowo[i] <= 'z') || slowo[i] == znak_specjalny)
			continue;
		else if (slowo[i] >= 'A' && slowo[i] <= 'Z' && duze_litery == 1)
			continue;
		else return 0;
	}
	return 1;
}
int poprawnosc_adres(string zmienna)
{
	//sprawdzam poprawność wprowadzonego adresu
	int flaga_ulica = 0;
	int flaga_numer = 0;
	int flaga_numer_domu = 0;
	if (zmienna.length() < 8)
	{
		cout << "Wprowadzony adres jest za krótki" << endl;
	}
	for (int i = 0; i < zmienna.length(); i++)
	{
		if ((zmienna[i] >= 'a' && zmienna[i] <= 'z') || (zmienna[i] >= 'A' && zmienna[i] <= 'Z') || zmienna[i] == ' ')
		{
			if (flaga_ulica == 1)
				flaga_numer = 1;
			continue;
		}
		else if (zmienna[i] == '.')
		{
			if (i == 4)
			{
				cout << "Nie wprowadzono nazwy miasta" << endl;
				return 0;
			}
			if (i + 2 >= zmienna.length())
			{
				cout << " Nie wprowadzono dokładnego adresu zamieszkania" << endl;
				return 0;
			}
			if (flaga_ulica == 1)
			{
				cout << "W sposob nieprawidlowy wpisano adres" << endl;
				return 0;
			}
			if (zmienna[i - 1] == 'l' && zmienna[i - 2] == 'u' && zmienna[i - 3] == ' ')
			{
				flaga_ulica = 1;
			}
			else
			{
				cout << "Nieprawidłowy format adresu" << endl;
				return 0;
			}
		}
		else if (zmienna[i] >= '0' && zmienna[i] <= '9')
		{
			if (flaga_ulica == 0)
			{
				cout << "Nie wpisano poprawnie adresu" << endl;
				return 0;
			}
			if (flaga_numer == 0)
			{
				cout << "Nie wprowadzono nazwy ulicy " << endl;
				return 0;
			}
			flaga_numer_domu = 1;
		}
		else if (zmienna[i] == '/')
		{
			if (flaga_numer_domu == 0)
			{
				cout << "Wprowadzono adres w zly sposob" << endl;
			}
		}
	}
	if (flaga_numer_domu == 1)
		return 1;
	else return 0;
}
vector<Osoba> wczytaj_wszystkie_osoby()
{
	vector<Osoba> zwracany;
	stringstream sszapytanie;
	sszapytanie << "SELECT * FROM osoba";
	int qstate = mysql_query(conn, sszapytanie.str().c_str());
	if (!qstate)
	{
		MYSQL_ROW row;
		MYSQL_RES *res;
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			Osoba temp(row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8]);
			zwracany.push_back(temp);
		}
	}
	else
	{
		cout << "Blad zapytania: " << mysql_error(conn) << endl;
	}
	return zwracany;
}
int sprawdz_date(string data)
{
	for (int i = 0; i < data.length(); i++)
	{
		if (((i >= 0 && i < 4) || i == 5 || i == 6 || i == 8 || i == 9 || i == 11 || i == 12 || i == 14 || i == 15 || i == 17 || i == 18) && (data[i] >= '0' && data[i] <= '9'))
			continue;
		else if ((i == 4 || i == 7) && data[i] == '-')
			continue;
		else if (i == 10 && data[i] == ' ')
			continue;
		else if ((i == 16 || i == 13) && data[i] == ':')
			continue;
		else
		{
			cout << "zwracam blad na i =" << i << endl;
			return 0;
		}
	}
	return 1;
}