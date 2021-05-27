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

    if (pBuf == bufSize) {
      bufSize += 16;
      buf = new int[bufSize];
    }
    buf[pBuf++] = r;
    __builtin_riscv_hipaic_send(r);

    printf("Send %d\n", r);

    result += r;
  }
  return result;
}

