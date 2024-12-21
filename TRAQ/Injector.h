#define WIN32_MEAN_AND_LEAN
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <tchar.h>
#include "Native-API/ntpebteb.h"
#include "Native-API/NtQueryInformationProcess.h"
#include "Native-API/ntldr.h"

typedef NTSTATUS(NTAPI* NtWriteVirtualMemory)(
    _In_ HANDLE ProcessHandle,
    _In_opt_ PVOID BaseAddress,
    _In_reads_bytes_(BufferSize) PVOID Buffer,
    _In_ SIZE_T BufferSize,
    _Out_opt_ PSIZE_T NumberOfBytesWritten
);

class Injector
{
private:
    HANDLE TargetProcess;
    HANDLE TargetThread;

public:
    VOID Attach(int pid)
    {
        this->TargetProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        if (this->TargetProcess == NULL) {
            printf("[) Failed to attach \n");
        }
    }

    // PEB UTILS
    HANDLE ExGetCurrentHandle()
    {
        return this->TargetProcess;
    }
    PVOID ExGetCurrentPebAddress()
    {
        NtQueryInformationProcess Query = (NtQueryInformationProcess)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");
        if (Query)
        {
            PROCESS_BASIC_INFORMATION pbi;
            ULONG length = 0;

            NTSTATUS status = Query(this->TargetProcess, ProcessBasicInformation, &pbi, sizeof(pbi), &length);

            if (status == 0)
                return pbi.PebBaseAddress;

            return NULL;
        }
        else
            return NULL;

    }
    PEB* ExGetCurrentPeb()
    {
        PEB Peb;
        if (ReadProcessMemory(this->TargetProcess, ExGetCurrentPebAddress(), &Peb, sizeof(PEB), NULL))
        {
            return &Peb;
        }
        return NULL;
    }

    VOID  ExDbgPrintPeb()
    {
        PVOID PEB = ExGetCurrentPeb();
        if (PEB)
        {
            printf("[) Current PEB Address -> %p\n", PEB);
        }
    }

    template<typename T> VOID WriteMemory(uintptr_t Address, PVOID Buffer)
    {
        NtWriteVirtualMemory Write = (NtWriteVirtualMemory)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtWriteVirtualMemory");
        NTSTATUS ntRet = Write(this->TargetProcess, Address, Buffer, sizeof(T), NULL);
        if (ntRet != 0)
            printf("[) Failed writing memory \n");
    }
};
