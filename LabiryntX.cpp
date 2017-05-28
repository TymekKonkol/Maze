#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
//custom made library by my prof, especially for this project, it's probably just their version of conio.h
#include "conio2.h"
#include <time.h>
#include <stdio.h>
#include <windows.h>

using namespace std;

#define GORA 0
#define PRAWO 1
#define DOL 2
#define LEWO 3


//struktury--------------------------

struct gracz_1
{
	int x;
	int y;
	char strzalka;
}gracz;

struct drzwi_1
{
	int x[10];
	int y[10];
	char znak[10];
}drzwia;

struct wspolrzedneOgol
{
	int xpocz;
	int ypocz;
	int xkonc;
	int ykonc;
	int xdrzwi;
	int ydrzwi;

}wspolrzedne;

struct dane_Rys
{
	int zmian;
	int bec;
	int wysokosc;
	int szerokoscP;
	int szerokoscL;
	int gdzieRozpoczac;
	int nrsciany;
}daneRys;

struct zmienne
{
	int poczatekBloku;
	int dlugoscSciany;
	int przesuniecieSciany;
	int a;
	int b;
	int szerokoscBloku;
	int usuniecieTpoziomu;
	int zmiennaNaWysokosc;
	int zmiennaNaSzerokosc;

	int pozycjax;
	int pozycjay;
	int pozycjayL;
	int pozycjaxL;
	int d1;
	int d2;
}zmienneRys;


//----------------------------------

void pokazPomoc()
{
	clrscr();
	textcolor(7);
	gotoxy(25, 1);
	cputs("q = wyjscie");
	gotoxy(25, 2);
	cputs("strzalki = poruszanie");
	gotoxy(25, 3);
	cputs("spacja = zmiana koloru");
	gotoxy(25, 4);
	cputs("d = otwieranie/zamykanie drzwi");
	gotoxy(25, 5);
	cputs("i = wczytanie domyslnego labiryntu");
	gotoxy(25, 6);
	cputs("e = wejscie do edytora(najpierw musisz wczytac jakis labirynt aby wejsc do edytora)");
	gotoxy(25, 7);
	cputs("o = odczytanie labiryntu z podanego pliku");
	gotoxy(25, 8);
	cputs("s = zapisanie do pliku z podana nazwa");
	gotoxy(25, 9);
	cputs("r = restart");
	gotoxy(25, 10);
	cputs("a = aktywacja animacji ruchu");
	getch();
}


void wskarzKierunek(char *kierun, int kierunek)
{
	if (kierunek == GORA)
		*kierun = '^';
	else if (kierunek == PRAWO)
		*kierun = '>';
	else if (kierunek == DOL)
		*kierun = 'v';
	else if (kierunek == LEWO)
		*kierun = '<';
}

void drzwi(int kierunek, char znak)
{
	for (int i = 0; drzwia.x[i] != 0; i++)
		if (kierunek == GORA && drzwia.x[i] == (gracz.x - 1) && drzwia.y[i] == (gracz.y))
			drzwia.znak[i] = znak;
		else if (kierunek == PRAWO && drzwia.y[i] == (gracz.y + 1) && drzwia.x[i] == (gracz.x))
			drzwia.znak[i] = znak;
		else if (kierunek == DOL && drzwia.x[i] == (gracz.x + 1) && drzwia.y[i] == (gracz.y))
			drzwia.znak[i] = znak;
		else if (kierunek == LEWO && drzwia.y[i] == (gracz.y - 1) && drzwia.x[i] == (gracz.x))
			drzwia.znak[i] = znak;
}

void *wczytajLab(char *nazwaPliku, char labirynt[][23], gracz_1 *gracz, wspolrzedneOgol *wspolrzedne, drzwi_1 *drzwia,int *czyRysowac)
{
	FILE *plik;
	plik = fopen(nazwaPliku, "r");
	if (plik == NULL)
	{
		*czyRysowac = 0;
		textcolor(LIGHTRED);
		gotoxy(26, 13);
		cout << "NIE UDALO SIE OTWORZYC PLIKU";
		gotoxy(26, 14);
		cout << "Na pewno jest w folderze z gra?";
	}
	else
	{
		*czyRysowac = 1;
		char znak = ' ';
		int i = 1, j = 1, t=0;
		while (znak != EOF)
		{
			
			j = 1;
			do
			{
				znak = fgetc(plik);
				if (znak == 'e')
				{
					labirynt[i][j] = 'E';
					(*gracz).x = i;
					(*gracz).y = j;
					(*wspolrzedne).xpocz = i;
					(*wspolrzedne).ypocz = j;
					(*drzwia).x[t] = i;
					(*drzwia).y[t] = j;
					(*drzwia).znak[t] = 'E';
					t++;
				}
				else if (znak == 'w')
				{
					labirynt[i][j] = 'W';
					(*wspolrzedne).xkonc = i;
					(*wspolrzedne).ykonc = j;
				}
				else if (znak == ' ')
				{
					labirynt[i][j] = ' ';
					
				}
				else if (znak == '#')
				{
					labirynt[i][j] = '#';
					(*drzwia).x[t] = i;
					(*drzwia).y[t] = j;
					(*drzwia).znak[t] = '#';
					t++;
				}
				else
				{
					labirynt[i][j] = ((rand() % ('&' - '!')) + '!');
					if (labirynt[i][j] == '"')
						labirynt[i][j] = '@';
					if (labirynt[i][j] == '#')
						labirynt[i][j] = '[';
				}

				if (znak == EOF)
				{
					break;
				}
				j++;
			} while ((znak) != '\n');
			i++;
		}
		for (int i = 0; i < 23; i++)
		{
			for (int j = 0; j < 23; j++)
			{
				if (i == 0 || i == 21 || j == 0 || j == 21)
				{
					labirynt[i][j] = 'X';
				}
			}
		}
	}
	return 0;
}

