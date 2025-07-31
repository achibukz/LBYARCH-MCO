#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#define N 4

float z_asm[N] = {0};

extern void asmKernel(const float* x1, const float* x2, const float* y1, const float* y2, float* z, int n);

/*
    Computes the distance between pairs of (x1[i], y1[i]) and (x2[i], y2[i]) points, and stores the result in the output array z.

    @param x1 – pointer to the array of starting x-coordinates
    @param x2 – pointer to the array of ending x-coordinates
    @param y1 – pointer to the array of starting y-coordinates
    @param y2 – pointer to the array of ending y-coordinates
    @param z – pointer to the array where the computed distances will be stored

    @return – none (results are stored in the z array)
*/
void cKernel(const float* x1, const float* x2, const float* y1, const float* y2, float* z) {
    for (int i = 0; i < N; i++) {
        float dx = x2[i] - x1[i];
        float dy = y2[i] - y1[i];
        z[i] = sqrtf(dx * dx + dy * dy);
    }
}


/*
    Measures the average execution time of the C kernel over 30 runs using Windows high-resolution performance counters.

    @param x1 – pointer to the array of starting x-coordinates
    @param x2 – pointer to the array of ending x-coordinates
    @param y1 – pointer to the array of starting y-coordinates
    @param y2 – pointer to the array of ending y-coordinates
    @param z – pointer to the output array where distances will be stored
    @param n – number of elements in each array

    @return double – average execution time in milliseconds over 30 runs
*/
double time_c_kernel(const float* x1, const float* x2, const float* y1, const float* y2, float* z, int n) {
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    double total = 0.0;

    for (int i = 0; i < 30; i++) {
        QueryPerformanceCounter(&start);

        for (int j = 0; j < n; j++) {
            float dx = x2[j] - x1[j];
            float dy = y2[j] - y1[j];
            z[j] = sqrtf(dx * dx + dy * dy);
        }

        QueryPerformanceCounter(&end);
        total += (double)(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
    }

    return total / 30.0;
}


/*
    Measures the average execution time of the assembly kernel over 30 runs using Windows high-resolution performance counters.

    @param x1 – pointer to the array of starting x-coordinates  
    @param x2 – pointer to the array of ending x-coordinates  
    @param y1 – pointer to the array of starting y-coordinates  
    @param y2 – pointer to the array of ending y-coordinates  
    @param z – pointer to the output array where distances will be stored  
    @param n – number of elements in each array

    @return double – average execution time in milliseconds over 30 runs
*/

double time_asm_kernel(const float* x1, const float* x2, const float* y1, const float* y2, float* z, int n) {
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    double total = 0.0;

    for (int i = 0; i < 30; i++) {
        QueryPerformanceCounter(&start);

        asmKernel(x1, x2, y1, y2, z, n);

        QueryPerformanceCounter(&end);
        total += (double)(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
    }

    return total / 30.0;
}

int main() {
    float x1[N] = {1.5, 4.0, 3.5, 2.0};
    float x2[N] = {3.0, 2.5, 2.5, 1.0};
    float y1[N] = {4.0, 3.0, 3.5, 3.0};
    float y2[N] = {2.0, 2.5, 1.0, 1.5};

    float z_c[N] = {0};

    cKernel(x1, x2, y1, y2, z_c);
    asmKernel(x1, x2, y1, y2, z_asm, N);

    printf("Index\tC Result\tASM Result\n");
    for (int i = 0; i < N; i++) {
        printf("%d\t%.9f\t%.9f\n", i, z_c[i], z_asm[i]);
    }

    printf("\n\n===== TIMING PERFORMANCE TESTS (SPEC i) =====\n");
    printf("\n\n=====       FOR READ ME PURPOSE         =====\n");
    int sizes[] = { 1 << 20, 1 << 24, 1 << 28 };
    for (int s = 0; s < 3; s++) {
        int n = sizes[s];
        printf("\nVector size: n = %d (2^%d)\n", n, (int)(log2(n)));

        float* tx1 = (float*)malloc(sizeof(float) * n);
        float* tx2 = (float*)malloc(sizeof(float) * n);
        float* ty1 = (float*)malloc(sizeof(float) * n);
        float* ty2 = (float*)malloc(sizeof(float) * n);
        float* tz_c = (float*)malloc(sizeof(float) * n);
        float* tz_asm = (float*)malloc(sizeof(float) * n);

        if (!tx1 || !tx2 || !ty1 || !ty2 || !tz_c || !tz_asm) {
            printf("Memory allocation failed for n = %d\n", n);
            continue;
        }

        for (int i = 0; i < n; i++) {
            tx1[i] = rand() % 1000 / 10.0f;
            tx2[i] = rand() % 1000 / 10.0f;
            ty1[i] = rand() % 1000 / 10.0f;
            ty2[i] = rand() % 1000 / 10.0f;
        }

        double avg_c = time_c_kernel(tx1, tx2, ty1, ty2, tz_c, n);
        double avg_asm = time_asm_kernel(tx1, tx2, ty1, ty2, tz_asm, n);

        printf("C Kernel   Average: %.3f ms\n", avg_c);
        printf("ASM Kernel Average: %.3f ms\n", avg_asm);

        printf("Index\tC Result\tASM Result\n");
        for (int i = 0; i < 5; i++) {
            printf("%d\t%.6f\t%.6f\n", i, tz_c[i], tz_asm[i]);
        }

        free(tx1); free(tx2); free(ty1); free(ty2); free(tz_c); free(tz_asm);
    }

    return 0;
}
