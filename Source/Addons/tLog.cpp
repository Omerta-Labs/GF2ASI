#include "tLog.h"

#include <cstdarg>
#include <iostream>
#include <fstream>

std::ofstream mFile;

void C_Logger::Create(const std::string& Name)
{
	if (!IsOpen())
	{
		mFile.open(Name.data(), std::ios::out | std::ios::trunc);
	}
}

void C_Logger::Destroy()
{
	if (IsOpen())
	{
		mFile.close();
	}
}

void C_Logger::Append()
{
	if (IsOpen())
	{
		mFile << std::endl;
	}
}

void C_Logger::Printf(const char* text, ...)
{
	if (IsOpen())
	{
		char buffer[4096]{ NULL };

		va_list va;
		va_start(va, text);
		vsprintf_s(buffer, text, va);
		va_end(va);

		WriteLine(buffer);
	}
}

void C_Logger::Write(const char* text)
{
	if (IsOpen())
	{
		mFile << text;
	}
}

void C_Logger::WriteLine(const char* text)
{
	if (IsOpen())
	{
		mFile << text << std::endl;
	}
}

bool C_Logger::IsOpen()
{
	return mFile.is_open();
}

void C_Logger::Flush()
{
	if (IsOpen())
	{
		std::flush(mFile);
	}
}
