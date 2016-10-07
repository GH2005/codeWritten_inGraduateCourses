// csci-6101 a4 q7, division of big integers
// by Yang Wang, B00711141

#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstring>

bool Subtraction(char *minue, int l_minue, char *subtr, int l_subtr) {
	// return true if minue >= subtr, else false
	// if false, minue is not modified; else, minue becomes the diff
	// neither minue or subtr should contain preceding zeros

	// all false cases
	if (l_minue < l_subtr) return false;
	else if (l_minue == l_subtr) { // same length
		for (int i = 0; i < l_minue; i++) { // every digit
			if (minue[i] < subtr[i]) return false;
			else if (minue[i] == subtr[i]) continue;
			else break;
		}
	}

	// subtraction, all true cases
	for (int i_r = 0; i_r < l_subtr; i_r++) {
		// every digit, starting from the least significant
		int i_minue = l_minue - 1 - i_r;
		int i_subtr = l_subtr - 1 - i_r;
		if (minue[i_minue] < subtr[i_subtr]) minue[i_minue - 1]--;
		minue[i_minue] = (minue[i_minue] + 10 - subtr[i_subtr]) % 10;
	}
	return true;
}

int main() {
	char *n = new char[51];
	char *m = new char[51];
	char *working_buf = new char[51];
	char *quotient = new char[51];
	printf("Input n and m, up to 50 digits long each, ");
	printf("no preceding zeros (input 0 for n to quit):\n");
	scanf(" %s", n); scanf(" %s", m);
	while (strcmp(n, "0") != 0) {
		// subtract each char with '0'
		int l_n = strlen(n); int l_m = strlen(m);
		for (int i = 0; i < l_n; i++) n[i] -= '0';
		for (int i = 0; i < l_m; i++) m[i] -= '0';
		//
		// division
		//
		// working buffer (remainder) and quotient
		char *working_start = working_buf;
		int working_len = 0;
		int q_len = 0;

		int n_next = 0;	// indicate which digit to use in n
		while (n_next < l_n) {	// every digit in n
			working_start[working_len++] = n[n_next];
			// every time working_len++, get a quotient digit
			char c_subtraction = 0;
			while (Subtraction(working_start, working_len, m, l_m)) {
				c_subtraction++;
				// remove leading zeros every time; decrease working_len
				while (working_start[0] == 0 && working_len > 0) {
					working_start++;
					working_len--;
				}
			}
			quotient[q_len++] = c_subtraction;

			n_next++;
		}
		// remove leading zeros in the quotient
		char *q_trim = quotient;
		while (q_trim[0] == 0 && q_len > 0) {
			q_trim++;
			q_len--;
		}
		//
		// add '0' back, add '\0', so as to output
		//
		for (int i = 0; i < working_len; i++) working_start[i] += '0';
		working_start[working_len] = '\0';
		if (working_len == 0) {	// if the remainder is 0
			working_start--;
			working_start[0] = '0';
		}
		for (int i = 0; i < q_len; i++) q_trim[i] += '0';
		q_trim[q_len] = '\0';
		if (q_len == 0) { // if the quotient is 0
			q_trim--;
			q_trim[0] = '0';
		}
		for (int i = 0; i < l_n; i++) n[i] += '0';
		for (int i = 0; i < l_m; i++) m[i] += '0';

		printf("n = m*q + r, where:\nn = %s\nm = %s\n", n, m);
		printf("q = %s\nr = %s\n\n", q_trim, working_start);

		printf("Input n and m, up to 50 digits long each, ");
		printf("no preceding zeros (input 0 for n to quit):\n");
		scanf(" %s", n); scanf(" %s", m);
	}

	delete[] n, m, working_buf, quotient;
	return 0;
}