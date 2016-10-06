#include <cstdio>
#include <cassert>
#include <string>

#define IN_FILE_NAME "demo.txt"
#define MAX_GENERATOR_LEN 4 * 8 + 1 + 1
#define MAX_FRAME_LEN 1524 * 8 + 1
// the last element is for '\0'.

int main() {

	// open file handles.
	FILE *pIn = fopen(IN_FILE_NAME, "r"); assert(pIn != nullptr);
	std::string oNm
		= std::string(IN_FILE_NAME).substr(0, strlen(IN_FILE_NAME) - 4)
			+ "_transmitted.txt";
	FILE *pOut = fopen(oNm.c_str(), "w"); assert(pOut != nullptr);

	// generate actual transmitted bit strings.
	int num = 0;
	fscanf(pIn, "%d", &num);
	fprintf(pOut, "%d\n", num);
	char *msg = new char[MAX_FRAME_LEN];
	char *gen = new char[MAX_GENERATOR_LEN];
	for (int item = 0; item < num; item++) {
		// read fields of the current item.
		fscanf(pIn, "%s", msg);
		fscanf(pIn, "%s", gen);
		int szMsg = strlen(msg);
		int szGen = strlen(gen);
		int degree = szGen - 1;
		// append msg with zeros.
		for (int i = 0; i < degree; i++) {
			msg[szMsg + i] = '0';
		}
		msg[szMsg + degree] = '\0';
		int szMsgp = strlen(msg);

		// construct the shift register.
		char *reg = new char[degree + 1];		// plus 1 for uniform coding.
		for (int i = 0; i < degree + 1; i++) reg[i] = '0';
		for (int i = 0; i < szMsgp; i++) {
			reg[degree] = msg[i]; 
			char overBit = reg[0];
			for (int j = 0; j < degree; j++) {	// j is the register index.
				int iCorGen = j + 1;
				// direct shift or XOR shift.
				if (gen[iCorGen] == '1') reg[j] = ((overBit - '0') ^ (reg[j + 1] - '0')) + '0';
				else reg[j] = reg[j + 1];
			}
		}

		// append check string to orignal message.
		reg[degree] = '\0';
		for (int i = 0; i < degree + 1; i++) msg[szMsg + i] = reg[i];
		assert(strlen(msg) == szMsgp);

		// write to the output file.
		fprintf(pOut, "%s       ", msg);
		fprintf(pOut, "%s\n", gen);

		delete[] reg;
	}

	// cleaning.
	delete[] msg;
	delete[] gen;
	fclose(pIn);
	fclose(pOut);
	return 0;
}