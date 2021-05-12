#include <stdio.h>

int foo() {
  static int r_idx = 0;
  int u_idx = r_idx++;
  int v_idx = r_idx++;
  __builtin_riscv_hipaic_gennewrand(u_idx);
  __builtin_riscv_hipaic_gennewrand(v_idx);
  return __builtin_riscv_hipaic_getnextrand();
}

int main() {
  int turns;
  printf("Input how many turns:\n");
  scanf("%d", &turns);
  int sum = 0;
  for (; turns > 0; --turns) {
    sum += foo();
  }
  printf("sum=%d\n", sum);
  return 0;
}

