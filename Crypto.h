/*
 * Crypto.h
 *
 *  Created on: Jan 15, 2024
 *      Author: jacob a psimos
 */

#ifndef __Crypto_h
#define __Crypto_h

#include <stdio.h>
#include <stdint.h>

typedef struct R4CKey
{
	uint32_t x;
	uint32_t j;
	uint32_t y;
	uint32_t k;
	uint8_t keyState[256];
	union
	{
		uint32_t keyDwords[4];
		uint8_t keyBytes[16];
	};
} R4CKey;

void R4CDeriveKey(struct R4CKey* skey, const void* key, size_t keyLength);
void R4CCrypt(struct R4CKey* skey, void* data, size_t dataLength, void* dataOut);

#endif
