#
# Derived from source code of TrueCrypt 7.1a, which is
# Copyright (c) 2008-2012 TrueCrypt Developers Association and which is governed
# by the TrueCrypt License 3.0.
#
# Modifications and additions to the original source code (contained in this file) 
# and all other portions of this file are Copyright (c) 2013-2015 IDRIX
# and are governed by the Apache License 2.0 the full text of which is
# contained in the file License.txt included in VeraCrypt binary and source
# code distribution packages.
#

OBJS :=
OBJSEX :=
OBJS += Cipher.o
OBJS += EncryptionAlgorithm.o
OBJS += EncryptionMode.o
OBJS += EncryptionModeXTS.o
OBJS += EncryptionTest.o
OBJS += EncryptionThreadPool.o
OBJS += Hash.o
OBJS += Keyfile.o
OBJS += Pkcs5Kdf.o
OBJS += Volume.o
OBJS += VolumeException.o
OBJS += VolumeHeader.o
OBJS += VolumeInfo.o
OBJS += VolumeLayout.o
OBJS += VolumePassword.o
OBJS += VolumePasswordCache.o

ifeq "$(PLATFORM)" "MacOSX"
    OBJSEX += ../Crypto/Aes_asm.oo
    OBJS += ../Crypto/Aes_hw_cpu.o
    OBJS += ../Crypto/Aescrypt.o
else ifeq "$(CPU_ARCH)" "x86"
	OBJS += ../Crypto/Aes_x86.o
	OBJS += ../Crypto/Aes_hw_cpu.o
else ifeq "$(CPU_ARCH)" "x64"
	OBJS += ../Crypto/Aes_x64.o
	OBJS += ../Crypto/Aes_hw_cpu.o
else
	OBJS += ../Crypto/Aescrypt.o
endif

OBJS += ../Crypto/Aeskey.o
OBJS += ../Crypto/Aestab.o
OBJS += ../Crypto/Rmd160.o
OBJS += ../Crypto/Serpent.o
OBJS += ../Crypto/Sha2.o
OBJS += ../Crypto/Twofish.o
OBJS += ../Crypto/Whirlpool.o

OBJS += ../Common/Crc.o
OBJS += ../Common/Endian.o
OBJS += ../Common/GfMul.o
OBJS += ../Common/Pkcs5.o
OBJS += ../Common/SecurityToken.o

VolumeLibrary: Volume.a

ifeq "$(PLATFORM)" "MacOSX"
../Crypto/Aes_asm.oo: ../Crypto/Aes_x86.asm ../Crypto/Aes_x64.asm
	@echo Assembling $(<F)
	$(AS) $(ASFLAGS) -f macho32 -o ../Crypto/Aes_x86.o ../Crypto/Aes_x86.asm
	$(AS) $(ASFLAGS) -f macho64 -o ../Crypto/Aes_x64.o ../Crypto/Aes_x64.asm
	lipo -create ../Crypto/Aes_x86.o ../Crypto/Aes_x64.o -output ../Crypto/Aes_asm.oo
	rm -fr ../Crypto/Aes_x86.o ../Crypto/Aes_x64.o
endif

include $(BUILD_INC)/Makefile.inc
