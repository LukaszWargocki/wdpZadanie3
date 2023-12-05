#include<stdio.h>
#include<stdlib.h>
#include<math.h>

const double EPS = 1e-6;

/*struktury*/
// struktura punktu
typedef struct punkt {
	double x;
	double y;
} punkt;

// prosta ax + by + c = 0
typedef struct prosta {
  double a;
  double b;
	double c;
} prosta;

// struktura wektora
typedef struct wektor {
	punkt a;
	punkt b;
} wektor;

// struktura kartki
typedef struct kartka {
	int czyProstokat;   // flaga ksztaltu
	punkt a, b;         // punkty narozne prostokata 
	punkt s;            // srodek kola
	double r;           // promien kola
	int ileZlozen;      // ilosc zlozen
	wektor *zlozenia;   // lista poprzednich zlozen
} kartka;

/*deklaracje funkcji*/
int czyZero(double x);
int czyUjemna(double x);
double fkwadrat(double x);
punkt odejmij(punkt v1, punkt v2);
punkt odbij(punkt p, wektor v);
double det(punkt v1, punkt v2);
punkt Punkt(double x, double y);
wektor Wektor(punkt a, punkt b);
prosta ProstaZWektora(wektor v);
kartka Prostokat(punkt a, punkt b);
kartka Kolo(punkt s, double r);
kartka Zlozona(kartka k, wektor v);
int czyPrzebija(kartka k, punkt p);
int przebiciaZlozonej(kartka k, punkt p, int ind);

// driver
int main() {
	// odczytaj ilosc kartek i zapytan
	int n, q;
	scanf("%d %d", &n, &q);
	// odczytaj i zapisz w tablicy kartki
	kartka* kartki = malloc((size_t)(n) * sizeof(kartka));
	for (int i = 0; i < n; i++) {
		char typ;
		scanf(" %c", &typ);
		if(typ == 'P') {
			punkt a, b;
			scanf("%lf %lf %lf %lf", &a.x, &a.y, &b.x, &b.y);
			kartki[i] = Prostokat(a, b);
		} else if (typ == 'K') {
			punkt s;
			double r;
			scanf("%lf %lf %lf", &s.x, &s.y, &r);
			kartki[i] = Kolo(s, r);
		} else { // typ == 'Z'
			int numerKartki = 0;
			punkt a, b;
			scanf("%d %lf %lf %lf %lf", &numerKartki, &a.x, &a.y, &b.x, &b.y);
			// zloz kartke o odpowiednim numerze i zapisz
			kartki[i] = Zlozona(kartki[numerKartki - 1], Wektor(a, b));
		}
	}

	// przetworz zapytania
	for (int i = 0; i < q; i++) {
		int k = 0;
		punkt p;
		scanf("%d %lf %lf", &k, &p.x, &p.y);
		//--k;
		int wynik;
		// karki 'P' i 'K'
		if (kartki[k - 1].ileZlozen == 0) {
			wynik = czyPrzebija(kartki[k - 1], p);
		// kartki 'Z'
		} else {
			wynik = przebiciaZlozonej(kartki[k - 1], p, kartki[k - 1].ileZlozen - 1);
		}
		printf("%d\n", wynik);
	}

	// zwolnij pamiec
	for (int i = 0; i < n; i++) {
		free(kartki[i].zlozenia);
	}
	free(kartki);

	return 0;
}

/*funkcje pomocnicze*/
// przyjety test rownosci z zerem liczby zmiennoprzecinkowej
int czyZero(double x) {
    return fabs(x) < EPS;
}

// przyjety test ujemnosci liczny zmiennoprzecinkowej
int czyUjemna(double x) {
    return x + EPS < 0;
}

// kwadrat zmiennoprzecinkowej
double fkwadrat(double x) {
  return x * x;
}

// roznica dwoch wektorow
punkt odejmij(punkt v1, punkt v2) {
  punkt rez;
  rez.x = v1.x - v2.x;
  rez.y = v1.y - v2.y;
  return rez;
}