void zapiszDoPliku(char labirynt[][23])
{
	char nazwaPliku[20] = {};
	FILE *plik;
	gotoxy(10, 10);
	cout << "Podaj nazwe pliku dla tego labiryntu";
	for (int i = 0; nazwaPliku[i - 1] != 0; i++)
	{
		nazwaPliku[i] = getch();
		if (nazwaPliku[i] == '\r')
			nazwaPliku[i] = 0;
	}
	plik = fopen(nazwaPliku, "w");
	for (int i = 1; i < 21; i++)
	{
		for (int j = 1; j < 21; j++)
		{
			if(labirynt[i][j]=='E')
				fputc('e', plik);
			else if(labirynt[i][j] == 'W')
				fputc('w', plik);
			else
				fputc(labirynt[i][j], plik);
		}
		fputc('\n', plik);
	}
	fclose(plik);
}

void rysujLabirynt(char labirynt[][23])
{
	for (int i = 0; i < 22; i++)
	{
		gotoxy(55, 4 + i);
		for (int j = 0; j < 22; j++)
			cout << labirynt[i][j];

	}
}

void sprawdzCzyNiePuste(char labirynt[][23], int edytorx, int edytory, char edytorZnak)
{	//na gore i w prawo	
	if (labirynt[edytorx - 1][edytory] == ' ' && labirynt[edytorx][edytory + 1] == ' ' && labirynt[edytorx - 1][edytory + 1] == ' ')
	{
		gotoxy(20, 10);
		cout << "STWORZYLES BLEDNE PUSTE POLE 2X2!";
		Sleep(500);
	}	// na gore i w lewo
	else if (labirynt[edytorx - 1][edytory] == ' ' && labirynt[edytorx][edytory - 1] == ' ' && labirynt[edytorx - 1][edytory - 1] == ' ')
	{
		gotoxy(20, 10);
		cout << "STWORZYLES BLEDNE PUSTE POLE 2X2!";
		Sleep(500);
	}// na dol i w lewo
	else if (labirynt[edytorx + 1][edytory] == ' ' && labirynt[edytorx][edytory - 1] == ' ' && labirynt[edytorx + 1][edytory - 1] == ' ')
	{
		gotoxy(20, 10);
		cout << "STWORZYLES BLEDNE PUSTE POLE 2X2!";
		Sleep(500);
	}// na dol i w prawo
	else if (labirynt[edytorx + 1][edytory] == ' ' && labirynt[edytorx][edytory + 1] == ' ' && labirynt[edytorx + 1][edytory + 1] == ' ')
	{
		gotoxy(20, 10);
		cout << "STWORZYLES BLEDNE PUSTE POLE 2X2!";
		Sleep(500);
	}
	else
		labirynt[edytorx][edytory] = edytorZnak;
}

void SprawdzCzyJestWejscie(char labirynt[][23], int edytorx, int edytory, char edytorZnak, char wejWyj)
{
	bool prawda = true;
	for (int i = 0; i < 24; i++)
		for (int j = 0; j < 24; j++)
		{
			if (labirynt[i][j] == wejWyj)
			{
				prawda = false;
				gotoxy(10, 10);
				cout << "JUZ JEST WEJSCIE/WYJSCIE";
				Sleep(500);
			}
		}
	if (prawda == true)
		labirynt[edytorx][edytory] = edytorZnak;
}

void zmieniacz(char labirynt[][23],drzwi_1 *drzwia,gracz_1 *gracz,wspolrzedneOgol *wspolrzedne)
{
	int t = 0;
	for (int i = 0; i < 24; i++)
		for (int j = 0; j < 24; j++)
		{
			if (labirynt[i][j] == '#')
			{
				(*drzwia).x[t] = i;
				(*drzwia).y[t] = j;
				(*drzwia).znak[t] = '#';
				t++;
			}
			if (labirynt[i][j] == 'E')
			{
				labirynt[i][j] = 'E';
				(*gracz).x = i;
				(*gracz).y = j;
				(*wspolrzedne).xpocz = i;
				(*wspolrzedne).ypocz = j;
				(*drzwia).x[t] = i;
				(*drzwia).y[t] = j;
				(*drzwia).znak[t] = 'E';
				t++;				
			}
			if (labirynt[i][j] == 'w')
			{
				labirynt[i][j] = 'W';
				(*wspolrzedne).xkonc = i;
				(*wspolrzedne).ykonc = j;
			}
		}
}

void edytor(char labirynt[][23], drzwi_1 *drzwia, gracz_1 *gracz, wspolrzedneOgol *wspolrzedne)
{
	int edytorx, edytory;
	char edytorZnak;
	gotoxy(10, 1);
	cout << "Podaj nr kolumny, wiersza i znak ktory jest do zmiany";
	gotoxy(10, 2);
	cout << "Kazde po spacji, zakoncz wciskajac enter";
	cin >> edytorx;
	cin >> edytory;
	edytorZnak = getch();
	if (edytorZnak == ' ')
	{																
		sprawdzCzyNiePuste(labirynt, edytorx, edytory, edytorZnak);
	}
	else if (edytorZnak == 'e')
	{
		SprawdzCzyJestWejscie(labirynt, edytorx, edytory, edytorZnak, 'E');
	}
	else if (edytorZnak == 'w')
	{
		SprawdzCzyJestWejscie(labirynt, edytorx, edytory, edytorZnak, 'W');		
	}
	else
		labirynt[edytorx][edytory] = edytorZnak;
	rysujLabirynt(labirynt);
	zmieniacz(labirynt, drzwia, gracz, wspolrzedne);
}

