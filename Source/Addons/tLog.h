#pragma once

#include <string>

// Not part of Illusion engine
class C_Logger
{
public:

	static void Create(const std::string& Name);
	static void Destroy();

	static void Append();
	static void Printf(const char* text, ...);
	static void Write(const char* text);
	static void WriteLine(const char* text);

private:

	static bool IsOpen();
	static void Flush();
};