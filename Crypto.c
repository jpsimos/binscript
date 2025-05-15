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
	size_t index1 = 0U;
	size_t index2 = 0U;

	skey->x = 0U;
	skey->j = 0U;
	skey->y = 0U;
	skey->k = 0U;

	if(keyLength > 16)
		keyLength = 16;

	for(size_t i = 0; i < 256U; i++)
	{
		skey->keyState[i] = (uint8_t)i;
	}

	for(size_t i = 0; i < 256U; i++)
	{
		index2 = (index2 + pkey[index1] + skey->keyState[i]) % 256U;
		skey->keyState[i] ^= skey->keyState[index2];
		skey->keyState[index2] ^= skey->keyState[i];
		skey->keyState[i] ^= skey->keyState[index2];
		index1 = (index1 + 1U) % keyLength;
	}
}

void R4CCrypt(struct R4CKey* skey, void* data, size_t dataLength, void* dataOut)
{
	uint8_t* dataPtrOut = (uint8_t*)(dataOut ? dataOut : data);
	uint8_t* dataPtrIn = (uint8_t*)data;
	for(size_t i = 0U; i < dataLength; i++)
	{
		skey->x = (skey->x + 1U) % 256U;
		skey->y = (skey->keyState[skey->x] + skey->y) % 256U;
		skey->keyState[skey->x] ^= skey->keyState[skey->y];
		skey->keyState[skey->y] ^= skey->keyState[skey->x];
		skey->keyState[skey->x] ^= skey->keyState[skey->y];
		dataPtrOut[i] = dataPtrIn[i] ^ skey->keyState[skey->j];
		skey->j = (skey->j + 1U) % 256U;
	}
}
