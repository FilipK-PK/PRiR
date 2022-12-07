#include <stdio.h>
#include <stdlib.h>

extern const unsigned char rCorn[], sBox[],rsBox[], mul2[], mul3[];
extern const unsigned char mul9[], mulB[], mulD[], mulE[];

void addRoudKey(unsigned char* block, unsigned char** keys, int index);
void shifrRow(unsigned char* block);
void subByte(unsigned char* block);
void mixCloumns(unsigned char* block);
void rShifrRow(unsigned char* block);
void rSubByte(unsigned char* block);
void rMixColumns(unsigned char* block);

void encryptBlock(unsigned char* block, unsigned char** keys, int round, int isDecrypt) {
	int i;

	if (isDecrypt) {
		addRoudKey(block, keys, round);
	}
	else {
		addRoudKey(block, keys, 0);
	}

	for (i = 1; i < round; i++) {
		if (isDecrypt) {
			rShifrRow(block);
			rSubByte(block);
			addRoudKey(block, keys, round - i);
			rMixColumns(block);
			
		}
		else {
			subByte(block);
			shifrRow(block);
			mixCloumns(block);
			addRoudKey(block, keys, i);
		}
	}

	if (isDecrypt) {
		rShifrRow(block);
		rSubByte(block);
		addRoudKey(block, keys, 0);
	}
	else {
		subByte(block);
		shifrRow(block);
		addRoudKey(block, keys, i);
	}
}

void addRoudKey(unsigned char* block, unsigned char** keys, int index) {
	for (int i = 0; i < 16; i++) {
		block[i] ^= keys[i / 4][4 * index + i % 4];
	}
}

void subByte(unsigned char* block) {
	for (int i = 0; i < 16; i++) {
		block[i] = sBox[block[i]];
	}
}

void rSubByte(unsigned char* block) {
	for (int i = 0; i < 16; i++) {
		block[i] = rsBox[block[i]];
	}
}

void shifrRow(unsigned char* block) {
	char c;

	c = block[4];
	block[4] = block[5];
	block[5] = block[6];
	block[6] = block[7];
	block[7] = c;

	c = block[8];
	block[8] = block[10];
	block[10] = c;

	c = block[9];
	block[9] = block[11];
	block[11] = c;

	c = block[15];
	block[15] = block[14];
	block[14] = block[13];
	block[13] = block[12];
	block[12] = c;
}

void rMixColumns(unsigned char* block)
{
	unsigned char res[16];

	res[0] = mulE[block[0]] ^ mulB[block[4]] ^ mulD[block[8]] ^ mul9[block[12]];
	res[1] = mulE[block[1]] ^ mulB[block[5]] ^ mulD[block[9]] ^ mul9[block[13]];
	res[2] = mulE[block[2]] ^ mulB[block[6]] ^ mulD[block[10]] ^ mul9[block[14]];
	res[3] = mulE[block[3]] ^ mulB[block[7]] ^ mulD[block[11]] ^ mul9[block[15]];

	res[4] = mul9[block[0]] ^ mulE[block[4]] ^ mulB[block[8]] ^ mulD[block[12]];
	res[5] = mul9[block[1]] ^ mulE[block[5]] ^ mulB[block[9]] ^ mulD[block[13]];
	res[6] = mul9[block[2]] ^ mulE[block[6]] ^ mulB[block[10]] ^ mulD[block[14]];
	res[7] = mul9[block[3]] ^ mulE[block[7]] ^ mulB[block[11]] ^ mulD[block[15]];

	res[8] = mulD[block[0]] ^ mul9[block[4]] ^ mulE[block[8]] ^ mulB[block[12]];
	res[9] = mulD[block[1]] ^ mul9[block[5]] ^ mulE[block[9]] ^ mulB[block[13]];
	res[10] = mulD[block[2]] ^ mul9[block[6]] ^ mulE[block[10]] ^ mulB[block[14]];
	res[11] = mulD[block[3]] ^ mul9[block[7]] ^ mulE[block[11]] ^ mulB[block[15]];

	res[12] = mulB[block[0]] ^ mulD[block[4]] ^ mul9[block[8]] ^ mulE[block[12]];
	res[13] = mulB[block[1]] ^ mulD[block[5]] ^ mul9[block[9]] ^ mulE[block[13]];
	res[14] = mulB[block[2]] ^ mulD[block[6]] ^ mul9[block[10]] ^ mulE[block[14]];
	res[15] = mulB[block[3]] ^ mulD[block[7]] ^ mul9[block[11]] ^ mulE[block[15]];

	for (int i = 0; i < 16; i++) {
		block[i] = res[i];
	}
}

