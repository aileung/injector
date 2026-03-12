// for lsp support

#pragma once
#ifndef _WIN32

#include <cstdint>

// types
typedef void *HANDLE;
typedef void *HMODULE;
typedef void *HINSTANCE;
typedef void *LPVOID;
typedef const void *LPCVOID;
typedef const char *LPCSTR;
typedef const wchar_t *LPCWSTR;
typedef char *LPSTR;
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef int BOOL;
typedef uint32_t UINT;
typedef size_t SIZE_T;
typedef uintptr_t ULONG_PTR;
typedef intptr_t LONG_PTR;
typedef long LONG;
typedef uint64_t ULONGLONG;
typedef ULONG_PTR DWORD_PTR;
typedef void *FARPROC;

// constants
#define TRUE 1
#define FALSE 0
#define NULL nullptr

#define PROCESS_ALL_ACCESS (0x001F0FFF)
#define PROCESS_VM_READ (0x0010)
#define PROCESS_VM_WRITE (0x0020)
#define PROCESS_VM_OPERATION (0x0008)
#define PROCESS_CREATE_THREAD (0x0002)
#define PROCESS_QUERY_INFORMATION (0x0400)

// memory allocation
#define MEM_COMMIT (0x1000)
#define MEM_RESERVE (0x2000)
#define MEM_RELEASE (0x8000)
#define MEM_FREE (0x10000)

// page protection
#define PAGE_NOACCESS (0x01)
#define PAGE_READONLY (0x02)
#define PAGE_READWRITE (0x04)
#define PAGE_WRITECOPY (0x08)
#define PAGE_EXECUTE (0x10)
#define PAGE_EXECUTE_READ (0x20)
#define PAGE_EXECUTE_READWRITE (0x40)
#define PAGE_EXECUTE_WRITECOPY (0x80)

// GetProcAddress
#define DONT_RESOLVE_DLL_REFERENCES (0x00000001)

// pe structures
#define IMAGE_DOS_SIGNATURE 0x5A4D    // MZ
#define IMAGE_NT_SIGNATURE 0x00004550 // PE\0\0
#define IMAGE_FILE_MACHINE_AMD64 0x8664
#define IMAGE_FILE_MACHINE_I386 0x014c

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES 16
#define IMAGE_DIRECTORY_ENTRY_EXPORT 0
#define IMAGE_DIRECTORY_ENTRY_IMPORT 1
#define IMAGE_DIRECTORY_ENTRY_BASERELOC 5
#define IMAGE_DIRECTORY_ENTRY_TLS 9

#define IMAGE_REL_BASED_ABSOLUTE 0
#define IMAGE_REL_BASED_HIGHLOW 3
#define IMAGE_REL_BASED_DIR64 10

#define IMAGE_ORDINAL_FLAG64 0x8000000000000000ULL
#define IMAGE_ORDINAL_FLAG32 0x80000000

#pragma pack(push, 1)

typedef struct {
  WORD e_magic;
  WORD e_cblp;
  WORD e_cp;
  WORD e_crlc;
  WORD e_cparhdr;
  WORD e_minalloc;
  WORD e_maxalloc;
  WORD e_ss;
  WORD e_sp;
  WORD e_csum;
  WORD e_ip;
  WORD e_cs;
  WORD e_lfarlc;
  WORD e_ovno;
  WORD e_res[4];
  WORD e_oemid;
  WORD e_oeminfo;
  WORD e_res2[10];
  LONG e_lfanew;
} IMAGE_DOS_HEADER;

typedef struct {
  WORD Machine;
  WORD NumberOfSections;
  DWORD TimeDateStamp;
  DWORD PointerToSymbolTable;
  DWORD NumberOfSymbols;
  WORD SizeOfOptionalHeader;
  WORD Characteristics;
} IMAGE_FILE_HEADER;

typedef struct {
  DWORD VirtualAddress;
  DWORD Size;
} IMAGE_DATA_DIRECTORY;

