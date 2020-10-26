#pragma once

class Funcs
{
public:
	DWORD GetPID(const char* process);
	bool CloseProcess(DWORD pid);
	bool inject(DWORD pid, const char* path);
};