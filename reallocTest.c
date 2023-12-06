#include<stdio.h>
#include<stdlib.h>
#include<math.h>

// driver
int main() {
  int n = 10;
  int *liczby = (int*) malloc((size_t) 1 * sizeof(int));
  liczby[0] = 33;
  for (int i = 1; i < n; i++) {
    liczby = (int*) realloc(liczby, (size_t) (i + 1) * sizeof(int));
    liczby[i] = i;
  }
  for (int i = 0; i < n; i++) {
    printf("%d\n", liczby[i]);
  }
  return 0;
}