typedef struct {
  WORD Magic;
  BYTE MajorLinkerVersion;
  BYTE MinorLinkerVersion;
  DWORD SizeOfCode;
  DWORD SizeOfInitializedData;
  DWORD SizeOfUninitializedData;
  DWORD AddressOfEntryPoint;
  DWORD BaseOfCode;
  ULONGLONG ImageBase;
  DWORD SectionAlignment;
  DWORD FileAlignment;
  WORD MajorOperatingSystemVersion;
  WORD MinorOperatingSystemVersion;
  WORD MajorImageVersion;
  WORD MinorImageVersion;
  WORD MajorSubsystemVersion;
  WORD MinorSubsystemVersion;
  DWORD Win32VersionValue;
  DWORD SizeOfImage;
  DWORD SizeOfHeaders;
  DWORD CheckSum;
  WORD Subsystem;
  WORD DllCharacteristics;
  ULONGLONG SizeOfStackReserve;
  ULONGLONG SizeOfStackCommit;
  ULONGLONG SizeOfHeapReserve;
  ULONGLONG SizeOfHeapCommit;
  DWORD LoaderFlags;
  DWORD NumberOfRvaAndSizes;
  IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER64;

typedef struct {
  DWORD Signature;
  IMAGE_FILE_HEADER FileHeader;
  IMAGE_OPTIONAL_HEADER64 OptionalHeader;
} IMAGE_NT_HEADERS64;

typedef struct {
  BYTE Name[8];
  union {
    DWORD PhysicalAddress;
    DWORD VirtualSize;
  } Misc;
  DWORD VirtualAddress;
  DWORD SizeOfRawData;
  DWORD PointerToRawData;
  DWORD PointerToRelocations;
  DWORD PointerToLinenumbers;
  WORD NumberOfRelocations;
  WORD NumberOfLinenumbers;
  DWORD Characteristics;
} IMAGE_SECTION_HEADER;

// import table
typedef struct {
  union {
    DWORD Characteristics;
    DWORD OriginalFirstThunk;
  };
  DWORD TimeDateStamp;
  DWORD ForwarderChain;
  DWORD Name;
  DWORD FirstThunk;
} IMAGE_IMPORT_DESCRIPTOR;

typedef struct {
  union {
    ULONGLONG Ordinal;
    ULONGLONG AddressOfData;
  } u1;
} IMAGE_THUNK_DATA64;

typedef struct {
  WORD Hint;
  BYTE Name[1];
} IMAGE_IMPORT_BY_NAME;

// Export table
typedef struct {
  DWORD Characteristics;
  DWORD TimeDateStamp;
  WORD MajorVersion;
  WORD MinorVersion;
  DWORD Name;
  DWORD Base;
  DWORD NumberOfFunctions;
  DWORD NumberOfNames;
  DWORD AddressOfFunctions;
  DWORD AddressOfNames;
  DWORD AddressOfNameOrdinals;
} IMAGE_EXPORT_DIRECTORY;

// relocations
typedef struct {
  DWORD VirtualAddress;
  DWORD SizeOfBlock;
} IMAGE_BASE_RELOCATION;

// tls
typedef struct {
  ULONGLONG StartAddressOfRawData;
  ULONGLONG EndAddressOfRawData;
  ULONGLONG AddressOfIndex;
  ULONGLONG AddressOfCallBacks;
  DWORD SizeOfZeroFill;
  DWORD Characteristics;
} IMAGE_TLS_DIRECTORY64;

#pragma pack(pop)

// convenience aliases
typedef IMAGE_NT_HEADERS64 IMAGE_NT_HEADERS;
typedef IMAGE_OPTIONAL_HEADER64 IMAGE_OPTIONAL_HEADER;
typedef IMAGE_THUNK_DATA64 IMAGE_THUNK_DATA;
typedef IMAGE_TLS_DIRECTORY64 IMAGE_TLS_DIRECTORY;

// dll entry point signature
typedef BOOL (*DLL_ENTRY_POINT)(HINSTANCE, DWORD, LPVOID);
#define DLL_PROCESS_ATTACH 1
#define DLL_PROCESS_DETACH 0

// functions
inline HANDLE OpenProcess(DWORD, BOOL, DWORD) { return nullptr; }
inline BOOL CloseHandle(HANDLE) { return 0; }
inline LPVOID VirtualAllocEx(HANDLE, LPVOID, SIZE_T, DWORD, DWORD) {
  return nullptr;
}
inline BOOL VirtualFreeEx(HANDLE, LPVOID, SIZE_T, DWORD) { return 0; }
inline BOOL VirtualProtectEx(HANDLE, LPVOID, SIZE_T, DWORD, DWORD *) {
  return 0;
}
inline BOOL WriteProcessMemory(HANDLE, LPVOID, LPCVOID, SIZE_T, SIZE_T *) {
  return 0;
}
inline BOOL ReadProcessMemory(HANDLE, LPCVOID, LPVOID, SIZE_T, SIZE_T *) {
  return 0;
}
inline HANDLE CreateRemoteThread(HANDLE, void *, SIZE_T, void *, LPVOID, DWORD,
                                 DWORD *) {
  return nullptr;
}
inline DWORD WaitForSingleObject(HANDLE, DWORD) { return 0; }
inline HMODULE GetModuleHandleA(LPCSTR) { return nullptr; }
inline FARPROC GetProcAddress(HMODULE, LPCSTR) { return nullptr; }
inline HMODULE LoadLibraryA(LPCSTR) { return nullptr; }
inline BOOL FreeLibrary(HMODULE) { return 0; }
inline DWORD GetLastError() { return 0; }

#endif // _WIN32