// zwraca prosta prostopadla do wektora v przechodzaca przez punkt p
punkt odbij(punkt p, wektor v) {
	prosta l = ProstaZWektora(v);
	double len = (l.a*p.x + l.b*p.y + l.c) / (fkwadrat(l.a) + fkwadrat(l.b));
	return Punkt(p.x - 2 * l.a * len, p.y - 2 * l.b * len);
}

// iloczyn wektorowy na plaszczyznie (wyznacznik)
double det(punkt v1, punkt v2) {
	double rez;
	rez  = v1.x * v2.y - v1.y * v2.x;
	return rez;
}

/*konstruktory */
// konstruktor punktu
punkt Punkt(double x, double y) {
	punkt rez;
	rez.x = x;
	rez.y = y;
	return rez;
}

// konstruktor odcinka z A do B
wektor Wektor(punkt a, punkt b) {
	wektor rez;
	rez.a = a;
	rez.b = b;
	return rez;
}

// zwraca prosta przechodzaca przez punkty AB wektora v
prosta ProstaZWektora(wektor v) {
  prosta rez;
  rez.a = v.b.y - v.a.y;
	rez.b = v.a.x - v.b.x;
	rez.c = v.a.y * v.b.x - v.a.x * v.b.y;
  return rez;
}

// konstruktor kartki typu prostokat
kartka Prostokat(punkt a, punkt b) {
	kartka rez;
	rez.czyProstokat = 1;
	rez.ileZlozen = 0;
	rez.a = a;
	rez.b = b;
	return rez;
}

// konstruktor kartki typu kolo
kartka Kolo(punkt s, double r) {
  kartka rez;
  rez.czyProstokat = 0;
  rez.ileZlozen = 0;
  rez.r = r;
  rez.s = s;
  return rez;
}

// konstrukor kartki zlozonej, zwraca zadana kartke z dodanym zadanym zlozeniem
kartka Zlozona(kartka k, wektor v) {
	kartka rez;
	if (k.czyProstokat) {
		rez = Prostokat(k.a, k.b);
	} else {
		rez = Kolo(k.s, k.r);
	}
	// skopiuj zlozenia zadanej kartki i dodaj biezace zlozenie
	rez.ileZlozen = k.ileZlozen + 1;
	rez.zlozenia = (wektor*) malloc((size_t) (rez.ileZlozen) * sizeof(wektor));
	if (k.ileZlozen) {
    for (int i = 0; i < k.ileZlozen; i++) {
      rez.zlozenia[i] = k.zlozenia[i];
    }
  }
	rez.zlozenia[rez.ileZlozen - 1] = v;
	return rez;
}

/*funkcje*/
// funkcja okreslajaca czy punkt p przebija kartke k
int czyPrzebija(kartka k, punkt p) {
	if (k.czyProstokat) {
    return fmin(p.x - k.a.x, p.y - k.a.y) > -EPS && fmin(k.b.x - p.x, k.b.y - p.y) > -EPS;
  } else { 
		return fkwadrat(p.x - k.s.x) + fkwadrat(p.y - k.s.y) < fkwadrat(k.r) + EPS;
  }
}

// wyznacza rekurencyjnie ile warstw ma zlozona kartka k w punkcie p
int przebiciaZlozonej(kartka k, punkt p, int ind) { 
	wektor zlozenie = k.zlozenia[ind];
	double wyzn = det(odejmij(zlozenie.b, zlozenie.a), odejmij(p, zlozenie.a));
	// przypadek bazowy
	if (ind == 0) { 
		if (czyZero(wyzn)) {													// na linii zlozenia
			return czyPrzebija(k, p);
		} else if (czyUjemna(wyzn)) {									// na prawo
			return 0;
		} else {																			// na lewo
			return czyPrzebija(k, p) + czyPrzebija(k, odbij(p, zlozenie));
		}
	}
	// przypadek rekurencyjny
	if (czyZero(wyzn)) { 														// na linii zlozenia
		return przebiciaZlozonej(k, p, ind - 1);
	} else if (czyUjemna(wyzn)) {										// na prawo
		return 0;
	}else { 																				// na lewo
		return przebiciaZlozonej(k, p, ind - 1) + przebiciaZlozonej(k, odbij(p, zlozenie), ind-1);
	}
}