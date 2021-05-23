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
  return a*b+c*d+e*f;
}

