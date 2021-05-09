#include <stdio.h>

int f(int x, int y) {
  return __builtin_riscv_hipaic_mulsi(x, y);
}

int main() {
  int x, y;
  printf("Input two int32 x y:\n");
  scanf("%d%d", &x, &y);
  printf("f(x,y)= %d\n__builtin_riscv_hipaic_mulsi(x,y)= %d\n", f(x,y), __builtin_riscv_hipaic_mulsi(x,y));
  printf("f(x*3, y*2)= %d\n", f(x*3, y*2));
  return 0;
}

