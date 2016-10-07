#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cassert>

int main() {
	srand((unsigned)time(NULL));
	int p = 0, g = 0;
	printf("Input p and g; to exit, input 0 for p:\n");
	scanf(" %d %d", &p, &g);
	while (p != 0) {
		int sa = rand() % 1000;
		int sb = rand() % 1000;
		printf("Random sa = %d, sb = %d.\n", sa, sb);
		int ta = 1, tb = 1;
		for (int i = 0; i < sa; i++) ta = (ta*g) % p;
		for (int i = 0; i < sb; i++) tb = (tb*g) % p;
		int key_a = 1, key_b = 1;
		for (int i = 0; i < sa; i++) key_a = (key_a * tb) % p;
		for (int i = 0; i < sb; i++) key_b = (key_b * ta) % p;
		assert(key_a == key_b);
		printf("Key derived = %d.\n\n", key_a);

		printf("Input p and g; to exit, input 0 for p:\n");
		scanf(" %d %d", &p, &g);
	}

	return 0;
}
