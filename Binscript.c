/*
 * Binscript.c
 *
 *  Created on: Jan 14, 2024
 *      Author: jacob a psimos
 */

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <linux/limits.h>
#include <linux/elf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <signal.h>
#include <unistd.h>
#include "Crc32.h"
#include "Crypto.h"

#ifdef __LP64__
	typedef Elf64_Ehdr ElfHeader;
#else
	typedef Elf32_Ehdr ElfHeader;
#endif


int main(int argc, char* argv[], char* envp[])
{
	ElfHeader elfHeader;
	char binscriptRealPath[PATH_MAX];
	ssize_t binscriptRealPathLength;
	ssize_t imageLength;
	ssize_t binscriptLength;
	ssize_t packedProgramLength;
	int vmFd;
	int binscriptFd;
	int packedProgramFd;
	int flags;
	uint8_t* binscriptMmio;
	uint8_t* packedProgramMmio;
	struct R4CKey skey;
	uint32_t pkey[4];

	errno = 0;
	flags = 0;
	
	// Determine if memfd_noexec is set and exit if so.
	// Then get self's path and begin the unpacking process.
	// Once everything checksout, exec the unpacked program.
	
	vmFd = open("/proc/sys/vm/memfd_noexec", O_RDONLY);

	if(-1 != vmFd)
	{
		if(1 == read(vmFd, &flags, 1))
		{
			flags -= '0';
		}
		close(vmFd);
	}
	
	if(flags)
	{
		exit(1);
	}

	binscriptRealPathLength = readlink("/proc/self/exe", binscriptRealPath, sizeof(binscriptRealPath) - 1);
	
	if(binscriptRealPathLength < 1)
	{
		err(errno, "%s:%d", __FILE__, __LINE__);
	}
	
	binscriptRealPath[binscriptRealPathLength] = '\0';

	binscriptFd = open(binscriptRealPath, O_RDONLY);

	if(-1 == binscriptFd)
	{
		err(errno, "%s:%d", __FILE__, __LINE__);
	}

	if(sizeof(ElfHeader) != read(binscriptFd, &elfHeader, sizeof(ElfHeader)))
	{
		err(errno, "%s:%d", __FILE__, __LINE__);
	}

	imageLength = (ssize_t)lseek(binscriptFd, 0L, SEEK_END);

	if(-1 == imageLength)
	{
		err(errno, "%s:%d", __FILE__, __LINE__);
	}

	binscriptLength = (ssize_t)(elfHeader.e_shoff + (elfHeader.e_shnum * elfHeader.e_shentsize));

	packedProgramLength = imageLength - binscriptLength;

	if(binscriptLength == imageLength)
	{
		exit(0);
	}

	binscriptMmio = (uint8_t*)mmap(NULL, (size_t)imageLength, PROT_READ, MAP_SHARED, binscriptFd, 0);

	if(MAP_FAILED == binscriptMmio)
	{
		err(errno, "%s:%d", __FILE__, __LINE__);
	}

	close(binscriptFd);

	packedProgramFd = memfd_create(argv[0], MFD_CLOEXEC);

	if(-1 == packedProgramFd)
	{
		err(errno, "%s:%d", __FILE__, __LINE__);
	}

	if(-1 == ftruncate(packedProgramFd, packedProgramLength))
	{
		err(errno, "%s:%d", __FILE__, __LINE__);
	}

	packedProgramMmio = (uint8_t*)mmap(NULL, (size_t)packedProgramLength, PROT_WRITE | PROT_READ, MAP_SHARED, packedProgramFd, 0);

	if(MAP_FAILED == packedProgramMmio)
	{
		err(errno, "%s:%d", __FILE__, __LINE__);
	}

	pkey[0] = CalculateCrc32(&binscriptMmio[0], binscriptLength, 0xEDB88320U);
	pkey[1] = CalculateCrc32(&binscriptMmio[0], binscriptLength, 0xAE9F7540U);
	pkey[2] = CalculateCrc32(&binscriptMmio[0], binscriptLength, 0xC8605A10U);
	pkey[3] = pkey[0] + pkey[1] + pkey[2] + packedProgramLength;
	R4CDeriveKey(&skey, &pkey[0], 16);
	R4CCrypt(&skey, &binscriptMmio[binscriptLength], (size_t)packedProgramLength, &packedProgramMmio[0]);

	if('#' == packedProgramMmio[0] && '!' == packedProgramMmio[1])
	{
		flags = fcntl(packedProgramFd, F_GETFD);
		if(-1 == flags)
		{
			err(errno, "%s:%d", __FILE__, __LINE__);
		}
		if(-1 == fcntl(packedProgramFd, F_SETFD, flags & ~FD_CLOEXEC))
		{
			err(errno, "%s:%d", __FILE__, __LINE__);
		}
	}

	munmap(binscriptMmio, (size_t)imageLength);

	if(-1 == fexecve(packedProgramFd, argv, envp))
	{
		err(errno, "%s:%d", __FILE__, __LINE__);
	}

	munmap(packedProgramMmio, (size_t)packedProgramLength);
	close(packedProgramFd);

	return 0;
}
