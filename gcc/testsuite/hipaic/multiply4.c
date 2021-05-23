#include <stdio.h>

inline static int foo(int u, int v) {
  static int r_idx = 0;
  int u_idx = r_idx++;
  int v_idx = r_idx++;
  int w_idx = r_idx++;
  __builtin_riscv_hipaic_saverand(u_idx);
  __builtin_riscv_hipaic_saverand(v_idx);
  __builtin_riscv_hipaic_loadopx(u, u_idx);
  return __builtin_riscv_hipaic_multiply(v, v_idx, w_idx);
}

inline static void bar(int len, int *u, int *v, int *result) {
  for (int i = 0; i < len; ++i) {
    result[i] = foo(u[i], v[i]);
  }
}

#define N 8
int main() {
  int result[N], u[N], v[N];
  for (int i = 0; i < N; ++i) {
    u[i] = i*567;
    v[i] = i*1234;
  }

  bar(N, u, v, result);
  for (int i = 0; i < N; ++i) {
    printf("u[%d]=%d v[%d]=%d result[%d]=%d\n", i, u[i], i, v[i], i, result[i]);
  }

  return 0;
}