/*
 Derived from source code of TrueCrypt 7.1a, which is
 Copyright (c) 2008-2012 TrueCrypt Developers Association and which is governed
 by the TrueCrypt License 3.0.

 Modifications and additions to the original source code (contained in this file) 
 and all other portions of this file are Copyright (c) 2013-2015 IDRIX
 and are governed by the Apache License 2.0 the full text of which is
 contained in the file License.txt included in VeraCrypt binary and source
 code distribution packages.
*/

#ifndef TC_HEADER_Volume_EncryptionThreadPool
#define TC_HEADER_Volume_EncryptionThreadPool

#include "Platform/Platform.h"
#include "EncryptionMode.h"

namespace VeraCrypt
{
	class EncryptionThreadPool
	{
	public:
		struct WorkType
		{
			enum Enum
			{
				EncryptDataUnits,
				DecryptDataUnits,
				DeriveKey
			};
		};

		struct WorkItem
		{
			struct State
			{
				enum Enum
				{
					Free,
					Ready,
					Busy
				};
			};

			struct WorkItem *FirstFragment;
			auto_ptr <Exception> ItemException;
			SyncEvent ItemCompletedEvent;
			SharedVal <size_t> OutstandingFragmentCount;
			SharedVal <State::Enum> State;
			WorkType::Enum Type;

			union
			{
				struct
				{
					const EncryptionMode *Mode;
					byte *Data;
					uint64 StartUnitNo;
					uint64 UnitCount;
					size_t SectorSize;
				} Encryption;
			};
		};

		static void DoWork (WorkType::Enum type, const EncryptionMode *mode, byte *data, uint64 startUnitNo, uint64 unitCount, size_t sectorSize);
		static bool IsRunning () { return ThreadPoolRunning; }
		static void Start ();
		static void Stop ();

	protected:
		static void WorkThreadProc ();

		static const size_t MaxThreadCount = 32;
		static const size_t QueueSize = MaxThreadCount * 2;

		static Mutex DequeueMutex;
		static volatile size_t DequeuePosition;
		static volatile size_t EnqueuePosition;
		static Mutex EnqueueMutex;
		static list < shared_ptr <Thread> > RunningThreads;
		static volatile bool StopPending;
		static size_t ThreadCount;
		static volatile bool ThreadPoolRunning;
		static SyncEvent WorkItemCompletedEvent;
		static WorkItem WorkItemQueue[QueueSize];
		static SyncEvent WorkItemReadyEvent;
	};
}

#endif // TC_HEADER_Volume_EncryptionThreadPool
