#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>

class PMemory
{
private:
	uintptr_t m_PID = 0;
	HANDLE m_pHandle = nullptr;

public:
	PMemory(const char* procName);
	~PMemory();

	uintptr_t GetPID();
	HANDLE GetProcessHandle();

	uintptr_t GetModuleAddress(const char* ModuleName);

	// Reads a pointer - only works via dll injection.
	template<class T>
	T ReadPtr(LPVOID addr)
	{
		return *((T*)addr);
	}

	template<class T>
	void WritePtr(LPVOID addr, T val)
	{
		*((T*)addr) = val;
	}

	// Returns the value found at the target address.
	template<typename T>
	T ReadMem(uintptr_t addr)
	{
		T val;
		ReadProcessMemory(m_pHandle, (LPCVOID)addr, &val, sizeof(T), NULL);
		return val;
	}

	// Returns 0 if WPM fails. Returns non-zero if success.
	template<typename T>
	bool WriteMem(uintptr_t addr, T val)
	{
		return WriteProcessMemory(m_pHandle, (LPVOID)addr, &val, sizeof(T), NULL);
	}

	template<typename T>
	uintptr_t ProtectMemory(LPVOID addr, uintptr_t prot)
	{
		uintptr_t oldProt;
		VirtualProtectEx(m_pHandle, addr, sizeof(T), prot, &oldProt);
		return oldProt;
	}
};
