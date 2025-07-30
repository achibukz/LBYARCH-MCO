#include <stdio.h>
#include <math.h>
#define N 4

float z_asm[N] = {0};

extern float* asmKernel(const float* x1, const float* x2, const float* y1, const float* y2);

void cKernel(const float* x1, const float* x2, const float* y1, const float* y2, float* z) {
    for (int i = 0; i < N; i++) {
        float dx = x2[i] - x1[i];
        float dy = y2[i] - y1[i];
        z[i] = sqrtf(dx * dx + dy * dy);
    }
}

int main() {
    float x1[N] = {1.5, 4.0, 3.5, 2.0};
    float x2[N] = {3.0, 2.5, 2.5, 1.0};
    float y1[N] = {4.0, 3.0, 3.5, 3.0};
    float y2[N] = {2.0, 2.5, 1.0, 1.5};

    float z_c[N] = {0};

    cKernel(x1, x2, y1, y2, z_c);
    float* result = asmKernel(x1, x2, y1, y2);

    printf("Index\tC Result\tASM Result\n");
    for (int i = 0; i < N; i++) {
        printf("%d\t%.9f\t%.9f\n", i, z_c[i], result[i]);
    }

    return 0;
}
