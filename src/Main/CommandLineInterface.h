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

#ifndef TC_HEADER_Main_CommandInterface
#define TC_HEADER_Main_CommandInterface

#include "System.h"
#include "Main.h"
#include "Volume/VolumeInfo.h"
#include "Core/MountOptions.h"
#include "Core/VolumeCreator.h"
#include "UserPreferences.h"
#include "UserInterfaceType.h"

namespace VeraCrypt
{
	struct CommandId
	{
		enum Enum
		{
			None,
			AutoMountDevices,
			AutoMountDevicesFavorites,
			AutoMountFavorites,
			BackupHeaders,
			ChangePassword,
			CreateKeyfile,
			CreateVolume,
			DeleteSecurityTokenKeyfiles,
			DismountVolumes,
			DisplayVersion,
			DisplayVolumeProperties,
			ExportSecurityTokenKeyfile,
			Help,
			ImportSecurityTokenKeyfiles,
			ListSecurityTokenKeyfiles,
			ListVolumes,
			MountVolume,
			RestoreHeaders,
			SavePreferences,
			Test
		};
	};

	struct CommandLineInterface
	{
	public:
		CommandLineInterface (int argc, wchar_t** argv, UserInterfaceType::Enum interfaceType);
		virtual ~CommandLineInterface ();


		CommandId::Enum ArgCommand;
		bool ArgDisplayPassword;
		shared_ptr <EncryptionAlgorithm> ArgEncryptionAlgorithm;
		shared_ptr <FilePath> ArgFilePath;
		VolumeCreationOptions::FilesystemType::Enum ArgFilesystem;
		bool ArgForce;
		shared_ptr <Hash> ArgHash;
		shared_ptr <KeyfileList> ArgKeyfiles;
		MountOptions ArgMountOptions;
		shared_ptr <DirectoryPath> ArgMountPoint;
		shared_ptr <Hash> ArgNewHash;
		shared_ptr <KeyfileList> ArgNewKeyfiles;
		shared_ptr <VolumePassword> ArgNewPassword;
		int ArgNewPim;
		bool ArgNoHiddenVolumeProtection;
		shared_ptr <VolumePassword> ArgPassword;
		int ArgPim;
		bool ArgQuick;
		FilesystemPath ArgRandomSourcePath;
		uint64 ArgSize;
		shared_ptr <VolumePath> ArgVolumePath;
		VolumeInfoList ArgVolumes;
		VolumeType::Enum ArgVolumeType;
		bool ArgTrueCryptMode;
        shared_ptr<SecureBuffer> ArgTokenPin;

		bool StartBackgroundTask;
		UserPreferences Preferences;

	protected:
		void CheckCommandSingle () const;
		shared_ptr <KeyfileList> ToKeyfileList (const wxString &arg) const;
		VolumeInfoList GetMountedVolumes (const wxString &filter) const;

	private:
		CommandLineInterface (const CommandLineInterface &);
		CommandLineInterface &operator= (const CommandLineInterface &);
	};

	shared_ptr<VolumePassword> ToUTF8Password (const wchar_t* str, size_t charCount = (size_t) -1);
	shared_ptr<SecureBuffer> ToUTF8Buffer (const wchar_t* str, size_t charCount = (size_t) -1);

	extern auto_ptr <CommandLineInterface> CmdLine;
}

#endif // TC_HEADER_Main_CommandInterface
