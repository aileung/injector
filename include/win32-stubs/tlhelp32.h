#pragma once
#include "windows.h"

// flags for CreateToolhelp32Snapshot
#define TH32CS_SNAPPROCESS 0x00000002
#define TH32CS_SNAPTHREAD 0x00000004
#define TH32CS_SNAPMODULE 0x00000008
#define TH32CS_SNAPALL 0x0000000F
#define TH32CS_SNAPMODULE32 0x00000010

typedef struct {
  DWORD dwSize;
  DWORD cntUsage;
  DWORD th32ProcessID;
  ULONG_PTR th32DefaultHeapID;
  DWORD th32ModuleID;
  DWORD cntThreads;
  DWORD th32ParentProcessID;
  LONG pcPriClassBase;
  DWORD dwFlags;
  char szExeFile[260]; // MAX_PATH
} PROCESSENTRY32;

typedef struct {
  DWORD dwSize;
  DWORD th32ModuleID;
  DWORD th32ProcessID;
  DWORD GlblcntUsage;
  DWORD ProccntUsage;
  BYTE *modBaseAddr;
  DWORD modBaseSize;
  HMODULE hModule;
  char szModule[256];
  char szExePath[260]; // MAX_PATH
} MODULEENTRY32;

inline BOOL Module32First(HANDLE, MODULEENTRY32 *) { return 0; }
inline BOOL Module32Next(HANDLE, MODULEENTRY32 *) { return 0; }

inline HANDLE CreateToolhelp32Snapshot(DWORD, DWORD) { return nullptr; }
inline BOOL Process32First(HANDLE, PROCESSENTRY32 *) { return 0; }
inline BOOL Process32Next(HANDLE, PROCESSENTRY32 *) { return 0; }
