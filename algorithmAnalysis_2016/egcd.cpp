#include <cstdio>

int ExGCD(int a, int b, int &x, int &y) {
	// returns gcd(a, b) and modifies x and y such that
	// ax + by = gcd(a, b)
	// assume a >= b >= 0, a > 0
	int a0 = a, a1 = b;
	int x0 = 1, y0 = 0;
	int x1 = 0, y1 = 1;
	while (a1 != 0) {
		int q = a0 / a1;
		int a2 = a0 - q*a1;
		int x2 = x0 - q*x1;
		int y2 = y0 - q*y1;
		a0 = a1; a1 = a2;
		x0 = x1; x1 = x2;
		y0 = y1; y1 = y2;
	}
	x = x0; y = y0;
	return a0;
}

int main() {
	int a = 0, b = 0;
	int x = 0, y = 0, g = 0;
	printf("Input integers a and b (input 0 for a to quit):\n");
	scanf(" %d %d", &a, &b);
	while (a > 0) {
		g = ExGCD(a, b, x, y);
		printf("GCD(%d, %d) = %d, x = %d, y = %d\n", a, b, g, x, y);
		if (g != 1 || a == 0 || b == 0) {	// an inverse doesn't exist
			printf("no inverse for %d (mod %d) exist.\n\n", a, b);
		}
		else {	// get the inverse
			printf("the inverse for %d (mod %d) is %d.\n\n", a, b, (x + b) % b);
		}

		printf("Input integers a and b (input 0 for a to quit):\n");
		scanf(" %d %d", &a, &b);
	}
	return 0;
}