void usunNadPrzejsciami(dane_Rys daneRys, int ktoryBlok, char ktoraStrona)
{
	if (daneRys.nrsciany < 2)
	{
		if (ktoraStrona == 'L')
			for (int i = 1; i < daneRys.gdzieRozpoczac + 4; i++)
			{
				gotoxy(daneRys.szerokoscL + ktoryBlok, 0 + i);
				cout << ' ';
				gotoxy(daneRys.szerokoscL + ktoryBlok, 26 - i);
				cout << ' ';
			}
		if (ktoraStrona == 'P')
			for (int i = 1; i < daneRys.gdzieRozpoczac + 4; i++)
			{
				gotoxy(daneRys.szerokoscP - ktoryBlok, 0 + i);
				cout << ' ';
				gotoxy(daneRys.szerokoscP - ktoryBlok, 26 - i);
				cout << ' ';
			}
	}
	else if (daneRys.nrsciany == 2)
	{
		if (ktoraStrona == 'L')
			for (int i = 1; i < daneRys.gdzieRozpoczac + 3; i++)
			{
				gotoxy(daneRys.szerokoscL + ktoryBlok, 0 + i);
				cout << ' ';
				gotoxy(daneRys.szerokoscL + ktoryBlok, 26 - i);
				cout << ' ';
			}
		if (ktoraStrona == 'P')
			for (int i = 1; i < daneRys.gdzieRozpoczac + 3; i++)
			{
				gotoxy(daneRys.szerokoscP - ktoryBlok, 0 + i);
				cout << ' ';
				gotoxy(daneRys.szerokoscP - ktoryBlok, 26 - i);
				cout << ' ';
			}
	}
	else if (daneRys.nrsciany > 2 && daneRys.nrsciany<6)
	{
		if (ktoraStrona == 'L')
			for (int i = 1; i < daneRys.gdzieRozpoczac + 2; i++)
			{
				gotoxy(daneRys.szerokoscL + ktoryBlok, 0 + i);
				cout << ' ';
				gotoxy(daneRys.szerokoscL + ktoryBlok, 26 - i);
				cout << ' ';
			}
		if (ktoraStrona == 'P')
			for (int i = 1; i < daneRys.gdzieRozpoczac + 2; i++)
			{
				gotoxy(daneRys.szerokoscP - ktoryBlok, 0 + i);
				cout << ' ';
				gotoxy(daneRys.szerokoscP - ktoryBlok, 26 - i);
				cout << ' ';
			}		
	}
	
}

void rysowanieBlokow(char labirynt[][23],gracz_1 gracz, dane_Rys daneRys, zmienne zmienneRys,int animacja, int nrbloku, int dlIteracji, int wys)
{	
	if (labirynt[gracz.x + zmienneRys.pozycjax + (zmienneRys.d1*daneRys.nrsciany)][gracz.y + zmienneRys.pozycjay + (zmienneRys.d2*daneRys.nrsciany)] == ' ' || labirynt[gracz.x + zmienneRys.pozycjax + (zmienneRys.d1*daneRys.nrsciany)][gracz.y + zmienneRys.pozycjay + (zmienneRys.d2*daneRys.nrsciany)] == '_')
	{
		for (int i = 1; i < (daneRys.wysokosc - zmienneRys.zmiennaNaWysokosc); i++)
		{
			textcolor(RED);
			gotoxy(daneRys.szerokoscP - nrbloku, i + daneRys.gdzieRozpoczac + zmienneRys.przesuniecieSciany);
			if (nrbloku == 1)
				cout << '|';
			else
				cout << labirynt[gracz.x + zmienneRys.pozycjax + (zmienneRys.d1*daneRys.nrsciany) + zmienneRys.d1][gracz.y + zmienneRys.pozycjay + (zmienneRys.d2*daneRys.nrsciany) + zmienneRys.d2];
			textcolor(WHITE);
			if (labirynt[gracz.x + zmienneRys.pozycjax + (zmienneRys.d1*daneRys.nrsciany)][gracz.y + zmienneRys.pozycjay + (zmienneRys.d2*daneRys.nrsciany)] == '_')
			{
				if (i < 2)
				{
					gotoxy(daneRys.szerokoscP - nrbloku, i + daneRys.gdzieRozpoczac + zmienneRys.przesuniecieSciany);
					cout << '#';
				}
			}
			if (animacja == 1)
				usunNadPrzejsciami(daneRys, nrbloku, 'P');
		}
	}
	else
	{
		for (int i = 1; i < daneRys.wysokosc - 2*dlIteracji; i++)
		{
			gotoxy(daneRys.szerokoscP - nrbloku + (wys*zmienneRys.usuniecieTpoziomu), wys +i + daneRys.gdzieRozpoczac);
			if (nrbloku == 1)
				cout << '|';
			else
			cout << labirynt[gracz.x + zmienneRys.pozycjax + zmienneRys.a][gracz.y + zmienneRys.pozycjay + zmienneRys.b];
		}

	}
	if (labirynt[gracz.x + zmienneRys.pozycjaxL + (zmienneRys.d1*daneRys.nrsciany)][gracz.y + zmienneRys.pozycjayL + (zmienneRys.d2*daneRys.nrsciany)] == ' ' || labirynt[gracz.x + zmienneRys.pozycjaxL + (zmienneRys.d1*daneRys.nrsciany)][gracz.y + zmienneRys.pozycjayL + (zmienneRys.d2*daneRys.nrsciany)] == '_')
	{
		for (int i = 1; i < (daneRys.wysokosc - zmienneRys.zmiennaNaWysokosc); i++)
		{
			textcolor(RED);
			gotoxy(daneRys.szerokoscL + nrbloku, i + daneRys.gdzieRozpoczac + zmienneRys.przesuniecieSciany);
			if (nrbloku == 1)
				cout << '|';
			else
			cout << labirynt[gracz.x + zmienneRys.pozycjaxL + (zmienneRys.d1*daneRys.nrsciany) + zmienneRys.d1][gracz.y + zmienneRys.pozycjayL + (zmienneRys.d2*daneRys.nrsciany) + zmienneRys.d2];
			textcolor(WHITE);
			if (labirynt[gracz.x + zmienneRys.pozycjaxL + (zmienneRys.d1*daneRys.nrsciany)][gracz.y + zmienneRys.pozycjayL + (zmienneRys.d2*daneRys.nrsciany)] == '_')
			{
				if (i < 2)
				{
					gotoxy(daneRys.szerokoscL + nrbloku, i + daneRys.gdzieRozpoczac + zmienneRys.przesuniecieSciany);
					cout << '#';
				}
			}
			if (animacja == 1)
				usunNadPrzejsciami(daneRys, nrbloku, 'L');
		}
		

	}
	else
	{
		for (int i = 1; i < daneRys.wysokosc - 2* dlIteracji; i++)
		{
			gotoxy(daneRys.szerokoscL + nrbloku -(wys*zmienneRys.usuniecieTpoziomu), wys +i + daneRys.gdzieRozpoczac);
			if (nrbloku == 1)
				cout << '|';
			else
			cout << labirynt[gracz.x + zmienneRys.pozycjaxL + zmienneRys.a][gracz.y + zmienneRys.pozycjayL + zmienneRys.b];
		}
	}
}

