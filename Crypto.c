/*
 * Crypto.c
 *
 *  Created on: Jan 15, 2024
 *      Author: jacob a psimos
 */

#include <stdio.h>
#include <stdint.h>
#include "Crypto.h"

void R4CDeriveKey(struct R4CKey* skey, const void* key, size_t keyLength)
{
	const uint8_t* pkey = (const uint8_t*)key;
	size_t index1 = 0;
	size_t index2 = 0;

	skey->x = 0;
	skey->j = 0;
	skey->y = 0;
	skey->k = 0;

	if(keyLength > 16)
		keyLength = 16;

	for(size_t i = 0; i < 256; i++)
	{
		skey->keyState[i] = (uint8_t)i;
	}

	for(size_t i = 0; i < 256; i++)
	{
		index2 = (index2 + pkey[index1] + skey->keyState[i]) % 256;
		uint8_t t = skey->keyState[i];
		skey->keyState[i] = skey->keyState[index2];
		skey->keyState[index2] = t;
		index1 = (index1 + 1) % keyLength;
	}
}

void R4CCrypt(struct R4CKey* skey, void* data, size_t dataLength, void* dataOut)
{
	uint8_t* dataPtrOut = (uint8_t*)(dataOut ? dataOut : data);
	uint8_t* dataPtrIn = (uint8_t*)data;
	for(size_t i = 0; i < dataLength; i++)
	{
		skey->x = (skey->x + 1) % 256;
		skey->y = (skey->keyState[skey->x] + skey->y) % 256;
		uint8_t t = skey->keyState[skey->x];
		skey->keyState[skey->x] = skey->keyState[skey->y];
		skey->keyState[skey->y] = t;
		uint8_t in = dataPtrIn[i];
		dataPtrOut[i] = in ^ skey->keyState[skey->j];
		skey->j = (skey->j + 1) % 256;
	}
}
