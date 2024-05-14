#include "tConsole.h"

static HANDLE handle;
static CONSOLE_SCREEN_BUFFER_INFO cInfo;
static bool useLogging;

void tConsole::fCreate(const char* name)
{
	if (handle == NULL) {
		AllocConsole();
		handle = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(handle, &cInfo);
		cInfo.dwSize.Y = 2500;
		SetConsoleScreenBufferSize(handle, cInfo.dwSize);
		SetTitle(name);
		useLogging = true;
	}
}

void tConsole::fDestroy()
{
	if (handle != INVALID_HANDLE_VALUE) {
		FreeConsole();
		handle = INVALID_HANDLE_VALUE;
	}
}

void tConsole::fFlush()
{
}

void tConsole::SetTitle(const char* title)
{
	if (handle != INVALID_HANDLE_VALUE) {
		SetConsoleTitle(title);
	}
}

void tConsole::fAppend()
{
	if (handle != INVALID_HANDLE_VALUE) {
		fWrite("\n");
	}
}

void tConsole::fPrintf(const char* text, ...)
{
	char buffer[4096]{ NULL };

	va_list va;
	va_start(va, text);
	vsprintf_s(buffer, text, va);
	va_end(va);

	fWriteLine(buffer);
}

void tConsole::fWrite(const char* text)
{
	DWORD count = 0;

	if (handle != INVALID_HANDLE_VALUE) {
		WriteConsole(handle, text, strlen(text), &count, NULL);
	}

	if (useLogging) {
		C_Logger::Write(text);
	}
}

void tConsole::fWriteLine(const char* text)
{
	if (handle != INVALID_HANDLE_VALUE) {
		fWrite(text);
		fWrite("\n");
	}
}

HANDLE tConsole::GetOutputHandle()
{
	return handle;
}