void animowaneZamzywanie(dane_Rys daneRys, int nrbloku, int iloscpowt, int gdziePoczatek)
{
	for (int i = 1; i < daneRys.gdzieRozpoczac + gdziePoczatek; i++)
	{
		for (int j = nrbloku; j < iloscpowt; j++)
		{
			gotoxy(daneRys.szerokoscL + j, 0 + i);
			cout << ' ';
			gotoxy(daneRys.szerokoscL + j, 26 - i);
			cout << ' ';
			gotoxy(daneRys.szerokoscP - j, 0 + i);
			cout << ' ';
			gotoxy(daneRys.szerokoscP - j, 26 - i);
			cout << ' ';
		}
	}
}

void zmianaZmiennych(dane_Rys *daneRys, zmienne *zmienneRys, int kierunek)
{
	if ((*daneRys).nrsciany > 1)
	{
		(*zmienneRys).szerokoscBloku = 2;
		(*zmienneRys).usuniecieTpoziomu = 1;
		(*zmienneRys).zmiennaNaWysokosc = 4;
		(*zmienneRys).zmiennaNaSzerokosc = 3;
		(*zmienneRys).poczatekBloku = 3;

		(*zmienneRys).dlugoscSciany = 3;

		(*zmienneRys).przesuniecieSciany = 2;
	}
	if ((*daneRys).nrsciany > 2)
	{
		(*daneRys).gdzieRozpoczac -= 1;
		(*zmienneRys).zmiennaNaWysokosc = 2;
		(*zmienneRys).zmiennaNaSzerokosc -= 1;
		(*zmienneRys).dlugoscSciany = 2;
		(*zmienneRys).przesuniecieSciany = 1;
	}
	if ((*daneRys).nrsciany > 3)
		(*daneRys).gdzieRozpoczac -= 1;
	if ((*daneRys).nrsciany > 5)
	{
		(*zmienneRys).dlugoscSciany = 1;
		(*zmienneRys).zmiennaNaWysokosc = 0;
		(*zmienneRys).przesuniecieSciany = 0;
	}

	switch (kierunek)
	{
	case GORA: (*zmienneRys).a = (*daneRys).zmian, (*zmienneRys).pozycjax = -1, (*zmienneRys).pozycjay = 1, (*zmienneRys).pozycjaxL = -1, (*zmienneRys).pozycjayL = -1, (*zmienneRys).d1 = -1;
		break;
	case PRAWO: (*zmienneRys).b = (*daneRys).bec, (*zmienneRys).pozycjax = 1, (*zmienneRys).pozycjay = 1, (*zmienneRys).pozycjaxL = -1, (*zmienneRys).pozycjayL = 1, (*zmienneRys).d2 = 1;
		break;
	case DOL: (*zmienneRys).a = (*daneRys).bec, (*zmienneRys).pozycjax = 1, (*zmienneRys).pozycjay = -1, (*zmienneRys).pozycjaxL = 1, (*zmienneRys).pozycjayL = 1, (*zmienneRys).d1 = 1;
		break;
	case LEWO: (*zmienneRys).b = (*daneRys).zmian, (*zmienneRys).pozycjax = -1, (*zmienneRys).pozycjay = -1, (*zmienneRys).pozycjaxL = 1, (*zmienneRys).pozycjayL = -1, (*zmienneRys).d2 = -1;
		break;
	default:
		break;
	}

}

