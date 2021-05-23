#include <cstdio>

extern "C" {
int bufSize = 0;
int *buf = 0;
int pBuf = 0;
}

#define N 2
int main() {
  int result = 0;
  for (int i = 0; i < N; ++i) {
    int r = __builtin_riscv_hipaic_getrand();

    if (pBuf == bufSize) {
      bufSize += 16;
      buf = new int[bufSize];
    }
    buf[pBuf++] = 5;
    printf("Hello\n");

    result += r;
  }
  return result;
}

