/*
 Legal Notice: Some portions of the source code contained in this file were
 derived from the source code of TrueCrypt 7.1a, which is 
 Copyright (c) 2003-2012 TrueCrypt Developers Association and which is 
 governed by the TrueCrypt License 3.0, also from the source code of
 Encryption for the Masses 2.02a, which is Copyright (c) 1998-2000 Paul Le Roux
 and which is governed by the 'License Agreement for Encryption for the Masses' 
 Modifications and additions to the original source code (contained in this file) 
 and all other portions of this file are Copyright (c) 2013-2015 IDRIX
 and are governed by the Apache License 2.0 the full text of which is
 contained in the file License.txt included in VeraCrypt binary and source
 code distribution packages. */

#include "Tcdefs.h"

#include <malloc.h>
#include <ctype.h>
#include "Cmdline.h"

#include "Resource.h"
#include "Crypto.h"
#include "Apidrvr.h"
#include "Dlgcode.h"
#include "Language.h"
#include <Strsafe.h>

#ifndef SRC_POS
#define SRC_POS (__FUNCTION__ ":" TC_TO_STRING(__LINE__))
#endif

/* Except in response to the WM_INITDIALOG message, the dialog box procedure
   should return nonzero if it processes the message, and zero if it does
   not. - see DialogProc */
BOOL CALLBACK CommandHelpDlgProc (HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER (lParam);		/* remove warning */
	UNREFERENCED_PARAMETER (wParam);		/* remove warning */

	switch (msg)
	{
	case WM_INITDIALOG:
		{
		char * tmp = err_malloc(8192);
		char tmp2[MAX_PATH * 2];
		argumentspec *as;
		int i;

		LocalizeDialog (hwndDlg, "IDD_COMMANDHELP_DLG");

		as = (argumentspec*) lParam;

		*tmp = 0;

		StringCbCopyA (tmp, 8192, "VeraCrypt " VERSION_STRING);
#ifdef _WIN64
		StringCbCatA (tmp, 8192, "  (64-bit)");
#else
		StringCbCatA (tmp, 8192, "  (32-bit)");
#endif
#if (defined(_DEBUG) || defined(DEBUG))
		StringCbCatA (tmp, 8192, "  (debug)");
#endif

		StringCbCatA (tmp, 8192, "\n\nCommand line options:\n\n");
		for (i = 0; i < as->arg_cnt; i ++)
		{
			if (!as->args[i].Internal)
			{
				StringCchPrintf(tmp2, MAX_PATH * 2, "%s\t%s\n", as->args[i].short_name, as->args[i].long_name);
				StringCchCat(tmp, 8192, tmp2);
			}
		}

		StringCbCatA (tmp, 8192, "\nExamples:\n\nMount a volume as X:\tveracrypt.exe /q /v volume.hc /l X\nDismount a volume X:\tveracrypt.exe /q /d X");

		SetWindowText (GetDlgItem (hwndDlg, IDC_COMMANDHELP_TEXT), (char*) tmp);
		
		TCfree(tmp);
		return 1;
		}

	case WM_COMMAND:
		EndDialog (hwndDlg, IDOK);
		return 1;
	case WM_CLOSE:
		EndDialog (hwndDlg, 0);
		return 1;
	}

	return 0;
}

int Win32CommandLine (char *lpszCommandLine, char ***lpszArgs)
{
	int argumentCount;
	int i;

	LPWSTR *arguments = CommandLineToArgvW (GetCommandLineW(), &argumentCount);
	if (!arguments)
	{
		handleWin32Error (NULL, SRC_POS);
		return 0;
	}

	--argumentCount;
	if (argumentCount < 1)
	{
		LocalFree (arguments);
		return 0;
	}

	*lpszArgs = malloc (sizeof (char *) * argumentCount);
	if (!*lpszArgs)
		AbortProcess ("OUTOFMEMORY");

	for (i = 0; i < argumentCount; ++i)
	{
		size_t argLen = wcslen (arguments[i + 1]);

		char *arg = malloc (argLen + 1);
		if (!arg)
			AbortProcess ("OUTOFMEMORY");

		if (argLen > 0)
		{
			int len = WideCharToMultiByte (CP_ACP, 0, arguments[i + 1], -1, arg, (int) argLen + 1, NULL, NULL);
			if (len == 0)
			{
				handleWin32Error (NULL, SRC_POS);
				AbortProcessSilent();
			}
		}
		else
			arg[0] = 0;

		(*lpszArgs)[i] = arg;
	}

	LocalFree (arguments);
	return argumentCount;
}

int GetArgSepPosOffset (char *lpszArgument)
{
	if (lpszArgument[0] == '/')
		return 1;

	return 0;
}

int GetArgumentID (argumentspec *as, char *lpszArgument)
{
	int i;

	for (i = 0; i < as->arg_cnt; i++)
	{
		if (_stricmp (as->args[i].long_name, lpszArgument) == 0)
		{
			return as->args[i].Id;
		}
	}

	for (i = 0; i < as->arg_cnt; i++)
	{
		if (as->args[i].short_name[0] == 0)
			continue;

		if (_stricmp (as->args[i].short_name, lpszArgument) == 0)
		{
			return as->args[i].Id;
		}
	}


	return -1;
}

int GetArgumentValue (char **lpszCommandLineArgs, int *nArgIdx,
		  int nNoCommandLineArgs, char *lpszValue, int nValueSize)
{
	*lpszValue = 0;

	if (*nArgIdx + 1 < nNoCommandLineArgs)
	{
		int x = GetArgSepPosOffset (lpszCommandLineArgs[*nArgIdx + 1]);
		if (x == 0)
		{
			/* Handles the case of space between parameter code
			   and value */
			StringCbCopyA (lpszValue, nValueSize, lpszCommandLineArgs[*nArgIdx + 1]);
			lpszValue[nValueSize - 1] = 0;
			(*nArgIdx)++;
			return HAS_ARGUMENT;
		}
	}

	return HAS_NO_ARGUMENT;
}
