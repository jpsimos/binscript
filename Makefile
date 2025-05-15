EXTRA_FLAGS := --std=gnu99 -D_GNU_SOURCE -O0 -Wall -fmessage-length=0

all:
	$(CC) -o binscript-packer BinscriptPacker.c Crypto.c Crc32.c $(EXTRA_FLAGS)
	$(CC) -o binscript Binscript.c Crypto.c Crc32.c $(EXTRA_FLAGS)
	$(CC) -o Test Test.c Crypto.c Crc32.c $(EXTRA_FLAGS)
	
clean:
	rm -f binscript
	rm -f binscript-packer
	rm -f Test

