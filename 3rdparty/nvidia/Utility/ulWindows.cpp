/******************************************************************************
ulWindows.cpp

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:
     Windows specific routines



******************************************************************************/
#include <ul.h>

//#include <windows.h>





//======================================================================
// Function:	GetFileName
// Description:	get a file name from a standard open dialog box
// Parameters:	hWnd = window that owns the dialog
//				fileMask = "3D Studio Files", "*.3ds", NULL, NULL
// Returns:		char * = full path/filename or NULL
//======================================================================
char * ulGetFileName(HWND hWnd, char *fileMask)
{
	OPENFILENAME theFileInfo;
	static char fileName[256];
	//char filterStrs[256];

	theFileInfo.lStructSize = sizeof(OPENFILENAME);
	theFileInfo.hwndOwner = hWnd;
	theFileInfo.hInstance = NULL;
	theFileInfo.lpstrFilter = fileMask;
	theFileInfo.lpstrCustomFilter = NULL;
	theFileInfo.lpstrFile = fileName;
	theFileInfo.nMaxFile = 255;
	theFileInfo.lpstrFileTitle = NULL;
	theFileInfo.lpstrInitialDir = NULL;
	theFileInfo.lpstrTitle = NULL;
	theFileInfo.Flags = OFN_LONGNAMES;
	theFileInfo.lpstrDefExt = NULL;

	if (GetOpenFileName(&theFileInfo))
		return fileName;
	else
		return NULL;
}







