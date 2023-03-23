#include "PMemory.h"
#include <iostream>

// Constructor sets up the PID and Process Handle
PMemory::PMemory(const char* procName)
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	auto snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	while (Process32Next(snapShot, &entry))
	{
		if (!strcmp(procName, entry.szExeFile))
		{
			this->m_PID = entry.th32ProcessID;
			this->m_pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->m_PID);
			break;
		}
	}

	if (snapShot)
		CloseHandle(snapShot);
}

// Destructor closes handle
PMemory::~PMemory()
{
	if (this->m_pHandle)
		CloseHandle(this->m_pHandle);
}

uintptr_t PMemory::GetPID()
{
	return this->m_PID;
}

HANDLE PMemory::GetProcessHandle()
{
	return this->m_pHandle;
}

uintptr_t PMemory::GetModuleAddress(const char* moduleName)
{
	uintptr_t moduleAddr = 0;
	MODULEENTRY32 entry;
	entry.dwSize = sizeof(MODULEENTRY32);

	auto snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, this->m_PID);


	while (Module32Next(snapShot, &entry))
	{
		if (!strcmp(moduleName, entry.szModule))
		{
			moduleAddr = reinterpret_cast<uintptr_t>(entry.modBaseAddr);
			break;
		}
	}

	if (snapShot)
		CloseHandle(snapShot);

	return moduleAddr;
}
