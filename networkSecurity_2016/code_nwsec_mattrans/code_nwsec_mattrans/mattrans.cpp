// matrix transposition cipher by Yang Wang, B00711141
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstring>

void Encryption();
void Decryption();

int main() {
	// encryption or decryption
	printf("Matrix Transposition Cipher. Input 1 for encryption");
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
	// encrypt each line of PT with the corresponding key
	//
	char *work_buf = new char[201];
	fscanf(f_pt, " %s", work_buf);
	while (!feof(f_pt)) {	// enumerate cases
		int txt_len = strlen(work_buf);
		// read the key
		int key_len = 0;
		fscanf(f_key, " %d", &key_len);
		int *keys = new int[key_len];
		for (int i = 0; i < key_len; i++) fscanf(f_key, " %d", &keys[i]);
		// encrypting
		int c_clm = key_len;
		int c_row = txt_len / c_clm;
		for (int i = 0; i < key_len; i++) {	// enumerate key elements
			keys[i]--;	// make keys zero-based
			for (int j = 0; j < c_row; j++) {	// enumerate rows
				fprintf(f_ct, "%c", work_buf[j * c_clm + keys[i]]);
			}
		}
		fprintf(f_ct, "\n");

		delete[] keys;
		fscanf(f_pt, " %s", work_buf);
	}
	delete[] work_buf;

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
	// decrypt each line of CT with the corresponding key
	//
	char *work_buf = new char[201];
	char *buf2 = new char[201];
	fscanf(f_ct, " %s", work_buf);
	while (!feof(f_ct)) {	// enumerate cases
		int text_len = strlen(work_buf);
		// read the key
		int key_len = 0;
		fscanf(f_key, " %d", &key_len);
		int *key = new int[key_len];
		for (int i = 0; i < key_len; i++) fscanf(f_key, " %d", &key[i]);
		// decrypting
		int c_row = key_len;
		int c_clm = text_len / key_len;
		for (int i = 0; i < key_len; i++) {
			key[i]--;
			for (int j = 0; j < c_clm; j++)
				buf2[key[i] * c_clm + j] = work_buf[i*c_clm + j];
		}
		buf2[text_len] = '\0';
		for (int i = 0; i < c_clm; i++)
			for (int j = 0; j < c_row; j++) fprintf(f_pt, "%c", buf2[j*c_clm + i]);
		fprintf(f_pt, "\n");

		delete[] key;
		fscanf(f_ct, " %s", work_buf);
	}

	delete[] work_buf;
	delete[] buf2;

	fclose(f_ct);
	fclose(f_key);
	fclose(f_pt);
	return;
}