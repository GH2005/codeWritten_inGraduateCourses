#include <cstdio>
#include <cassert>
#include <ctime>
#include <cstring>
#include <cstdlib>

#define TRANS_FILENAME "CRC32_MSG_350_transmitted.txt"
#define MAX_GENERATOR_LEN 4 * 8 + 1 + 1
#define MAX_FRAME_LEN 1524 * 8 + 1

int main() {
	printf("Please input 1 (less than 32 bits), 2 (equal to 32 bits) or 3 (more than 32 bits) ");
	printf("as the length of burst errors:\n");
	int choice = 0;
	scanf("%d", &choice);
	assert(choice == 1 || choice == 2 || choice == 3);

	char *nmOUT = new char[FILENAME_MAX];
	strncpy(nmOUT, TRANS_FILENAME, strlen(TRANS_FILENAME) - 15);
	nmOUT[strlen(TRANS_FILENAME) - 15] = '\0';
	strcat(nmOUT, "err_transmitted.txt");
	FILE *fIN = fopen(TRANS_FILENAME, "r");
	FILE *fOUT = fopen(nmOUT, "w");
	delete[] nmOUT;

	const char *CRC32gen = "100000100110000010001110110110111";
	int num = 0;
	fscanf(fIN, "%d", &num);
	fprintf(fOUT, "%d\n", num);
	char *trans = new char[MAX_FRAME_LEN];
	char *gen = new char[MAX_GENERATOR_LEN];
	srand((unsigned)time(NULL));
	for (int i = 0; i < num; i++) {			// assume all errors happen from the first bit of a 
		fscanf(fIN, "%s", trans);				// transmitted bit string.
		fscanf(fIN, "%s", gen);
		if (choice == 1) {
			int errLen = 1 + rand() % 31;
			for (int j = 0; j < errLen; j++) {
				trans[j] = ((trans[j] - '0') ^ char(rand() % 2)) + '0';
			}
		}
		else if (choice == 2) {
			int errLen = 32;
			for (int j = 0; j < errLen; j++) {
				trans[j] = ((trans[j] - '0') ^ char(rand() % 2)) + '0';
			}
		}
		else {	// choice == 3.
			int errLen = 33 + rand() % 12159;
			for (int j = 0; j < errLen; j++) {
				trans[j] = ((trans[j] - '0') ^ char(rand() % 2)) + '0';
			}
		}
		fprintf(fOUT, "%s       %s\n", trans, CRC32gen);
	}

	fclose(fOUT);
	fclose(fIN);
	return 0;
}