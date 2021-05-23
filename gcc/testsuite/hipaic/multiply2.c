#include <stdio.h>

int foo(int u, int v) {
  static int r_idx = 0;
  int u_idx = r_idx++;
  int v_idx = r_idx++;
  int w_idx = r_idx++;
  __builtin_riscv_hipaic_saverand(u_idx);
  __builtin_riscv_hipaic_saverand(v_idx);
  __builtin_riscv_hipaic_loadopx(u, u_idx);
  return __builtin_riscv_hipaic_multiply(v, v_idx, w_idx);
}

int bar(int u, int v) {
  int a, b, c, d, e, f;
  a = foo(u, v);
  b = foo(u, v);
  c = foo(u+1, v+1);
  d = foo(u+2, v+2);
  e = foo(u+1, v+1);
  f = foo(u+2, v+2);
  return __builtin_riscv_hipaic_mulsi(a, b) + __builtin_riscv_hipaic_mulsi(c, d) + __builtin_riscv_hipaic_mulsi(e, f);
}

int main() {
  int u, v, w;
  printf("Input two int32 u v:\n");
  scanf("%d%d", &u, &v);
  w = bar(u, v);
  printf("bar(u, v)= %d\n", w);
  return 0;
}

