#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

extern void encryptBlock(unsigned char* block, unsigned char** keys, int round, int isDecrypr);
extern void keyExpansion(int round, unsigned char** newKeys);

void deleteKeys(unsigned char** keys);
int allockKeys(const char* keyPath, unsigned char*** keys);
void addLostChar(unsigned char* block, int len);
int delLostChar(unsigned char* block);
void openFile(const char* path, const char* typeOpen, FILE** file);
int initBlocks(const char* path, unsigned char*** blocks);
void encryptBlocks(unsigned char*** blocks, unsigned char*** keys, int lenBlocks, int round, int isDecrypt);
void deleteBlocks(unsigned char*** blocks, int len);
void writeBlocks(const char* path, unsigned char*** blocks, int lenBlocks);

void encryptFile(const char* inPath, const char* outPath, const char * keyPath, int decrypt) {
	unsigned char ** keys, ** blocks;
	int round, lenBlocks;

	round = allockKeys(keyPath, &keys);
	keyExpansion(round, keys);

	lenBlocks = initBlocks(inPath, &blocks);

	encryptBlocks(&blocks, &keys, lenBlocks, round, decrypt);
	writeBlocks(outPath, &blocks, lenBlocks);

	deleteKeys(keys);
	deleteBlocks(&blocks, lenBlocks);
}

void writeBlocks(const char* path, unsigned char*** blocks, int lenBlocks) {
	FILE *file;
	int len;

	openFile(path, "wb", &file);

	for (int i = 0; i < lenBlocks; i++) {
		len = delLostChar((*blocks)[i]);
		fwrite((*blocks)[i], sizeof(unsigned char), len, file);
	}

	fclose(file);
}

void deleteBlocks(unsigned char*** blocks, int len) {
	if (*blocks) {
		for (int i = 0; i < len; i++) {
			if ((*blocks)[i]) {
				free((*blocks)[i]);
			}
			else {
				printf("ostrzezenie, czyszczenie pustego wiersza\n");
			}
		}

		free(*blocks);
	}
	else {
		printf("ostrzezenie czyszczenie pustej tablicy\n");
	}
}

void encryptBlocks(unsigned char *** blocks, unsigned char *** keys, int lenBlocks, int round, int isDecrypt) {
	for (int i = 0; i < lenBlocks; i++) {
		encryptBlock((*blocks)[i], *keys, round, isDecrypt);
	}
}

int initBlocks(const char * path, unsigned char*** blocks) {
	FILE* file;
	int len, ind = 0, lenBlocks = 0;
	char block[16];

	openFile(path, "rb", &file);

	while ((len = (int)fread(block, sizeof(unsigned char), 16, file)) > 0){
		lenBlocks++;
	}

	fclose(file);

	*blocks = malloc(lenBlocks * sizeof(unsigned char*));

	if (*blocks == NULL) {
		printf("B³ad alokacji pamieci\n");
		exit(-1);
	}

	for (int i = 0; i < lenBlocks; i++) {
		(*blocks)[i] = malloc(16 * sizeof(unsigned char));
	}

	openFile(path, "rb", & file);

	while ((len = (int)fread((*blocks)[ind], sizeof(unsigned char), 16, file)) > 0) {

		for (int i = len; i < 16; i++) {
			(*blocks)[ind][i] = '\0';
		}

		ind++;
	}

	fclose(file);

	return lenBlocks;
}

void openFile(const char* path, const char * typeOpen, FILE** file) {

	*file = fopen(path, typeOpen);

	if (*file == NULL) {
		printf("Blad otwarcia pliku '%s'\n", path);
		exit(1);
	}
}

void addLostChar(unsigned char*block, int len) {
	for (int i = len; i < 16; i++) {
		block[i] = '\0';
	}
}

int delLostChar(unsigned char* block) {
	int len = 16;

	for (int i = 15; block[i] == '\0'; i--) {
		len--;
	}

	return len;
}

int allockKeys(const char * keyPath, unsigned char*** keys) {
	FILE* keyFile;
	int keyLen, round, colKeys;
	char key[32];

	if ((keyFile = fopen(keyPath, "rb")) == NULL) {
		printf("Blad otwarcia pliku '%s'\n", keyPath);
		exit(2);
	}
	else {
		keyLen = (int)fread(key, sizeof(unsigned char), 32, keyFile);
		fclose(keyFile);
	}

	round = keyLen / 4 + 6;
	colKeys = (keyLen / 4) * (keyLen / 4 + 7);

	*keys = malloc(4 * sizeof(unsigned char*));

	if (keys == NULL) {
		printf("B³ad alokacji pamiêci dla kolumn kluczy\n");
		exit(-1);
	}

	for (int i = 0; i < 4; i++) {
		(*keys)[i] = (unsigned char*)malloc(colKeys * sizeof(unsigned char));

		if (keys[i] == NULL) {
			printf("B³ad alokacji pamiêci dla wierszy kluczy\n");
			exit(-1);
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < round - 6; j++) {
			(*keys)[i][j] = key[i * 4 + j];
		}
	}

	return round;
}

void deleteKeys(unsigned char** keys) {
	if (keys) {
		for (int i = 0; i < 4; i++) {
			if (keys[i]) {
				free(keys[i]);
			}
			else {
				printf("Ostrzezenie, proba zwolnienia pustaej pamieci wierszy kluczy\n");
			}
		}

		free(keys);
	}
	else {
		printf("Ostrzezenie, proba zwolnienia pustaej pamieci kolumn kluczy\n");
	}
}
