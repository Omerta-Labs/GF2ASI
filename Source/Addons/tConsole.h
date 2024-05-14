#pragma once
#include "../framework.h"
#include "tLog.h"

class tConsole
{
public:
	static void fCreate(const char* name);
	static void fDestroy();

	static void fFlush();
	static void SetTitle(const char* title);

	static void fAppend();
	static void fPrintf(const char* text, ...);
	static void fWrite(const char* text);
	static void fWriteLine(const char* text);

	static HANDLE GetOutputHandle();
};

