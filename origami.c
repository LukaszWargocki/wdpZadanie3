#include<stdio.h>
#include<stdbool.h>

const float EPS = 10e-6;

bool czyZero(float x) {
  return fabs(x) < EPS;
}

bool czyUjemna(float x) {
  return x + EPS < 0;
}

int main () {
  int n, q;
  scanf("%d %d", &n, &q);
  
  return 0;
}
// punkt
typedef struct punkt {
  float x;
  float y;
} punkt;

typedef struct prosta {
  float a;
  float b;
} prosta;

// typedef struct wektor {
//   float x;
//   float y;
// } wektor;

// kwadrat
float kwad(float x) {
  return x * x;
}
// różnica wektorów
// wektor odejmij(wektor v1, wektor v2) {
//   wektor rez;
//   rez.x = v1.x - v2.x;
//   rez.y = v1.y - v2.y;
//   return rez;
// }
punkt odejmij(punkt v1, punkt v2) {
  punkt rez;
  rez.x = v1.x - v2.x;
  rez.y = v1.y - v2.y;
  return rez;
}
// iloczyn wektorowy
float det(punkt v1, punkt v2) {
  float rez;
  rez  = v1.x * v2.y - v1.y * v2.x;
  return rez;
}

prosta prostaZPunktow(punkt p1, punkt p2) {
  prosta rez;
  rez.a = (p2.y - p1.y) / (p2.x - p1.x);
  rez.b = p1.y - (p1.x * rez.a);
  return rez;
}

prosta prostaAPrzezPunkt(float a, punkt p) {
  prosta rez;
  rez.a = a;
  rez.b = p.y - a * p.x;
  return rez;
}

punkt odbij(punkt a, punkt b, punkt p) {
  punkt rez;
  if (a.x == b.x) { // pionowa
    rez.x = 2 * a.x - p.x;
    rez.y = p.y;
  } else if (a.y == b.y) { // pozioma
    rez.x = p.x;
    rez.y = 2 * a.y - p.y;
  } else { // prosta odchylona
    prosta l = prostaZPunktow(a, b);
    prosta orto = prostaAPrzezPunkt(-1 / l.a, p);
    // punkt przecięcia
    float crossX = (orto.b - l.b) / (l.a - orto.a);
    float crossY = crossX * l.a + l.b;
    rez.x = 2 * crossX - p.x;
    rez.y = 2 * crossY - p.y;
  }
  return rez;
}

typedef struct prostokat {
  punkt a;
  punkt b;
} prostokat;

int przebijPro(prostokat r, punkt p) {
  if (r.a.x <= p.x &&  p.x <= r.b.x && r.a.y <= p.y && p.y <= r.b.y)
    return 1;
  return 0;
}

typedef struct kolo {
  punkt s;
  float r;
} kolo;

int przebijKolo(kolo k, punkt p) {
  if (kwad(p.x - k.s.x) + kwad(p.y - k.s.y) <= kwad(k.r))
    return 1;
  return 0;
}

int zlozPro(punkt a, punkt b, prostokat r, punkt p) {
  float wyz = det(odejmij(b, a), odejmij(p, a));
  if (wyz == 0) 
    return przebijPro(r, p);
  else if (wyz < 0)
    return 0;
  else
    return przebijPro(r, p) + przebijPro(r, odbij(a, b, p));
};

int zlozKolo(punkt a, punkt b, kolo k, punkt p) {
  // stosunek wyniku iloczynu wektorowego wektorów ab i ap do zera określa po której stronie zagięcia jest punkt p
  float wyz = det(odejmij(b, a), odejmij(p, a));
  if (czyUjemna(wyz)) // na prawo
    return 0;
  else if (czyZero(wyz)) // na zgięciu
    return przebijKolo(k, p);
  else // na lewo
    return przebijKolo(k, p) + przebijKolo(k, odbij(a, b, p));
};