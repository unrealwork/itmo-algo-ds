#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


bool isDigit(char ch);


void sort(int *a, int n) {
    int BITS_PER_BYTE = 8;
    int BITS = 32;                 // each int is 32 bits
    int R = 1 << BITS_PER_BYTE;    // each bytes is between 0 and 255
    int MASK = R - 1;              // 0xFF
    int w = BITS / BITS_PER_BYTE;  // each int is 4 bytes

    int *aux = malloc(n * sizeof(n));
    for (int i = 0; i < n; i++) {
        aux[i] = 0;
    }
    int *count = malloc((R + 1) * sizeof(int));
    for (int d = 0; d < w; d++) {

        // compute frequency counts
        for (int i = 0; i < (R + 1); i++) {
            count[i] = 0;
        }
        for (int i = 0; i < n; i++) {
            int c = (a[i] >> BITS_PER_BYTE * d) & MASK;
            count[c + 1]++;
        }

        // compute cumulates
        for (int r = 0; r < R; r++)
            count[r + 1] += count[r];

        // for most significant byte, 0x80-0xFF comes before 0x00-0x7F
        if (d == w - 1) {
            int shift1 = count[R] - count[R / 2];
            int shift2 = count[R / 2];
            for (int r = 0; r < R / 2; r++)
                count[r] += shift1;
            for (int r = R / 2; r < R; r++)
                count[r] -= shift2;
        }

        // move data
        for (int i = 0; i < n; i++) {
            int c = (a[i] >> BITS_PER_BYTE * d) & MASK;
            aux[count[c]++] = a[i];
        }

        // copy back
        for (int i = 0; i < n; i++) {
            a[i] = aux[i];
        }
    }
    free(count);
    free(aux);
}


int read_int(FILE *file) {
    if (file) {
        char ch = (char) getc(file);
        char *buffer = (char *) malloc(32 * sizeof(char));
        int j = 0;
        while (ch >= '0' && ch <= '9' || ch == '-') {
            buffer[j] = ch;
            ch = (char) getc(file);
            j++;
        }
        buffer[j] = '\0';
        int res = atoi(buffer);
        free(buffer);
        return res;
    }
}

int *read_array(FILE *file, int size) {
    int *arr = (int *) malloc(size * sizeof(int));
    int i = 0;
    while (i < size) {
        arr[i] = read_int(file);
        i++;
    }
    return arr;
}

bool isDigit(char ch) { return ch >= '0' && ch <= '9'; }

unsigned long long int solve(int *pInt, int n) {
    unsigned long long sum = 0L;
    sort(pInt, n);
    for (int i = 0; i < n; i += 10) {
        sum += pInt[i];
    }
    return sum;
}

int main() {
    FILE *file;
    file = fopen("input.txt", "r");
    int n = read_int(file);
    int m = read_int(file);
    int *arr = read_array(file, n);
    int *mul = malloc(m * n * sizeof(int));
    int k = 0;
    for (int i = 0; i < m; i++) {
        int read = read_int(file);
        for (int j = 0; j < n; j++) {
            mul[k] = read * arr[j];
            k++;
        }
    }
    free(arr);
    fclose(file);
    unsigned long long int res = solve(mul, m * n);
    free(mul);
    FILE *output = fopen("output.txt", "w");
    fprintf(output, "%llu", res);
    fclose(output);
    return 0;
}

