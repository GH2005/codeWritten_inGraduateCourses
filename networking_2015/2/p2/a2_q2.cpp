// bit stuffing simulation.
#include <cstdio>
#include <cassert>

int GetStuffedBits(char *fileName, char *(&stuffedBuffer), int &bytesStuffed,
										int &bytesOrig) {
	// returns the number of times of bit stufing;
	int cStuffs = 0;

	// load the file into the memory.
	FILE *pf = fopen(fileName, "rb"); assert(pf != nullptr);
	fseek(pf, 0, SEEK_END);
	bytesOrig = ftell(pf);
	fseek(pf, 0, SEEK_SET);
	char *buffer = new char[bytesOrig];
	assert(fread(buffer, bytesOrig, 1, pf) == 1);
	fclose(pf);
	
	// convert the data to stuffed bits.
	stuffedBuffer = new char[(bytesOrig / 5 + 1) * 6];	// a sufficient buffer.
	int iOrig = 0;
	bytesStuffed = 0;
	int cSuccOnes = 0;
	char currByte = 0x00; int cCurrBits = 0;
	while (iOrig < bytesOrig) {										// push on with original bytes.
		for (int i = 0; i < sizeof(char) * 8; i++) {			// push on with every bit of the byte.
			currByte = currByte << 1;								// empty the last bit of  currbyte.
			char theBit = buffer[iOrig] & 0x80;				// bit-AND by 1000 0000.
			if (theBit < 0) currByte = currByte ^ 0x01;	// bit-XOR to save bit 1.

			// updates.
			if (theBit < 0) cSuccOnes++;				// it's a bit of 1.
			else cSuccOnes = 0;								// it's a bit of 0.

			cCurrBits++;
			if (cSuccOnes == 5 && cCurrBits == sizeof(char) * 8) {					// cSuccOnes == 5.
				stuffedBuffer[bytesStuffed++] = currByte;
				cCurrBits = 1;
				currByte = 0x00;
				cSuccOnes = 0;
				cStuffs++;
			}
			else if (cSuccOnes == 5 && cCurrBits == sizeof(char) * 8 - 1) {
				currByte = currByte << 1;
				stuffedBuffer[bytesStuffed++] = currByte;
				cCurrBits = 0;
				currByte = 0x00;
				cSuccOnes = 0;
				cStuffs++;
			}
			else if (cSuccOnes == 5) {
				currByte = currByte << 1;
				cCurrBits++;
				cSuccOnes = 0;
				cStuffs++;
			}
			else if (cCurrBits == sizeof(char) * 8) {													// cSuccOnes < 5.
				stuffedBuffer[bytesStuffed++] = currByte;
				cCurrBits = 0;
				currByte = 0x00;
			}

			buffer[iOrig] = buffer[iOrig] << 1;
		}
		iOrig++;
	}

	// if some bits remains to be added.
	if (cCurrBits > 0) {
		currByte = currByte << (sizeof(char) * 8 - cCurrBits);
		stuffedBuffer[bytesStuffed++] = currByte;
	}

	delete[] buffer;
	return cStuffs;
}

int RestoreText(char *fileName, char *stuffedBuffer, int bytesStuffed) {
	// return the number of bytes restored.
	int cRes = 0;
	char *resBuffer = new char[bytesStuffed];

	// go through every bit in stuffedBuffer.
	char theByte = 0x00; int cBits = 0;
	int cSuccOnes = 0; bool ignoreBit = false;
	for (int i = 0; i < bytesStuffed; i++) {
		for (int j = 0; j < sizeof(char) * 8; j++) {
			char theBit = stuffedBuffer[i] & 0x80;
			if (ignoreBit) {
				assert(theBit == 0);
				ignoreBit = false;
				stuffedBuffer[i] = stuffedBuffer[i] << 1;
				continue; 
			}

			theByte = theByte << 1;									// empty the last bit of theByte.
			if (theBit < 0) theByte = theByte ^ 0x01;	// bit-XOR to save bit 1.

			// update.
			// successive ones.
			if (theBit < 0) cSuccOnes++;
			else cSuccOnes = 0;
			if (cSuccOnes == 5) ignoreBit = true;
			// count of bits.
			cBits++;
			if (cBits == sizeof(char) * 8) {
				resBuffer[cRes++] = theByte;
				cBits = 0;
				theByte = 0x00;
			}

			stuffedBuffer[i] = stuffedBuffer[i] << 1;
		}
	}

	// write to the file.
	FILE *pf = fopen(fileName, "wb"); assert(pf != nullptr);
	assert(fwrite(resBuffer, cRes, 1, pf) == 1);
	fclose(pf);

	delete[] resBuffer;
	return cRes;
}

int main() {
	char *stuffedBuffer; int bytesStuffed, bytesOrig;
	int cStuffs = GetStuffedBits("Gibberish.txt", stuffedBuffer, bytesStuffed,
															bytesOrig);
	assert(RestoreText("Restored.txt", stuffedBuffer, bytesStuffed) == bytesOrig);
	delete[] stuffedBuffer;
	return 0;
}