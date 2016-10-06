#include <cstdio>
#include <cassert>
#include <cstring>

#define INFILE_NAME "demo_err_transmitted.txt"
#define MAX_FRAME_LEN 1524 * 8 + 1
#define MAX_GEN_LEN 4 * 8 + 1 + 1

int main() {
	char *nmOUT = new char[FILENAME_MAX];
	strncpy(nmOUT, INFILE_NAME, strlen(INFILE_NAME) - 15);
	nmOUT[strlen(INFILE_NAME) - 15] = '\0';
	strcat(nmOUT, "checked.txt");
	FILE *pIN = fopen(INFILE_NAME, "r");
	FILE *pOUT = fopen(nmOUT, "w");
	delete[] nmOUT;

	int num = 0;
	int cErr = 0;
	fscanf(pIN, "%d", &num);
	char *trans = new char[MAX_FRAME_LEN];
	char *gen = new char[MAX_GEN_LEN];
	for (int item = 0; item < num; item++) {
		// read the transmitted string and the generator string.
		fscanf(pIN, "%s", trans);
		fscanf(pIN, "%s", gen);
		// do error check with a shift register.
		int szTRANS = strlen(trans);
		int degree = strlen(gen) - 1;
		char *reg = new char[degree + 1];		// for uniform coding.
		memset(reg, '0', degree + 1);
		for (int i = 0; i < szTRANS; i++) {
			reg[degree] = trans[i];
			char overBit = reg[0];
			for (int j = 0; j < degree; j++) {		// for the j-th register.
				int iCorGen = j + 1;
				if (gen[iCorGen] == '1') reg[j] = ((overBit - '0') ^ (reg[j + 1] - '0')) + '0';
				else reg[j] = reg[j + 1];
			}
		}
		// check and output the remainder.
		for (int i = 0; i < degree; i++) {
			if (reg[i] == '1') { cErr++; break; }
		}
		reg[degree] = '\0';
		fprintf(pOUT, "%s\n", reg);
		delete[] reg;
	}

	fprintf(pOUT, "%d\n", cErr);

	delete[] trans;
	delete[] gen;
	fclose(pOUT);
	fclose(pIN);
	return 0;
}