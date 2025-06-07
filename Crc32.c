/*
 * Crc32.c
 *
 *  Created on: Jan 20, 2024
 *      Author: jacob psimos
 */

#include <stdio.h>
#include <stdint.h>
#include "Crc32.h"

static inline uint32_t GetTableValue(uint32_t tableOffset, const uint32_t polynomial)
{
	uint32_t tableValue = 0;
	uint32_t crc;

	for(uint32_t c = 0; c <= tableOffset; c++)
	{
		crc = 0;
		
		crc = ((crc ^ c) & 1) ? ((crc >> 1) ^ polynomial) : (crc >> 1);
		crc = ((crc ^ (c >> 1)) & 1) ? ((crc >> 1) ^ polynomial) : (crc >> 1);
		crc = ((crc ^ (c >> 2)) & 1) ? ((crc >> 1) ^ polynomial) : (crc >> 1);
		crc = ((crc ^ (c >> 3)) & 1) ? ((crc >> 1) ^ polynomial) : (crc >> 1);
		crc = ((crc ^ (c >> 4)) & 1) ? ((crc >> 1) ^ polynomial) : (crc >> 1);
		crc = ((crc ^ (c >> 5)) & 1) ? ((crc >> 1) ^ polynomial) : (crc >> 1);
		crc = ((crc ^ (c >> 6)) & 1) ? ((crc >> 1) ^ polynomial) : (crc >> 1);
		crc = ((crc ^ (c >> 7)) & 1) ? ((crc >> 1) ^ polynomial) : (crc >> 1);

		tableValue = crc;
	}

	return tableValue;
}

uint32_t CalculateCrc32(const void* data, const size_t dataSizeBytes, const uint32_t polynomial)
{
	return RecalculateCrc32(CRC32_DEFAULT, data, dataSizeBytes, polynomial);
}

uint32_t RecalculateCrc32(uint32_t crc, const void* data, const size_t dataSizeBytes, const uint32_t polynomial)
{
	const uint8_t* dataPtr = (const uint8_t*)data;
	uint32_t tableIndex;
	size_t dataIndex;

	if(CRC32_DEFAULT != crc)
	{
		crc ^= CRC32_XOR;
	}

	for(dataIndex = 0; dataIndex < dataSizeBytes; dataIndex++)
	{
		tableIndex = (crc ^ (uint32_t)dataPtr[dataIndex]) & 0xFF;
		crc = (crc >> 8) ^ GetTableValue(tableIndex, polynomial);
	}

	crc ^= CRC32_XOR;

	return crc;
}
