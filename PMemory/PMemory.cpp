#include "PMemory.h"
#include <iostream>

// Constructor sets up the PID and Process Handle
PMemory::PMemory(const char* procName)
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapShot == INVALID_HANDLE_VALUE)
	{
		std::cerr << "CreateToolHelp32Snapshot failed. Error: " << GetLastError() << std::endl;
	}

	while (Process32Next(snapShot, &entry))
	{
		if (!strcmp(procName, entry.szExeFile))
		{
			this->m_PID = entry.th32ProcessID;
			this->m_pHandle = OpenProcess(PROCESS_ALL_ACCESS, false, this->m_PID);
			if (!this->m_pHandle)
			{
				std::cerr << "OpenProcess failed. Error: " << GetLastError() << std::endl;
			}
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

// Writes a NOP byte to a specific address 
bool PMemory::WriteNOP(HANDLE hProcess, LPVOID lpAddress)
{
	BYTE NOP = 0x90;
	DWORD oldProtect;

	// Change the memory protection to allow writing
	if (!VirtualProtectEx(hProcess, lpAddress, sizeof(NOP), PAGE_EXECUTE_READWRITE, &oldProtect))
	{
		std::cerr << "VirtualProtectEx failed: " << GetLastError() << std::endl;
		return false;
	}

	// Write the NOP instruction
	uintptr_t written;
	if (!WriteProcessMemory(hProcess, lpAddress, &NOP, sizeof(NOP), &written) || written != sizeof(NOP))
	{
		std::cerr << "WriteProcessMemory failed: " << GetLastError() << std::endl;
		// Restore the original memory protection
		VirtualProtectEx(hProcess, lpAddress, sizeof(NOP), oldProtect, &oldProtect);
		return false;
	}

	// Restore the original memory protection
	if (!VirtualProtectEx(hProcess, lpAddress, sizeof(NOP), oldProtect, &oldProtect))
	{
		return false;
	}

	return true;
}
