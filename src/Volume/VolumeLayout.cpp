/*
 Derived from source code of TrueCrypt 7.1a, which is
 Copyright (c) 2008-2012 TrueCrypt Developers Association and which is governed
 by the TrueCrypt License 3.0.

 Modifications and additions to the original source code (contained in this file)
 and all other portions of this file are Copyright (c) 2013-2016 IDRIX
 and are governed by the Apache License 2.0 the full text of which is
 contained in the file License.txt included in VeraCrypt binary and source
 code distribution packages.
*/

#include "Volume/EncryptionMode.h"
#include "Volume/EncryptionModeXTS.h"
#include "VolumeLayout.h"
#include "Boot/Windows/BootCommon.h"

namespace VeraCrypt
{
	VolumeLayout::VolumeLayout ()
	{
	}

	VolumeLayout::~VolumeLayout ()
	{
	}

	VolumeLayoutList VolumeLayout::GetAvailableLayouts (VolumeType::Enum type)
	{
		VolumeLayoutList layouts;

		layouts.push_back (shared_ptr <VolumeLayout> (new VolumeLayoutV2Normal ()));
		layouts.push_back (shared_ptr <VolumeLayout> (new VolumeLayoutV1Normal ()));
		layouts.push_back (shared_ptr <VolumeLayout> (new VolumeLayoutV2Hidden ()));
		layouts.push_back (shared_ptr <VolumeLayout> (new VolumeLayoutSystemEncryption ()));

		if (type != VolumeType::Unknown)
		{
			VolumeLayoutList l;

			foreach (shared_ptr <VolumeLayout> vl, layouts)
			{
				if (vl->GetType() == type)
					l.push_back (vl);
			}

			layouts = l;
		}

		return layouts;
	}

	shared_ptr <VolumeHeader> VolumeLayout::GetHeader ()
	{
		if (Header.get() == nullptr)
			Header.reset (new VolumeHeader (GetHeaderSize()));

		return Header;
	}


