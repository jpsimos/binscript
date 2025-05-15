/*
 * Crc32.h
 *
 *  Created on: Jan 20, 2024
 *      Author: jacob psimos
 */

#ifndef CRC32_H_
#define CRC32_H_

#include <stdio.h>
#include <stdint.h>

uint32_t CalculateCrc32(const void* data, const size_t len, const uint32_t polynomial);
uint32_t RecalculateCrc32(const void* data, const size_t len, const uint32_t crc, const uint32_t polynomial);

#endif /* CRC32_H_ */
