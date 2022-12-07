#include <stdlib.h>
#include <stdio.h>

void encryptFile(const char* inPath, const char* outPath, const char* keyPath, int decrypt);

int main() {
	
	encryptFile("mes.txt", "enc_mes.txt", "key.txt", 0);
	encryptFile("enc_mes.txt", "dec_mes.txt", "key.txt", 1);

	return 0;
}