	VolumeLayoutV1Normal::VolumeLayoutV1Normal ()
	{
		Type = VolumeType::Normal;
		HeaderOffset = TC_VOLUME_HEADER_OFFSET;
		HeaderSize = TC_VOLUME_HEADER_SIZE_LEGACY;

		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new AES ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new Serpent ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new Twofish ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new Camellia ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new AESTwofish ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new AESTwofishSerpent ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new SerpentAES ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new SerpentTwofishAES ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new TwofishSerpent ()));

		SupportedEncryptionModes.push_back (shared_ptr <EncryptionMode> (new EncryptionModeXTS ()));
	}

	uint64 VolumeLayoutV1Normal::GetDataOffset (uint64 volumeHostSize) const
	{
		return HeaderSize;
	}

	uint64 VolumeLayoutV1Normal::GetDataSize (uint64 volumeHostSize) const
	{
		return volumeHostSize - GetHeaderSize();
	}


	VolumeLayoutV2Normal::VolumeLayoutV2Normal ()
	{
		Type = VolumeType::Normal;
		HeaderOffset = TC_VOLUME_HEADER_OFFSET;
		HeaderSize = TC_VOLUME_HEADER_SIZE;
		BackupHeaderOffset = -TC_VOLUME_HEADER_GROUP_SIZE;

		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new AES ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new Serpent ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new Twofish ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new Camellia ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new GOST89 ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new Kuznyechik ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new AESTwofish ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new AESTwofishSerpent ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new SerpentAES ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new SerpentTwofishAES ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new TwofishSerpent ()));

		SupportedEncryptionModes.push_back (shared_ptr <EncryptionMode> (new EncryptionModeXTS ()));
	}

	uint64 VolumeLayoutV2Normal::GetDataOffset (uint64 volumeHostSize) const
	{
		return Header->GetEncryptedAreaStart();
	}

	uint64 VolumeLayoutV2Normal::GetDataSize (uint64 volumeHostSize) const
	{
		return Header->GetVolumeDataSize();
	}

	uint64 VolumeLayoutV2Normal::GetMaxDataSize (uint64 volumeSize) const
	{
		if (volumeSize < TC_TOTAL_VOLUME_HEADERS_SIZE)
			return 0;

		return volumeSize - TC_TOTAL_VOLUME_HEADERS_SIZE;
	}


	VolumeLayoutV2Hidden::VolumeLayoutV2Hidden ()
	{
		Type = VolumeType::Hidden;
		HeaderOffset = TC_HIDDEN_VOLUME_HEADER_OFFSET;
		HeaderSize = TC_VOLUME_HEADER_SIZE;
		BackupHeaderOffset = -TC_HIDDEN_VOLUME_HEADER_OFFSET;

		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new AES ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new Serpent ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new Twofish ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new Camellia ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new GOST89 ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new Kuznyechik ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new AESTwofish ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new AESTwofishSerpent ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new SerpentAES ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new SerpentTwofishAES ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new TwofishSerpent ()));

		SupportedEncryptionModes.push_back (shared_ptr <EncryptionMode> (new EncryptionModeXTS ()));
	}

	uint64 VolumeLayoutV2Hidden::GetDataOffset (uint64 volumeHostSize) const
	{
		return Header->GetEncryptedAreaStart();
	}

	uint64 VolumeLayoutV2Hidden::GetDataSize (uint64 volumeHostSize) const
	{
		return Header->GetVolumeDataSize();
	}

	uint64 VolumeLayoutV2Hidden::GetMaxDataSize (uint64 volumeSize) const
	{
		// Reserve free space at the end of the host filesystem
		uint64 reservedSize;

		if (volumeSize < TC_VOLUME_SMALL_SIZE_THRESHOLD)
			reservedSize = TC_HIDDEN_VOLUME_HOST_FS_RESERVED_END_AREA_SIZE;
		else
			reservedSize = TC_HIDDEN_VOLUME_HOST_FS_RESERVED_END_AREA_SIZE_HIGH; // Ensure size of a hidden volume larger than TC_VOLUME_SMALL_SIZE_THRESHOLD is a multiple of the maximum supported sector size

		if (volumeSize < reservedSize)
			return 0;

		return volumeSize - reservedSize;
	}


	VolumeLayoutSystemEncryption::VolumeLayoutSystemEncryption ()
	{
		Type = VolumeType::Normal;
		HeaderOffset = TC_BOOT_VOLUME_HEADER_SECTOR_OFFSET;
		HeaderSize = TC_BOOT_ENCRYPTION_VOLUME_HEADER_SIZE;

		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new AES ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new Serpent ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new Twofish ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new Camellia ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new GOST89 ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new Kuznyechik ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new AESTwofish ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new AESTwofishSerpent ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new SerpentAES ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new SerpentTwofishAES ()));
		SupportedEncryptionAlgorithms.push_back (shared_ptr <EncryptionAlgorithm> (new TwofishSerpent ()));

		SupportedEncryptionModes.push_back (shared_ptr <EncryptionMode> (new EncryptionModeXTS ()));
	}

	uint64 VolumeLayoutSystemEncryption::GetDataOffset (uint64 volumeHostSize) const
	{
		return 0;
	}

	uint64 VolumeLayoutSystemEncryption::GetDataSize (uint64 volumeHostSize) const
	{
		return volumeHostSize;
	}

	Pkcs5KdfList VolumeLayoutSystemEncryption::GetSupportedKeyDerivationFunctions (bool truecryptMode) const
	{
		Pkcs5KdfList l;
		if (!truecryptMode)
			l.push_back (shared_ptr <Pkcs5Kdf> (new Pkcs5HmacSha256_Boot ()));
		l.push_back (shared_ptr <Pkcs5Kdf> (new Pkcs5HmacRipemd160_1000 (truecryptMode)));
		if (!truecryptMode)
		{
			l.push_back (shared_ptr <Pkcs5Kdf> (new Pkcs5HmacSha512 (false)));
			l.push_back (shared_ptr <Pkcs5Kdf> (new Pkcs5HmacWhirlpool (false)));
			l.push_back (shared_ptr <Pkcs5Kdf> (new Pkcs5HmacStreebog ()));
		}
		return l;
	}
}
