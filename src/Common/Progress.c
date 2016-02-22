/*
 Legal Notice: Some portions of the source code contained in this file were
 derived from the source code of TrueCrypt 7.1a, which is 
 Copyright (c) 2003-2012 TrueCrypt Developers Association and which is 
 governed by the TrueCrypt License 3.0, also from the source code of
 Encryption for the Masses 2.02a, which is Copyright (c) 1998-2000 Paul Le Roux
 and which is governed by the 'License Agreement for Encryption for the Masses' 
 Modifications and additions to the original source code (contained in this file) 
 and all other portions of this file are Copyright (c) 2013-2016 IDRIX
 and are governed by the Apache License 2.0 the full text of which is
 contained in the file License.txt included in VeraCrypt binary and source
 code distribution packages. */

#include "Tcdefs.h"
#include "Language.h"
#include "Dlgcode.h"
#include "Progress.h"
#include "../Format/Tcformat.h"
#include "../Format/FormatCom.h"
#include "../Format/resource.h"

#include <Strsafe.h>

static ULONG prevTime, startTime;
static __int64 TotalSize;
static __int64 resumedPointBytesDone;
static BOOL bProgressBarReverse = FALSE;
static BOOL bRWThroughput = FALSE;
static BOOL bShowStatus = FALSE;
static BOOL bPercentMode = FALSE;

static wchar_t *seconds, *minutes, *hours, *days;


// If bIOThroughput is TRUE, the speed reflects the amount of data read AND written per second (rather than
// the speed of the "transform cursor").
void InitProgressBar (__int64 totalBytes, __int64 bytesDone, BOOL bReverse, BOOL bIOThroughput, BOOL bDisplayStatus, BOOL bShowPercent)
{
	HWND hProgressBar = GetDlgItem (hCurPage, nPbar);
	SendMessage (hProgressBar, PBM_SETRANGE32, 0, 10000);
	SendMessage (hProgressBar, PBM_SETSTEP, 1, 0);

	bProgressBarReverse = bReverse;
	bRWThroughput = bIOThroughput;
	bShowStatus = bDisplayStatus;
	bPercentMode = bShowPercent;

	seconds = GetString ("SECONDS");
	minutes = GetString ("MINUTES");
	hours = GetString ("HOURS");
	days = GetString ("DAYS");

	prevTime = startTime = GetTickCount ();
	TotalSize = totalBytes;
	resumedPointBytesDone = bytesDone;
}


BOOL UpdateProgressBar (__int64 byteOffset)
{
	return UpdateProgressBarProc (byteOffset);
}


BOOL UpdateProgressBarProc (__int64 byteOffset)
{
	wchar_t text[100];
	wchar_t speed[100];
	HWND hProgressBar = GetDlgItem (hCurPage, nPbar);
	int time = GetTickCount ();
	int elapsed = (time - startTime) / 1000;

	uint64 bytesDone = (bProgressBarReverse ? (TotalSize - byteOffset) : byteOffset);
	uint64 bytesPerSec = (bProgressBarReverse ? (resumedPointBytesDone - byteOffset) : (bytesDone - resumedPointBytesDone)) / (elapsed + 1);

	if (bPercentMode)
	{
		double perc = (double) (100.0 * (bProgressBarReverse ? ((double) (TotalSize - byteOffset)) : ((double) byteOffset)) / (TotalSize == 0 ? 0.0001 : ((double) TotalSize)));

		if (perc > 99.999999999)
			StringCchCopyW (text,ARRAYSIZE(text),  GetString ("PROCESSED_PORTION_100_PERCENT"));
		else
			StringCchPrintfW (text, ARRAYSIZE (text), GetString ("PROCESSED_PORTION_X_PERCENT"), perc);

		StringCchCatW (text, ARRAYSIZE(text), L" ");
	}
	else
	{
		GetSizeString (bytesDone, text, sizeof(text));
		if (bytesDone < (unsigned __int64) BYTES_PER_MB * 1000000)
			StringCchPrintfW(text, ARRAYSIZE(text), L"%I64d %s ", bytesDone / BYTES_PER_MB, GetString ("MB"));
		else if (bytesDone < (unsigned __int64) BYTES_PER_GB * 1000000)
			StringCchPrintfW(text, ARRAYSIZE(text), L"%I64d %s ", bytesDone / BYTES_PER_GB, GetString ("GB"));
		else if (bytesDone < (unsigned __int64) BYTES_PER_TB * 1000000)
			StringCchPrintfW(text, ARRAYSIZE(text), L"%I64d %s ", bytesDone / BYTES_PER_TB, GetString ("TB"));
		else
			StringCchPrintfW(text, ARRAYSIZE(text), L"%I64d %s ", bytesDone / BYTES_PER_PB, GetString ("PB"));
	}

	SetWindowTextW (GetDlgItem (hCurPage, IDC_BYTESWRITTEN), text);

	if (!bShowStatus)
	{
		GetSpeedString (bRWThroughput ? bytesPerSec*2 : bytesPerSec, speed, sizeof(speed));
		StringCchCatW (speed, ARRAYSIZE(speed), L" ");
		SetWindowTextW (GetDlgItem (hCurPage, IDC_WRITESPEED), speed);
	}

	if (byteOffset < TotalSize)
	{
		int64 sec = (int64) ((bProgressBarReverse ? byteOffset : (TotalSize - byteOffset)) / (bytesPerSec == 0 ? 0.001 : bytesPerSec));

		if (bytesPerSec == 0 || sec > 60 * 60 * 24 * 999)
			StringCchPrintfW (text, ARRAYSIZE(text), L"%s ", GetString ("NOT_APPLICABLE_OR_NOT_AVAILABLE"));
		else if (sec >= 60 * 60 * 24 * 2)
			StringCchPrintfW (text, ARRAYSIZE(text), L"%I64d %s ", sec / (60 * 24 * 60), days);
		else if (sec >= 120 * 60)
			StringCchPrintfW (text, ARRAYSIZE(text), L"%I64d %s ", sec / (60 * 60), hours);
		else if (sec >= 120)
			StringCchPrintfW (text, ARRAYSIZE(text), L"%I64d %s ", sec / 60, minutes);
		else
			StringCchPrintfW (text, ARRAYSIZE(text), L"%I64d %s ", sec, seconds);

		SetWindowTextW (GetDlgItem (hCurPage, IDC_TIMEREMAIN), text);
	}

	prevTime = time;

	SendMessage (hProgressBar, PBM_SETPOS, 
		(int) (10000.0 * (bProgressBarReverse ? (TotalSize - byteOffset) : byteOffset) / (TotalSize == 0 ? 1 : TotalSize)),
		0);

	return bVolTransformThreadCancel;
}
