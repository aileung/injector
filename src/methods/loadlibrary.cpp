#include "methods/loadlibrary.h"
#include "process.h"
#include <cstdint>
#include <cstring>
#include <strings.h>
#define _stricmp strcasecmp

#ifdef _WIN32
#include <tlhelp32.h>
#include <windows.h>
#else
#include "win32-stubs/tlhelp32.h"
#include "win32-stubs/windows.h"
#endif

// find remote kernel32
uintptr_t find_remote_module(int pid, const char *name) {
  HANDLE snapshot =
      CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
  if (snapshot == INVALID_HANDLE_VALUE) {
    return 0;
  }

  MODULEENTRY32 module;
  module.dwSize = sizeof(MODULEENTRY32);

  if (!Module32First(snapshot, &module)) {
    CloseHandle(snapshot);
    return 0;
  }

  do {
    if (_stricmp(module.szModule, name) == 0) {
      uintptr_t base = reinterpret_cast<uintptr_t>(module.modBaseAddr);

      CloseHandle(snapshot);
      return base;
    }
  } while (Module32Next(snapshot, &module));

  CloseHandle(snapshot);
  return 0;
};

bool inject_loadlibrary(Process &proc, const std::string &dll_path) {
  if (!proc.valid()) {
    return false;
  }

  std::size_t size = dll_path.size() + 1;

  // allocate memory in target process
  void *remote_buffer = proc.allocate(size);
  if (!remote_buffer) {
    return false;
  }

  // write dll path in target memory
  if (!proc.write(remote_buffer, dll_path.c_str(), size)) {
    return false;
  }

  // get local kernel32 base
  HMODULE local_kernel32 = GetModuleHandleA("kernel32");

  // get local LoadLibraryA
  void *local_loadlibrary = GetProcAddress(local_kernel32, "LoadLibraryA");

  // compute offset
  uintptr_t offset = (uintptr_t)local_loadlibrary - (uintptr_t)local_kernel32;

  // remote kernel32
  uintptr_t remote_kernel32 = find_remote_module(proc.pid(), "kernel32.dll");
  if (!remote_kernel32) {
    return false;
  }

  void *remote_loadlibrary = (void *)(remote_kernel32 + offset);

  // compute remote thread
  void *thread = proc.create_thread(remote_loadlibrary, remote_buffer);

  if (!thread) {
    return false;
  }

  // wait for dll to load
  WaitForSingleObject((HANDLE)thread, INFINITE);
  CloseHandle((HANDLE)thread);

  // free memory
  proc.free(remote_buffer);

  return true;
}
