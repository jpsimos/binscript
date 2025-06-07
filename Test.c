/*
	Test.c
	Simple unit test program for binscript crypto components.
	Author Jacob A Psimos
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "Crc32.h"
#include "Crypto.h"

int main(int argc, char* argv[])
{
	const char* testData = "A rhino on the Sahara.";
	const char* testDataKey = "Is a force";
	const size_t testDataLength = strlen(testData);
	const size_t testDataKeyLength = strlen(testDataKey);
	const uint32_t testDataChecksum = CalculateCrc32(testData, testDataLength, 0xEDB88320U);
	char testDataCopy[100];
	char encryptedTestData[100];
	char decryptedTestData[100];
	R4CKey encryptoKey;
	R4CKey otherEncryptoKey;
	
	if(0xCEED4F9DU != testDataChecksum)
	{
		puts("Fail checksum");
		return 1;
	}
	
	R4CDeriveKey(&encryptoKey, testData, testDataKeyLength);
	R4CDeriveKey(&otherEncryptoKey, testData, testDataKeyLength);
	
	memcpy(testDataCopy, testData, strlen(testData));
	R4CCrypt(&encryptoKey, testDataCopy, testDataLength, encryptedTestData);
	R4CCrypt(&otherEncryptoKey, encryptedTestData, testDataLength, decryptedTestData);
	
	if(memcmp(testData, decryptedTestData, testDataLength))
	{
		puts("Fail crypto");
		return 2;
	}
	
	puts("Pass");

	return 0;
}
