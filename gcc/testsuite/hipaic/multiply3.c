int foo(int u, int v) {
  static int r_idx = 0;
  int u_idx = r_idx++;
  int v_idx = r_idx++;
  __builtin_riscv_hipaic_saverand(u_idx);
  __builtin_riscv_hipaic_saverand(v_idx);
  __builtin_riscv_hipaic_loadopx(u, u_idx);
  return __builtin_riscv_hipaic_multiply(v, v_idx);
}

void bar(int len, int *u, int *v, int *result) {
  for (int i = 0; i < len; ++i) {
    result[i] = foo(u[i], v[i]);
  }
}
