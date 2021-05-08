#include <stdio.h>

unsigned int f(unsigned int x, unsigned int y) {
  return __builtin_riscv_hipaic_mulsi(x, y);
}

int main() {
  int x, y;
  printf("Input two uint32 x y:\n");
  scanf("%u%u", &x, &y);
  printf("f(x,y)=%u\n__builtin_riscv_hipaic_mulsi(x,y)= %u\n", f(x,y), __builtin_riscv_hipaic_mulsi(x,y));
  printf("f(x*3, y*2)= %u\n", f(x*3, y*2));
  return 0;
}

