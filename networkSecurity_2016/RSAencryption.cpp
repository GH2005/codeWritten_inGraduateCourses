#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>

int main() {
	int m = 0, e = 0, n = 0;
	printf("Input m, e and n in order; to exit, input 0 for m:\n");
	scanf(" %d %d %d", &m, &e, &n);
	while (m != 0) {
		int working = 1;
		for (int i = 0; i < e; i++) working = (working*m) % n;
		printf("The ciphertext c = %d.\n", working);

		printf("Input m, e and n in order; to exit, input 0 for m:\n");
		scanf(" %d %d %d", &m, &e, &n);
	}

	return 0;
}
