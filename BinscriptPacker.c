/*
 * BinscriptPacker.c
 *
 *  Created on: Jan 15, 2024
 *      Author: jacob a psimos
 */

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/mman.h>
#include "Crc32.h"
#include "Crypto.h"

// Prototypes
void PrintHelp();
int streq(const char *a, const char *b);

int main(int argc, char* argv[])
{
	char* packedProgramPath = NULL;
	char* outputFilePath = NULL;
	char  binscriptPath[PATH_MAX];
	int packedProgramFd;
	int binscriptFd;
	int outputFileFd;
	ssize_t packedProgramLength;
	ssize_t binscriptLength;
	uint8_t* packedProgramMmio;
	uint8_t* binscriptMmio;
	struct R4CKey skey;
	uint32_t pkey[4];

	if(1 == argc)
	{
		PrintHelp();
		exit(0);
	}

	strncpy(binscriptPath, "binscript", 10);

	for(int argi = 1; argi < argc; argi++)
	{
		char* arg = argv[argi];
		char* nextArg = argi < argc - 1 ? argv[argi + 1] : NULL;

		if(streq(arg, "-h") || streq(arg, "--help"))
		{
			PrintHelp();
			exit(0);
		}

		if(nextArg)
		{
			if(streq(arg, "-b") || streq(arg, "--binscript"))
			{
				strncpy(binscriptPath, nextArg, PATH_MAX);
				binscriptPath[PATH_MAX - 1] ^= binscriptPath[PATH_MAX - 1];
			}
			else if(streq(arg, "-o") || streq(arg, "--output"))
			{
				outputFilePath = nextArg;
			}
		}
		else
		{
			packedProgramPath = arg;
		}
	}

	if(NULL == outputFilePath)
	{
		fputs("Missing required -o, --output file path.\n", stderr);
		exit(1);
	}

	if(NULL == packedProgramPath)
	{
		fputs("Missing required program path.\n", stderr);
		exit(1);
	}

	binscriptFd = open(binscriptPath, O_RDONLY);

	if(-1 == binscriptFd)
	{
		err(errno, "%s:%d", __FILE__, __LINE__);
	}

	binscriptLength = lseek(binscriptFd, 0L, SEEK_END);

	if(-1 == binscriptLength)
	{
		err(errno, "%s:%d", __FILE__, __LINE__);
	}

	binscriptMmio = (uint8_t*)mmap(NULL, (size_t)binscriptLength, PROT_READ, MAP_PRIVATE, binscriptFd, 0);

	if(MAP_FAILED == binscriptMmio)
	{
		err(errno, "%s:%d", __FILE__, __LINE__);
	}

	close(binscriptFd);

	outputFileFd = open(outputFilePath, O_RDWR | O_CREAT | O_TRUNC,
			S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

	if(-1 == outputFileFd)
	{
		err(errno, "%s:%d", __FILE__, __LINE__);
	}

	packedProgramFd = open(packedProgramPath, O_RDONLY);

	if(-1 == packedProgramFd)
	{
		err(errno, "%s:%d", __FILE__, __LINE__);
	}

	packedProgramLength = lseek(packedProgramFd, 0L, SEEK_END);

	if(-1 == packedProgramLength)
	{
		err(errno, "%s:%d", __FILE__, __LINE__);
	}

	packedProgramMmio = (uint8_t*)mmap(NULL, (size_t)packedProgramLength, PROT_READ, MAP_PRIVATE, packedProgramFd, 0);

	if(MAP_FAILED == packedProgramMmio)
	{
		err(errno, "%s:%d", __FILE__, __LINE__);
	}

	close(packedProgramFd);

	write(outputFileFd, binscriptMmio, (size_t)binscriptLength);

	pkey[0] = CalculateCrc32(&binscriptMmio[0], binscriptLength, 0xEDB88320U);
	pkey[1] = CalculateCrc32(&binscriptMmio[0], binscriptLength, 0xAE9F7540U);
	pkey[2] = CalculateCrc32(&binscriptMmio[0], binscriptLength, 0xC8605A10U);
	pkey[3] = pkey[0] + pkey[1] + pkey[2] + packedProgramLength;
	R4CDeriveKey(&skey, &pkey[0], 16);

	for(off_t i = 0; i < packedProgramLength; i++)
	{
		uint8_t next;
		R4CCrypt(&skey, &packedProgramMmio[i], 1, &next);
		write(outputFileFd, &next, 1);
	}

	if(STDOUT_FILENO != outputFileFd)
	{
		close(outputFileFd);
	}

	return 0;
}

void PrintHelp()
{
	puts(
			"binscript-packer - packs a program into binscript.\n"
			"usage: ./binscript-packer [file]\n"
			"  -b     --binscript [location]  Location of the binscript program.\n"
			"                                 Defaults to ./binscript\n"
			"  -o     --output [file]         Output the packed binscript to FILE.\n"
			"  -h     --help                  Show help and exit.\n"
			"                                 Omitted -o, --output means stdout (default).\n"
			"\n"
			"Written by Jacob A Psimos\n"
			"Compiled on " __DATE__ " " __TIME__
	);
}

int streq(const char *a, const char *b)
{
	int equal = (a != b && a != NULL && b != NULL);
	while (equal && a != b)
	{
		if (*a != *b)
		{
			equal = 0;
		}
		else if (*a++ == '\0' || *b++ == '\0')
		{
			break;
		}
	}
	return equal;
}
