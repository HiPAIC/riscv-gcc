#include <cstdio>

extern "C" {
int bufSize = 0;
int *buf = 0;
int pBuf = 0;
}

#define N 5
int main() {
  int result = 0;
  for (int i = 0; i < N; ++i) {
    int r = __builtin_riscv_hipaic_getrand();
    int x = __builtin_riscv_hipaic_recv();

    if (pBuf == bufSize) {
      bufSize += 16;
      buf = new int[bufSize];
    }
    buf[pBuf++] = r;

    printf("Diff %d GetRnd %d Recv %d\n", x-r, r, x);

    result += r;
  }
  return result;
}

