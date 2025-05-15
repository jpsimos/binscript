/*
 * Crc32.c
 *
 *  Created on: Jan 20, 2024
 *      Author: jacob psimos
 */

#include <stdio.h>
#include <stdint.h>
#include "Crc32.h"

static inline uint32_t DoCalculateCrc32(const unsigned char dat[], const size_t len, uint32_t crc, const uint32_t polynomial)
{
	for (off_t i = 0; i < len; i++) {
		uint32_t value = (crc ^ dat[i]) & 0xFF;
		value = value & 1 ? (value >> 1) ^ polynomial : value >> 1;
		value = value & 1 ? (value >> 1) ^ polynomial : value >> 1;
		value = value & 1 ? (value >> 1) ^ polynomial : value >> 1;
		value = value & 1 ? (value >> 1) ^ polynomial : value >> 1;
		value = value & 1 ? (value >> 1) ^ polynomial : value >> 1;
		value = value & 1 ? (value >> 1) ^ polynomial : value >> 1;
		value = value & 1 ? (value >> 1) ^ polynomial : value >> 1;
		value = value & 1 ? (value >> 1) ^ polynomial : value >> 1;
		crc = value ^ crc >> 8;
	}
	crc ^= ~0U;
	return crc;
}

uint32_t CalculateCrc32(const void* data, const size_t len, const uint32_t polynomial)
{
	return DoCalculateCrc32((const unsigned char*)data, len, ~0U, polynomial);
}

uint32_t RecalculateCrc32(const void* data, const size_t len, const uint32_t crc, const uint32_t polynomial)
{
	return DoCalculateCrc32((const unsigned char*)data, len, crc ^ ~0U, polynomial);
}
