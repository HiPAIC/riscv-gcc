int foo() {
  static int r_idx = 0;
  int u_idx = r_idx++;
  int v_idx = r_idx++;
  __builtin_riscv_hipaic_saverand(u_idx);
  __builtin_riscv_hipaic_saverand(v_idx);
  return __builtin_riscv_hipaic_getrand();
}

int bar() {
  return foo()+foo()-foo();
}