void zamazNadHoryzontem(dane_Rys daneRys)
{
	for (int i = 1; i < daneRys.gdzieRozpoczac + 1; i++)
	{
		int a = (50 - daneRys.szerokoscP);
		if (daneRys.nrsciany < 5)
			a = (58 - daneRys.szerokoscP);
		if (daneRys.nrsciany <= 2)
			a = 33;
		for (int j = 1; j < a; j++)
		{
			gotoxy(daneRys.szerokoscL + j, 0 + i);
			cout << ' ';
			gotoxy(daneRys.szerokoscL + j, 26 - i);
			cout << ' ';
			gotoxy(daneRys.szerokoscP - j, 0 + i);
			cout << ' ';
			gotoxy(daneRys.szerokoscP - j, 26 - i);
			cout << ' ';
		}
	}
}

void rysujHoryzont(char labirynt[][23], dane_Rys daneRys,zmienne zmienneRys)
{
	for (int i = 1; i < daneRys.wysokosc; i++)
		for (int j = 1; j < (daneRys.szerokoscP - 25); j++)
		{
			gotoxy(daneRys.szerokoscP - j, i + daneRys.gdzieRozpoczac);
			cout << labirynt[gracz.x + (zmienneRys.d1*daneRys.nrsciany) + zmienneRys.d1][gracz.y + (zmienneRys.d2*daneRys.nrsciany) + zmienneRys.d2];
		}
	for (int i = 1; i < daneRys.wysokosc; i++)
	{
		for (int j = 1; j < (26 - daneRys.szerokoscL); j++)
		{
			gotoxy(daneRys.szerokoscL + j, i + daneRys.gdzieRozpoczac);
			cout << labirynt[gracz.x + (zmienneRys.d1*daneRys.nrsciany) + zmienneRys.d1][gracz.y + (zmienneRys.d2*daneRys.nrsciany) + zmienneRys.d2];
		}
	}
}

//sprawdzanie kolizji

bool czyPrzed(int kierunek, char labirynt[][23], gracz_1 gracz)
{
	bool prawda = true;

	if (kierunek == GORA)
	{
		if (labirynt[gracz.x - 1][gracz.y] != ' ' && labirynt[gracz.x - 1][gracz.y] != '_' && labirynt[gracz.x - 1][gracz.y] != 'E' && labirynt[gracz.x- + 1][gracz.y] != 'W')
		{
			prawda = false;
		}
	}
	else if (kierunek == PRAWO)
	{
		if (labirynt[gracz.x][gracz.y + 1] != ' ' && labirynt[gracz.x][gracz.y + 1] != '_' && labirynt[gracz.x][gracz.y + 1] != 'E' && labirynt[gracz.x][gracz.y + 1] != 'W')
		{
			prawda = false;
		}
	}
	else if (kierunek == DOL)
	{
		if (labirynt[gracz.x + 1][gracz.y] != ' ' && labirynt[gracz.x + 1][gracz.y] != '_' && labirynt[gracz.x + 1][gracz.y] != 'E' && labirynt[gracz.x + 1][gracz.y] != 'W')
		{
			prawda = false;
		}
	}
	else if (kierunek == LEWO)
	{
		if (labirynt[gracz.x][gracz.y - 1] != ' ' && labirynt[gracz.x][gracz.y - 1] != '_' && labirynt[gracz.x][gracz.y - 1] != 'E' && labirynt[gracz.x][gracz.y - 1] != 'W')
		{
			prawda = false;
		}
	}
	return prawda;
}

bool czyZa(int kierunek, char labirynt[][23], gracz_1 gracz)
{
	bool prawda = true;

	if (kierunek == GORA)
	{
		if (labirynt[gracz.x + 1][gracz.y] != ' ' && labirynt[gracz.x + 1][gracz.y] != '_' && labirynt[gracz.x + 1][gracz.y] != 'E' && labirynt[gracz.x + 1][gracz.y] != 'W')
		{
			prawda = false;
		}
	}
	else if (kierunek == PRAWO)
	{
		if (labirynt[gracz.x][gracz.y - 1] != ' ' && labirynt[gracz.x][gracz.y - 1] != '_' && labirynt[gracz.x][gracz.y - 1] != 'E' && labirynt[gracz.x][gracz.y - 1] != 'W')
		{
			prawda = false;
		}
	}
	else if (kierunek == DOL)
	{
		if (labirynt[gracz.x - 1][gracz.y] != ' ' && labirynt[gracz.x - 1][gracz.y] != '_' && labirynt[gracz.x - 1][gracz.y] != 'E' && labirynt[gracz.x - 1][gracz.y] != 'W')
		{
			prawda = false;
		}
	}
	else if (kierunek == LEWO)
	{
		if (labirynt[gracz.x][gracz.y + 1] != ' ' && labirynt[gracz.x][gracz.y + 1] != '_' && labirynt[gracz.x][gracz.y + 1] != 'E' && labirynt[gracz.x][gracz.y + 1] != 'W')
		{
			prawda = false;
		}
	}
	return prawda;
}

char coPrzed(int kierunek, char labirynt[][23],gracz_1 gracz)
{
	char znak = 'x';
	if (kierunek == GORA)
	{
		znak = labirynt[gracz.x - 1][gracz.y];
	}
	else if (kierunek == PRAWO)
	{
		znak = labirynt[gracz.x][gracz.y + 1];
	}
	else if (kierunek == DOL)
	{
		znak = labirynt[gracz.x + 1][gracz.y];
	}
	else if (kierunek == LEWO)
	{
		znak = labirynt[gracz.x][gracz.y - 1];
	}
	return znak;
}

