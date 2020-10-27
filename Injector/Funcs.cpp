#include "includes.h"

using f_LoadLibraryA		= HINSTANCE(__stdcall*)(const char* lpLibFilename);
using f_GetProcAddress		= UINT_PTR(__stdcall*)(HINSTANCE hModule, const char* lpProcName);
using f_DLL_ENTRY_POINT		= BOOL(__stdcall*)(void* hDll, DWORD dwReason, void* pReserved);

struct MANUAL_MAPPING_DATA
{
	f_LoadLibraryA	 pLoadLibraryA;
	f_GetProcAddress pGetProcessAddress;
	HINSTANCE		 hMod;
};

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

bool LoadLibrary_(DWORD pid, const char* path)
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

void __stdcall Shellcode(MANUAL_MAPPING_DATA* pData);

bool ManualMap(DWORD pid, const char* path)
{
	const auto handle = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
	if (handle != INVALID_HANDLE_VALUE)
	{
		BYTE*					pSrcData	   = nullptr;
		IMAGE_NT_HEADERS*		pOldNtHeader   = nullptr;
		IMAGE_OPTIONAL_HEADER*	pOldOptHeader  = nullptr;
		IMAGE_FILE_HEADER*		pOldFileHeader = nullptr;
		BYTE*					pTargetBase	   = nullptr;

		DWORD dwCheck = 0;
		if (!GetFileAttributesA(path))
			return false;

		ifstream File(path, ios::binary | ios::ate);

		if (File.fail())
			return false;

		auto FileSize = File.tellg();
		if (FileSize < 0x1000)
		{
			File.close();
			return false;
		}	
		pSrcData = new BYTE[(UINT_PTR)(FileSize)];
		if (!pSrcData)
		{
			File.close();
			return false;
		}
		File.seekg(0, ios::beg);
		File.read((char*)(pSrcData), FileSize);
		File.close();

		if (reinterpret_cast<IMAGE_DOS_HEADER*>(pSrcData)->e_magic != 0x5A4D)
		{
			delete[] pSrcData;
			return false;
		}

		pOldNtHeader = reinterpret_cast<IMAGE_NT_HEADERS*>(pSrcData + reinterpret_cast<IMAGE_DOS_HEADER*>(pSrcData)->e_lfanew);
		pOldOptHeader = &pOldNtHeader->OptionalHeader;
		pOldFileHeader = &pOldNtHeader->FileHeader;
		pTargetBase = reinterpret_cast<BYTE*>(VirtualAllocEx(handle, reinterpret_cast<void*>(pOldOptHeader->ImageBase), 
			pOldOptHeader->SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
		if (!pTargetBase)
		{
			pTargetBase = reinterpret_cast<BYTE*>(VirtualAllocEx(handle, nullptr,
				pOldOptHeader->SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
			if (!pTargetBase)
			{
				delete[] pSrcData;
				return false;
			}
		}
		MANUAL_MAPPING_DATA data{ 0 };
		data.pLoadLibraryA = LoadLibraryA;
		data.pGetProcessAddress = reinterpret_cast<f_GetProcAddress>(GetProcAddress);
		auto* pSectionHeader = IMAGE_FIRST_SECTION(pOldNtHeader);
		for (UINT i = 0; i != pOldFileHeader->NumberOfSections; ++i, ++pSectionHeader)
		{
			if (pSectionHeader->SizeOfRawData)
			{
				if (!WriteProcessMemory(handle, pTargetBase + pSectionHeader->VirtualAddress, pSrcData + pSectionHeader->PointerToRawData,
					pSectionHeader->SizeOfRawData, nullptr))
				{
					delete[] pSrcData;
					VirtualFreeEx(handle, pTargetBase, 0, MEM_RELEASE);
					return false;
				}		
			}
		}

		memcpy(pSrcData, &data, sizeof(data));
		WriteProcessMemory(handle, pTargetBase, pSrcData, 0x1000, nullptr);

		delete[] pSrcData;

		void* pShellCode = VirtualAllocEx(handle, nullptr, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (!pShellCode)
		{
			VirtualFreeEx(handle, pTargetBase, 0, MEM_RELEASE);
			return false;
		}

		WriteProcessMemory(handle, pShellCode, Shellcode, 0x1000, nullptr);

		HANDLE hThread = CreateRemoteThread(handle, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(pShellCode), pTargetBase, 0, nullptr);
		if (!hThread)
		{
			VirtualFreeEx(handle, pTargetBase, 0, MEM_RELEASE);
			VirtualFreeEx(handle, pTargetBase, 0, MEM_RELEASE);
			return false;
		}
		CloseHandle(hThread);

		HINSTANCE hCheck = NULL;
		while (!hCheck)
		{
			MANUAL_MAPPING_DATA data_checked{ 0 };
			ReadProcessMemory(handle, pTargetBase, &data_checked, sizeof(data_checked), nullptr);
			hCheck = data_checked.hMod;
			Sleep(10);
		}
		VirtualFreeEx(handle, pShellCode, 0, MEM_RELEASE);
	}
	else
	{
		return false;
	}
	CloseHandle(handle);
	return true;
}

#define _RELOC_FLAG32(RelInfo)((RelInfo >> 0x0C) == IMAGE_REL_BASED_HIGHLOW)
#define _RELOC_FLAG64(RelInfo)((RelInfo >> 0x0C) == IMAGE_REL_BASED_DIR64)
#ifdef _WIN64
#define RELOC_FLAG _RELOC_FLAG64
#else
#define RELOC_FLAG _RELOC_FLAG32
#endif

UINT __forceinline _strlenA(const char* szString)
{
	UINT ret = 0;
	for (; *szString; ++szString, ++ret);
	return ret;
}

void __stdcall Shellcode(MANUAL_MAPPING_DATA* pData)
{
	if (!pData)
		return;
	BYTE* pBase = reinterpret_cast<BYTE*>(pData);
	auto* pOpt = &reinterpret_cast<IMAGE_NT_HEADERS*>(pBase + reinterpret_cast<IMAGE_DOS_HEADER*>(pData)->e_lfanew)->OptionalHeader;
	auto _LoadLibraryA = pData->pLoadLibraryA;
	auto _GetProcAddress = pData->pGetProcessAddress;
	auto _DllMain = reinterpret_cast<f_DLL_ENTRY_POINT>(pBase + pOpt->AddressOfEntryPoint);
	BYTE* LocationDelta = pBase - pOpt->ImageBase;
	if (LocationDelta)
	{
		if (!pOpt->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size)
			return;
		auto* pRelocData = reinterpret_cast<IMAGE_BASE_RELOCATION*>(pBase + pOpt->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
		while (pRelocData->VirtualAddress)
		{
			UINT AmountOfEntries = (pRelocData->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
			WORD* pRelativeInfo = reinterpret_cast<WORD*>(pRelocData + 1);
			for (UINT i = 0; i != AmountOfEntries; ++i, ++pRelativeInfo)
			{
				if (RELOC_FLAG(*pRelativeInfo))
				{
					UINT_PTR* pPatch = reinterpret_cast<UINT_PTR*>(pBase + pRelocData->VirtualAddress + ((*pRelativeInfo) & 0xFFF));
					*pPatch += reinterpret_cast<UINT_PTR>(LocationDelta);
				}
			}
			pRelocData = reinterpret_cast<IMAGE_BASE_RELOCATION*>(reinterpret_cast<BYTE*>(pRelocData) + pRelocData->SizeOfBlock);
		}
	}
	if (pOpt->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size)
	{
		auto* pImportDescr = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(pBase + pOpt->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
		while (pImportDescr->Name)
		{
			char* szMod = reinterpret_cast<char*>(pBase + pImportDescr->Name);
			HINSTANCE hDll = _LoadLibraryA(szMod);

			ULONG_PTR* pThunkRef = reinterpret_cast<ULONG_PTR*>(pBase + pImportDescr->OriginalFirstThunk);
			ULONG_PTR* pFuncRef = reinterpret_cast<ULONG_PTR*>(pBase + pImportDescr->FirstThunk);

			if (!pThunkRef)
				pThunkRef = pFuncRef;
			for (; *pThunkRef; ++pThunkRef, ++pFuncRef)
			{
				if (IMAGE_SNAP_BY_ORDINAL(*pThunkRef))
				{
					*pFuncRef = _GetProcAddress(hDll, reinterpret_cast<char*>(*pThunkRef & 0xFFFF));
				}
				else
				{
					auto* pImport = reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(pBase + (*pThunkRef));
					*pFuncRef = _GetProcAddress(hDll, pImport->Name);
				}
			}
			++pImportDescr;
		}
	}
	if (pOpt->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size)
	{
		auto* pTLS = reinterpret_cast<IMAGE_TLS_DIRECTORY*>(pBase + pOpt->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);
		auto* pCallback = reinterpret_cast<PIMAGE_TLS_CALLBACK*>(pTLS->AddressOfCallBacks);
		for (; pCallback && *pCallback; ++pCallback)
			(*pCallback)(pBase, DLL_PROCESS_ATTACH, nullptr);
	}
	_DllMain(pBase, DLL_PROCESS_ATTACH, nullptr);
	pData->hMod = reinterpret_cast<HINSTANCE>(pBase);
}

bool Funcs::inject(DWORD pid, const char* path, int delay = 0, int method = 0)
{
	Sleep(delay * 1000);

	switch (method)
	{
	case 0:
	    return LoadLibrary_(pid, path);
	case 1:
	    return ManualMap(pid, path);
	default: break;
	}

	return false;
}
