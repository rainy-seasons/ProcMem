#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>

class PMemory
{
private:
	std::uintptr_t m_PID = 0;
	HANDLE m_pHandle = nullptr;

public:
	PMemory(const char* procName);
	~PMemory();

	std::uintptr_t GetPID();
	HANDLE GetProcessHandle();

	std::uintptr_t GetModuleAddress(const char* ModuleName);

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
	T ReadMem(std::uintptr_t addr)
	{
		T val;
		ReadProcessMemory(m_pHandle, (LPCVOID)addr, &val, sizeof(T), NULL);
		return val;
	}

	// Returns 0 if WPM fails. Returns non-zero if success.
	template<typename T>
	bool WriteMem(std::uintptr_t addr, T val)
	{
		return WriteProcessMemory(m_pHandle, (LPVOID)addr, &val, sizeof(T), NULL);
	}

	template<typename T>
	std::uintptr_t ProtectMemory(LPVOID addr, std::uintptr_t prot)
	{
		std::uintptr_t oldProt;
		VirtualProtectEx(m_pHandle, addr, sizeof(T), prot, &oldProt);
		return oldProt;
	}
};