//RYSOWANIE SCIAN CZY DE

void rysujDoTylu(char labirynt[][23],gracz_1 gracz, dane_Rys daneRys, zmienne zmienneRys,int animacja)
{
	// 1 blok sciany			
	rysowanieBlokow(labirynt, gracz, daneRys, zmienneRys, animacja, 1, 0, 0);
	if (animacja == 1)
	{
		animowaneZamzywanie(daneRys, 1, 3, 1);
		Sleep(5);
	}
		// 2 blok				
		rysowanieBlokow(labirynt, gracz, daneRys, zmienneRys, animacja, 2, 0, 0);
		if (animacja == 1)
			Sleep(5);
		// 3 blok				
	if (daneRys.nrsciany < 6)
	{
		rysowanieBlokow(labirynt, gracz, daneRys, zmienneRys, animacja, 3, 1, 1);
		if (animacja == 1)
		{
			animowaneZamzywanie(daneRys, 3, 5, 2);
			Sleep(5);
		}
	}// 4 blok
	if (daneRys.nrsciany < 3)
	{
		rysowanieBlokow(labirynt, gracz, daneRys, zmienneRys, animacja, 4, 1, 1);
		if (animacja == 1)
			Sleep(5);
	}	// 5 blok
	if (daneRys.nrsciany < 2)
	{
		rysowanieBlokow(labirynt, gracz, daneRys, zmienneRys, animacja, 5, 2, 2);
		if (animacja == 1)
		{
			animowaneZamzywanie(daneRys, 5, 6, 3);
			Sleep(5);
		}
	}
}

void rysujDoPrzodu(char labirynt[][23], gracz_1 gracz, dane_Rys daneRys, zmienne zmienneRys, int animacja)
{
	// 5 blok
	if (daneRys.nrsciany < 2)
	{
		rysowanieBlokow(labirynt, gracz, daneRys, zmienneRys, animacja, 5, 2, 2);
		if (animacja == 1)
		{
			animowaneZamzywanie(daneRys, 5, 6, 3);
			Sleep(5);
		}
	}// 4 blok
	if (daneRys.nrsciany < 3)
	{
		rysowanieBlokow(labirynt, gracz, daneRys, zmienneRys, animacja, 4, 1, 1);
		if (animacja == 1)
			Sleep(5);
	}
	if (daneRys.nrsciany < 6)
	{// 3 blok				
		rysowanieBlokow(labirynt, gracz, daneRys, zmienneRys, animacja, 3, 1, 1);
		if (animacja == 1)
		{
			animowaneZamzywanie(daneRys, 3, 5, 2);
			Sleep(5);
		}
	}
	// 2 blok				
	rysowanieBlokow(labirynt, gracz, daneRys, zmienneRys, animacja, 2, 0, 0);
	if (animacja == 1)
		Sleep(5);
	// 1 blok sciany			
	rysowanieBlokow(labirynt, gracz, daneRys, zmienneRys, animacja, 1, 0, 0);
	if (animacja == 1)
	{
		animowaneZamzywanie(daneRys, 1, 3, 1);
		Sleep(5);
	}
}


void rysujScianeREK(char labirynt[][23], gracz_1 gracz, int kierunek, dane_Rys daneRys, int animacja, char zn)
{
	int czyRysowacOtwarteDrzwi = 0;
	zmienneRys.poczatekBloku = 3, zmienneRys.dlugoscSciany = 5, zmienneRys.przesuniecieSciany = 3;
	zmienneRys.a = 0, zmienneRys.b = 0, zmienneRys.szerokoscBloku = 3, zmienneRys.usuniecieTpoziomu = 0, zmienneRys.zmiennaNaWysokosc = 6, zmienneRys.zmiennaNaSzerokosc = 5;

	zmienneRys.pozycjax = 1, zmienneRys.pozycjay = 1, zmienneRys.pozycjayL = 1, zmienneRys.pozycjaxL = 1, zmienneRys.d1 = 0, zmienneRys.d2 = 0;

	textcolor(WHITE);
	if (daneRys.nrsciany == 7)
	{

	}
	else
	{		
		zmianaZmiennych(&daneRys, &zmienneRys,kierunek);
		
		int zmiennaGraczx = gracz.x;
		int zmiennaGraczy = gracz.y;
		
		gracz.x = gracz.x + (zmienneRys.d1*daneRys.nrsciany);
		gracz.y = gracz.y + (zmienneRys.d2*daneRys.nrsciany);
		if ((coPrzed(kierunek, labirynt, gracz) == '_'))
			czyRysowacOtwarteDrzwi = 1;

		if ((coPrzed(kierunek, labirynt, gracz) == ' ') || (coPrzed(kierunek, labirynt, gracz) == '_'))
		{			
			gracz.x = zmiennaGraczx;
			gracz.y = zmiennaGraczy;
			if (zn == 0x50)
			{
				rysujDoTylu(labirynt, gracz, daneRys, zmienneRys, animacja);				
			}
			else
			{
				rysujDoPrzodu(labirynt, gracz, daneRys, zmienneRys, animacja);				
			}
			if (animacja == 1 && daneRys.nrsciany==6)
			{
				for (int j = 0; j < 25; j++)
				{
					for (int i = 0; i < 10; i++)
					{
						gotoxy(23 + i, 1 + j);
						cout << ' ';
					}
				}
			}
			if (czyRysowacOtwarteDrzwi != 0 )
			{
				for (int i = 1; i < 3; i++)
					for (int j = 2; j < (daneRys.szerokoscP - 26); j++)
					{
						gotoxy(daneRys.szerokoscP - j, i + daneRys.gdzieRozpoczac);
						cout << '#';
					}
				for (int i = 1; i < 3; i++)
				{
					for (int j = 2; j < (27 - daneRys.szerokoscL); j++)
					{
						gotoxy(daneRys.szerokoscL + j, i + daneRys.gdzieRozpoczac);
						cout << '#';
					}
				}				
			}
			

			daneRys.zmian -= 1, daneRys.bec += 1;
			daneRys.wysokosc -= zmienneRys.zmiennaNaWysokosc, daneRys.szerokoscP -= zmienneRys.zmiennaNaSzerokosc, daneRys.szerokoscL += zmienneRys.zmiennaNaSzerokosc;
			daneRys.gdzieRozpoczac += zmienneRys.poczatekBloku, daneRys.nrsciany += 1;

			rysujScianeREK(labirynt, gracz, kierunek, daneRys,animacja,zn);
		}
		else
		{
			gracz.x = zmiennaGraczx;
			gracz.y = zmiennaGraczy;
			rysujHoryzont(labirynt,daneRys,zmienneRys);			
			if (animacja == 1)
				zamazNadHoryzontem(daneRys);			
		}	
		
		
	}
}