void mixCloumns(unsigned char* block) {
	unsigned char res[16];

	res[0] = mul2[block[0]] ^ mul3[block[4]] ^ block[8] ^ block[12];
	res[1] = mul2[block[1]] ^ mul3[block[5]] ^ block[9] ^ block[13];
	res[2] = mul2[block[2]] ^ mul3[block[6]] ^ block[10] ^ block[14];
	res[3] = mul2[block[3]] ^ mul3[block[7]] ^ block[11] ^ block[15];

	res[4] = block[0] ^ mul2[block[4]] ^ mul3[block[8]] ^ block[12];
	res[5] = block[1] ^ mul2[block[5]] ^ mul3[block[9]] ^ block[13];
	res[6] = block[2] ^ mul2[block[6]] ^ mul3[block[10]] ^ block[14];
	res[7] = block[3] ^ mul2[block[7]] ^ mul3[block[11]] ^ block[15];

	res[8] = block[0] ^ block[4] ^ mul2[block[8]] ^ mul3[block[12]];
	res[9] = block[1] ^ block[5] ^ mul2[block[9]] ^ mul3[block[13]];
	res[10] = block[2] ^ block[6] ^ mul2[block[10]] ^ mul3[block[14]];
	res[11] = block[3] ^ block[7] ^ mul2[block[11]] ^ mul3[block[15]];

	res[12] = mul3[block[0]] ^ block[4] ^ block[8] ^ mul2[block[12]];
	res[13] = mul3[block[1]] ^ block[5] ^ block[9] ^ mul2[block[13]];
	res[14] = mul3[block[2]] ^ block[6] ^ block[10] ^ mul2[block[14]];
	res[15] = mul3[block[3]] ^ block[7] ^ block[11] ^ mul2[block[15]];

	for (int i = 0; i < 16; i++) {
		block[i] = res[i];
	}
}

void rShifrRow(unsigned char* block) {
	char c;

	c = block[7];
	block[7] = block[6];
	block[6] = block[5];
	block[5] = block[4];
	block[4] = c;

	c = block[8];
	block[8] = block[10];
	block[10] = c;

	c = block[9];
	block[9] = block[11];
	block[11] = c;

	c = block[12];
	block[12] = block[13];
	block[13] = block[14];
	block[14] = block[15];
	block[15] = c;
}

void keyExpansion(int round, unsigned char** newKeys) {
	unsigned char index[4] = { 0, 0, 0, 0 };

	int jump = round - 6;
	int lenKey = (round * 4) / jump;

	if (((round+1) * 4) % jump) {
		lenKey++;
	}

	for (int i = 1; i < lenKey + 1; i++) {
		newKeys[0][i * jump] = newKeys[1][i * jump - 1];
		newKeys[1][i * jump] = newKeys[2][i * jump - 1];
		newKeys[2][i * jump] = newKeys[3][i * jump - 1];
		newKeys[3][i * jump] = newKeys[0][i * jump - 1];

		for (int j = 0; j < 4; j++) {
			newKeys[j][i * jump] = sBox[newKeys[j][i * jump]];
		}

		index[0] = rCorn[i - 1];

		for (int j = 0; j < 4; j++) {
			newKeys[j][i * jump] = newKeys[j][(i - 1) * jump] ^ newKeys[j][i * jump] ^ index[j];
		}

		for (int j = 0; j < 4; j++) {
			for (int k = 1; k < jump; k++) {
				newKeys[j][i * jump + k] = newKeys[j][(i - 1) * jump + k] ^ newKeys[j][i * jump + k - 1];
			}
		}
	}
}
