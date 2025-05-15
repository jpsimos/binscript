∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞
∞∞∞∞∞∞|                          Binscript
∞∞∞∞∞∞|                   Author: Jacob A Psimos
∞∞∞∞∞∞|
∞∞∞∞∞∞|  Binscript is a two part tool can be described as an executable packer.
∞∞∞∞∞∞|  Originally designed to convert a shell script to an executable binary, it can pack any
∞∞∞∞∞∞|  executable except iterations of itsself into a wrapper executable that makes it difficult
∞∞∞∞∞∞|  to read plain text content of the program being converted. The resulting packed binscript
∞∞∞∞∞∞|  decrypts itsself in RAM and executes the original program.
∞∞∞∞∞∞|
∞∞∞∞∞∞|  Refer to the two sections for details and the last two for wisdom.
∞∞∞∞∞∞|
∞∞∞∞∞∞|  1. binscript 
∞∞∞∞∞∞|  2. binscript-packer is used once by the one wishing to convert
∞∞∞∞∞∞|
∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞∞

∞∞∞∞∞∞| binscript

Contains unpacking code that decrypts the original program and executes it from a memfd device.
It needs to be built once and the resulting executable is passed on the command line to binscript-packer
where it will be converted to a packed executable containing the user's program as specified in the arguments aswell.
Binscript can be ran without ever packing code into it. In that case, it exits with zero.

∞∞∞∞∞∞| binscript-packer
Does the insertion and encryption of the user's program as specified in the arguments along with the path to the binscript executable.
After the packed binscript is created, it can be considered a replacement for the original user's program.
Packing already packed executables is not supported.

∞∞∞∞∞∞| disclaimer
Binscript is not intended to be used to conceal or otherwise subvert malware from detection. On its own, binscript
is harmless and leaves no mess behind on the system it is executed on. I am not responsible for misuse of this tool.

∞∞∞∞∞∞| 1-800-medivac
Packed executables from binscript can easily be detected without executing them. Verify the length of the file
in question matches that in the ELF header. 

