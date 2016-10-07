// digraphic substitution cipher by Yang Wang, B00711141
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstring>
#include <cassert>

void Encryption();
void Decryption();

int main() {
	// encryption or decryption
	printf("Digraphic Substitution Cipher. Input 1 for encryption");
	printf(" or 2 for decryption:\n");
	int dec = 0;
	scanf(" %d", &dec);
	if (dec == 1) Encryption();
	else Decryption();

	return 0;
}

void Encryption() {
	//
	// file handles
	//
	char *fnm_buffer = new char[50];
	printf("Input the name of the file containing PT:\n");
	scanf(" %s", fnm_buffer);
	FILE *f_pt = fopen(fnm_buffer, "r");
	printf("Input the name of the file containing Keys:\n");
	scanf(" %s", fnm_buffer);
	FILE *f_key = fopen(fnm_buffer, "r");
	printf("Input the name of the file to which the CT is to be written:\n");
	scanf(" %s", fnm_buffer);
	FILE *f_ct = fopen(fnm_buffer, "w");
	delete[] fnm_buffer;
	//
	// process one by one
	//
	char *pt_buf = new char[201];
	char *key_buf = new char[51];
	char cipher_tbl[25];
	bool hash[26];
	fscanf(f_pt, " %s", pt_buf);
	while (!feof(f_pt)) {	// enumerate cases
		fscanf(f_key, " %s", key_buf);	// get the key

		// fill cipher_tbl
		memset(hash, 0, sizeof(bool) * 26);	// reset hash
		int next = 0;								// progress of the filling
		for (int i = 0; i < strlen(key_buf); i++) {
			if (key_buf[i] == 'J') continue;		// you know it
			if (hash[key_buf[i] - 'A']) continue;
			cipher_tbl[next++] = key_buf[i];
			hash[key_buf[i] - 'A'] = true;
		}
		for (char c = 'A'; c <= 'Z'; c++) {
			if (c == 'J') continue;				// yeah
			if (hash[c - 'A']) continue;
			cipher_tbl[next++] = c;
			hash[c - 'A'] = true;
		}
		assert(next == 25);

		// encrypting
		for (int i = 0; i < strlen(pt_buf); i+=2) {	// every 2-char in PT
			// clarify the positions of the two chars in the cipher_tbl
			int idx1, idx2;
			for (int j = 0; j < 25; j++) {
				if (cipher_tbl[j] == pt_buf[i]) idx1 = j;
				if (cipher_tbl[j] == pt_buf[i + 1]) idx2 = j;
			}
			assert(idx1 != idx2);			// no repeating chars in a pair is allowed
			if (idx1 / 5 == idx2 / 5) {		// same row
				int q = idx1 / 5;
				int r1 = idx1 % 5, r2 = idx2 % 5;
				r1 = (r1 + 1) % 5; r2 = (r2 + 1) % 5;
				fprintf(f_ct, "%c%c", cipher_tbl[q * 5 + r1], cipher_tbl[q * 5 + r2]);
			}
			else if (idx1 % 5 == idx2 % 5) {		// same column
				idx1 = (idx1 + 5) % 25;
				idx2 = (idx2 + 5) % 25;
				fprintf(f_ct, "%c%c", cipher_tbl[idx1], cipher_tbl[idx2]);
			}
			else {	// rectangular
				int q1 = idx1 / 5, r1 = idx1 % 5;
				int q2 = idx2 / 5, r2 = idx2 % 5;
				fprintf(f_ct, "%c%c", cipher_tbl[q1 * 5 + r2], cipher_tbl[q2 * 5 + r1]);
			}
		} // for - every 2-char in PT
		fprintf(f_ct, "\n");

		fscanf(f_pt, " %s", pt_buf);
	} // while - enumerate cases
	delete[] pt_buf;
	delete[] key_buf;


	fclose(f_pt);
	fclose(f_key);
	fclose(f_ct);
	return;
}

void Decryption() {
	//
	// file handles
	//
	char *fnm_buffer = new char[50];
	printf("Input the name of the file containing CT:\n");
	scanf(" %s", fnm_buffer);
	FILE *f_ct = fopen(fnm_buffer, "r");
	printf("Input the name of the file containing Keys:\n");
	scanf(" %s", fnm_buffer);
	FILE *f_key = fopen(fnm_buffer, "r");
	printf("Input the name of the file to which the PT is to be written:\n");
	scanf(" %s", fnm_buffer);
	FILE *f_pt = fopen(fnm_buffer, "w");
	delete[] fnm_buffer;
	//
	// process one by one
	//
	char *ct_buf = new char[201];
	char *key_buf = new char[51];
	char cipher_tbl[25];
	bool hash[26];
	fscanf(f_ct, " %s", ct_buf);
	while (!feof(f_ct)) {	// enumerate cases
		fscanf(f_key, " %s", key_buf);	// get the key

		// fill cipher_tbl
		memset(hash, 0, sizeof(bool) * 26);	// reset hash
		int next = 0;								// progress of the filling
		for (int i = 0; i < strlen(key_buf); i++) {
			if (key_buf[i] == 'J') continue;		// you know it
			if (hash[key_buf[i] - 'A']) continue;
			cipher_tbl[next++] = key_buf[i];
			hash[key_buf[i] - 'A'] = true;
		}
		for (char c = 'A'; c <= 'Z'; c++) {
			if (c == 'J') continue;				// yeah
			if (hash[c - 'A']) continue;
			cipher_tbl[next++] = c;
			hash[c - 'A'] = true;
		}
		assert(next == 25);

		// decrypting
		for (int i = 0; i < strlen(ct_buf); i += 2) {	// every 2-char in PT
			// clarify the positions of the two chars in the cipher_tbl
			int idx1, idx2;
			for (int j = 0; j < 25; j++) {
				if (cipher_tbl[j] == ct_buf[i]) idx1 = j;
				if (cipher_tbl[j] == ct_buf[i + 1]) idx2 = j;
			}
			assert(idx1 != idx2);			// no repeating chars in a pair is allowed
			if (idx1 / 5 == idx2 / 5) {		// same row
				int q = idx1 / 5;
				int r1 = idx1 % 5, r2 = idx2 % 5;
				r1 = (r1 - 1) % 5; r2 = (r2 - 1) % 5;
				if (r1 < 0) r1 += 5;
				if (r2 < 0) r2 += 5;
				fprintf(f_pt, "%c%c", cipher_tbl[q * 5 + r1], cipher_tbl[q * 5 + r2]);
			}
			else if (idx1 % 5 == idx2 % 5) {		// same column
				idx1 = (idx1 - 5) % 25;
				idx2 = (idx2 - 5) % 25;
				if (idx1 < 0) idx1 += 25;
				if (idx2 < 0) idx2 += 25;
				fprintf(f_pt, "%c%c", cipher_tbl[idx1], cipher_tbl[idx2]);
			}
			else {	// rectangular
				int q1 = idx1 / 5, r1 = idx1 % 5;
				int q2 = idx2 / 5, r2 = idx2 % 5;
				fprintf(f_pt, "%c%c", cipher_tbl[q1 * 5 + r2], cipher_tbl[q2 * 5 + r1]);
			}
		} // for - every 2-char in PT
		fprintf(f_pt, "\n");

		fscanf(f_ct, " %s", ct_buf);
	} // while - enumerate cases
	delete[] ct_buf;
	delete[] key_buf;


	fclose(f_ct);
	fclose(f_key);
	fclose(f_pt);
	return;
}