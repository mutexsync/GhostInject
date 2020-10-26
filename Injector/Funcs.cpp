#include "includes.h"

DWORD Funcs::GetPID(const char* process)
{
	DWORD pid = NULL;
	const auto handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (handle != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(handle, &entry))
		{
			do
			{
				if (!_strcmpi(entry.szExeFile, process))
				{
					pid = entry.th32ProcessID;
					break;
				}
			} while (Process32Next(handle, &entry));
		}
	}
	CloseHandle(handle);
	return pid;
}

bool Funcs::CloseProcess(DWORD pid)
{
	const auto handle = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
	if (handle != INVALID_HANDLE_VALUE)
	{
		if (TerminateProcess(handle, 0))
		{
			CloseHandle(handle);
			return true;
		}
	}
	return false;
}

bool Funcs::inject(DWORD pid, const char* path)
{
	const auto handle = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
	if (handle && handle != INVALID_HANDLE_VALUE)
	{
		void* alloc = VirtualAllocEx(handle, 0, strlen(path), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		if (alloc)
			WriteProcessMemory(handle, alloc, path, strlen(path), 0);
		CreateRemoteThread(handle, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, (LPVOID)alloc, 0, 0);
	}
	else
	{
		return false;
	}
	CloseHandle(handle);
	return true;
}