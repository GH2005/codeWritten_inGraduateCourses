#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>

#define MAX_GENERATOR_LEN 4 * 8 + 1 + 1
#define MAX_FRAME_LEN 1524 * 8 + 1

int main() {
	printf("Input the number of items you want:\n");
	int num = 0;
	scanf("%d", &num);

	char *nmOUT = new char[FILENAME_MAX];
	_itoa(num, strcpy(nmOUT, "CRC32_MSG_") + 10, 10);
	strcat(nmOUT, ".txt");
	FILE *fOUT = fopen(nmOUT, "w");
	delete[] nmOUT;

	fprintf(fOUT, "%d\n", num);
	const char *CRC32gen = "100000100110000010001110110110111";
	char *buf = new char[MAX_FRAME_LEN];
	srand((unsigned)time(NULL));
	for (int i = 0; i < num; i++) {
		for (int j = 0; j < 1520 * 8; j++) {
			buf[j] = char(rand() % 2) + '0';
		}
		buf[1520 * 8] = '\0';
		// output this item.
		fprintf(fOUT, "%s       %s\n", buf, CRC32gen);
	}
	delete[] buf;

	fclose(fOUT);
	return 0;
}