void rysuj(char labirynt[][23], int *x, int *y, int kierunek, int a,char zn)
{
	daneRys.zmian = 0, daneRys.bec = 0;
	daneRys.szerokoscP = 54, daneRys.wysokosc = 26, daneRys.szerokoscL=1;
	daneRys.gdzieRozpoczac = 0, daneRys.nrsciany = 0;
	
	rysujScianeREK(labirynt, gracz, kierunek, daneRys, a,zn);	
}

void OtworzDrzwi()
{		
		for (int i = 24; i > 2; i--)
		{
			for (int j = 2; j < 53; j++)
			{
				gotoxy(j, i+1);
				cout << ' ';
				gotoxy(j, i);
				cout << '#';
			}
			Sleep(5);
		}		
}

void zamknijDrzwi()
{	
		for (int i = 1; i < 26; i++)
		{
			for (int j = 2; j < 53; j++)
			{
				gotoxy(j, i);
				cout << '#';
			}
			Sleep(5);
		}
}


int main()
{
	int zn, attr = 7, t=0;
	
	settitle("Tymoteusz Konkol 160845");
	textbackground(BLACK);
	
	gracz.x = 1;
	gracz.y = 1;
	gracz.strzalka = '^';
	wspolrzedne.xpocz = gracz.x;
	wspolrzedne.ypocz = gracz.y;

	char graczPoczatkowe = gracz.strzalka;
	int czyRysowac = 0;
	int animacja = 0;
	
	char labirynt[23][23];

	int	kierunek = GORA, kierPoczatkowe = kierunek;
	int kroki = 0;

	clock_t czas = clock();
	clock_t zegar = clock();	
	do {
		if (animacja == 0)
		{
			clrscr();
		}
		textcolor(attr);
		labirynt[gracz.x][gracz.y] = gracz.strzalka;
		

		if (czyRysowac == 1)
		{
			gotoxy(55, 4);
			rysujLabirynt(labirynt);
			rysuj(labirynt, &gracz.x, &gracz.y, kierunek, animacja, zn);
		}

		gotoxy(55, 1);
		printf("Ilosc krokow: %d ", kroki);
		gotoxy(55, 2);
		czas = clock() - zegar;
		printf("Czas: %g  sek", ((float)czas) / CLOCKS_PER_SEC);
		gotoxy(55, 3);
		cputs("h = pomoc");

		

		zn = getch();
		if (zn == 0) {
			zn = getch();
			if (zn == 0x48)  //strzalka w gore
			{
				//labirynt[gracz.x][gracz.y] = coPrzed(kierunek, labirynt, gracz);
				if (kierunek == GORA && czyPrzed(kierunek, labirynt, gracz) == true)
				{
					labirynt[gracz.x][gracz.y] = ' ';
					gracz.x--;					
				}
				else if (kierunek == PRAWO && czyPrzed(kierunek, labirynt, gracz) == true)
				{
					labirynt[gracz.x][gracz.y] = ' ';
					gracz.y++;				
				}
				else if (kierunek == DOL && czyPrzed(kierunek, labirynt, gracz) == true)
				{
					labirynt[gracz.x][gracz.y] = ' ';
					gracz.x++;					
				}
				else if (kierunek == LEWO && czyPrzed(kierunek, labirynt, gracz) == true)
				{
					labirynt[gracz.x][gracz.y] = ' ';
					gracz.y--;					
				}
				if (czyRysowac == 1)
					kroki++;
			}
			else if (zn == 0x50) //strzalka w dol
			{

				if (kierunek == GORA && czyZa(kierunek, labirynt, gracz) == true)
				{
					labirynt[gracz.x][gracz.y] = ' ';
					gracz.x++;
				}
				else if (kierunek == PRAWO && czyZa(kierunek, labirynt, gracz) == true)
				{
					labirynt[gracz.x][gracz.y] = ' ';
					gracz.y--;
				}
				else if (kierunek == DOL && czyZa(kierunek, labirynt, gracz) == true)
				{
					labirynt[gracz.x][gracz.y] = ' ';
					gracz.x--;
				}
				else if (kierunek == LEWO && czyZa(kierunek, labirynt, gracz) == true)
				{
					labirynt[gracz.x][gracz.y] = ' ';
					gracz.y++;
				}
				if (czyRysowac == 1)
					kroki++;

			}
			else if (zn == 0x4b) //strzalka w lewo
			{
				clrscr();
				if (kierunek > 0)
				{
					kierunek -= 1;
					wskarzKierunek(&gracz.strzalka, kierunek);
				}
				else
					kierunek = 3;
				wskarzKierunek(&gracz.strzalka, kierunek);
			}
			else if (zn == 0x4d)	//strzalka w prawo
			{
				clrscr();
				if (kierunek < 3)
				{
					kierunek += 1;
					wskarzKierunek(&gracz.strzalka, kierunek);
				}
				else
					kierunek = 0;
				wskarzKierunek(&gracz.strzalka, kierunek);
			}
		}
		else if (zn == ' ')
		{
			attr = (attr + 1) % 16;
		}
		else if (zn == 'h' || zn == 'H')
		{
			pokazPomoc();
		}
		else if (zn == 'r' || zn == 'R')
		{
			labirynt[gracz.x][gracz.y] = ' ';
			gracz.x = wspolrzedne.xpocz;
			gracz.y = wspolrzedne.ypocz;
			kierunek = kierPoczatkowe;
			gracz.strzalka = graczPoczatkowe;
			kroki = 0;
			zegar = clock();
		}
		else if (zn == 'i')
		{			
			wczytajLab("labirynt.txt", labirynt, &gracz, &wspolrzedne,&drzwia,&czyRysowac);
			gotoxy(1, 1);
			rysujLabirynt(labirynt);			
			kroki = 0;
			kierunek = kierPoczatkowe;
			gracz.strzalka = graczPoczatkowe;
			zegar = clock();
		}
		else if (zn == 'o')
		{				
			gotoxy(25, 11);
			cputs("**********************************");
			gotoxy(25, 12);
			cputs("* Podaj nazwe pliku z labiryntem *");
			gotoxy(25, 13);
			cputs("* (plik musi byæ w formacie txt) *");
			gotoxy(25, 14);
			cputs("*                                *");
			gotoxy(25, 15);
			cputs("**********************************");
			Sleep(200);
			char nazwaPliku[20] = {};
			for (int i = 0; nazwaPliku[i-1] != 0; i++)
			{
				nazwaPliku[i] = getch();
				if (nazwaPliku[i] == '\r')
					nazwaPliku[i] = 0;
			}
			wczytajLab(nazwaPliku, labirynt, &gracz, &wspolrzedne, &drzwia,&czyRysowac);				
			gotoxy(1, 1);						
			kierunek = kierPoczatkowe;
			gracz.strzalka = graczPoczatkowe;
			kroki = 0;
			zegar = clock();
		}
		else if (zn == 'e')
		{			
			if (labirynt[0][0] == 'X')
			{
				clrscr();
				labirynt[gracz.x][gracz.y] = ' ';
				for (int i = 0; drzwia.x[i] != 0; i++)
					labirynt[(drzwia.x[i])][(drzwia.y[i])] = drzwia.znak[i];

				char znak = 'b';				
				do
				{
					clrscr();
					gotoxy(55, 1);
					printf("s - zapisz");
					gotoxy(55, 2);
					printf("k - koniec edycji");
					gotoxy(55, 3);
					printf("e - edytuj");
					rysujLabirynt(labirynt);
					znak = getch();
					if (znak == 'q')
						exit(0);
					else if (znak == 's')
					{
						zapiszDoPliku(labirynt);
					}
					else if (znak == 'e')
					{
						edytor(labirynt,&drzwia,&gracz,&wspolrzedne);
					}
				} while (znak != 'k');
				
			}
			else
			{
				gotoxy(26, 13);
				cout << "NIE WCZYTALES LABIRYNTU";
				getch();
			}
			gotoxy(14, 13);
			cout << "Zakonczono edycje";
			Sleep(500);
		}
		else if (zn == 'd' && coPrzed(kierunek, labirynt, gracz) == '#')
		{			
			drzwi(kierunek, '_');	
			OtworzDrzwi();
		}		
		else if (zn == 'd' && coPrzed(kierunek, labirynt, gracz) == '_')
		{	
			drzwi(kierunek, '#');	
			zamknijDrzwi();
		}
		else if (zn == 'a' && animacja == 0)
		{
			animacja = 1;
		}
		else if (zn == 'a' && animacja == 1)
		{
			animacja = 0;
		}	
		
		for (int i = 0; drzwia.x[i] != 0; i++)
			if ((gracz.x != drzwia.x[i] || gracz.y != drzwia.y[i]))
			{
				labirynt[(drzwia.x[i])][(drzwia.y[i])] = drzwia.znak[i];
			}
		if (gracz.x == wspolrzedne.xkonc && gracz.y == wspolrzedne.ykonc)
		{

			gotoxy(25, 1);
			cputs("*****************************");
			gotoxy(25, 2);
			cputs("*       koniec gry          *");
			gotoxy(25, 3);
			cputs("*   czy chcesz grac dalej?  *");
			gotoxy(25, 4);
			cputs("*       y-tak reszta-nie    *");
			gotoxy(25, 5);
			cputs("*****************************");
			char decyzja;
			cin >> decyzja;
			if (decyzja == 'y')
			{
				gracz.x = wspolrzedne.xpocz;
				gracz.y = wspolrzedne.ypocz;
				kierunek = kierPoczatkowe;
				gracz.strzalka = graczPoczatkowe;
				kroki = 0;
				zegar = clock();
			}
			else
				break;
		}
	} while (zn != 'q');

	return 0;
}
