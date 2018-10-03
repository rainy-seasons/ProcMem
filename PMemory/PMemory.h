#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

namespace ProcMem {
	class PMemory
	{
	public:
		bool Process(char* procName);
		DWORD Module(LPSTR ModuleName);
		HANDLE GetHandle();
		void CleanHandle();

		template<class T>
		T ReadPtr(DWORD addr)
		{
			return *((T*)addr);
		}

		template<class T>
		void WritePtr(DWORD addr, T val)
		{
			*((T*)addr) = val;
		}

		template<class T>
		T ReadMem(DWORD addr)
		{
			T buf;
			ReadProcessMemory(m_pHandle, (LPVOID)addr, &buf, sizeof(but), NULL);
			return buf;
		}

		template<class T>
		T WriteMem(DWORD addr, T val)
		{
			WriteProcessMemory(m_pHandle, (LPVOID)addr, &val, sizeof(val), NULL);
		}

		template<class T>
		DWORD ProtectMemory(LPVOID addr, DWORD prot)
		{
			DWORD oldProt;
			VirtualProtectEx(m_pHandle, addr, sizeof(T), prot, &oldProt);
			return oldProt;
		}

		template<int SIZE>
		void WriteNop(DWORD addr)
		{
			auto oldProt = ProtectMemory<BYTE[SIZE]>(addr, PAGE_EXECUTE_READWRITE);
			for (int i = 0; i < SIZE; i++)
			{
				Write<BYTE>(addr + i, 0x90);
			}
		}

	private:
		DWORD m_PID;
		HANDLE m_pHandle;
		PROCESSENTRY32 m_pEntry;
	};
}