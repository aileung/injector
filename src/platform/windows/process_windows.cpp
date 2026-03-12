#include "process.h"
#include <stdatomic.h>

#ifdef _WIN32
#include <windows.h>
#else
#include "win32-stubs/windows.h"
#endif

struct Process::Impl {
  HANDLE handle = nullptr;
  int pid = -1;
};

Process::~Process() { detach(); }

bool Process::attach(int target_pid) {
  impl = new Impl();

  impl->handle =
      OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
                      PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
                  FALSE, target_pid);
  if (!impl->handle) {
    delete impl;
    impl = nullptr;
    return false;
  }

  impl->pid = target_pid;
  return true;
}

void Process::detach() {
  if (!impl) {
    return;
  }
  if (impl->handle) {
    CloseHandle(impl->handle);
  }
  delete impl;
  impl = nullptr;
}

void *Process::allocate(std::size_t size) {
  if (!impl || !impl->handle) {
    return nullptr;
  }
  return VirtualAllocEx(impl->handle, nullptr, size, MEM_COMMIT | MEM_RESERVE,
                        PAGE_READWRITE);
}

bool Process::free(void *address) {
  if (!impl || !impl->handle) {
    return false;
  }
  return VirtualFreeEx(impl->handle, address, 0, MEM_RELEASE);
}

bool Process::write(void *address, const void *data, std::size_t size) {
  if (!impl || !impl->handle) {
    return false;
  }
  SIZE_T written = 0;

  BOOL result = WriteProcessMemory(impl->handle, address, data, size, &written);
  return result && written == size;
}

bool Process::read(void *address, void *buffer, std::size_t size) {
  if (!impl || !impl->handle) {
    return false;
  }
  SIZE_T bytes_read = 0;

  BOOL result =
      ReadProcessMemory(impl->handle, address, buffer, size, &bytes_read);
  return result && bytes_read == size;
}

void *Process::create_thread(void *start_routine, void *argument) {
  if (!impl || !impl->handle) {
    return nullptr;
  }

  HANDLE thread = CreateRemoteThread(
      imple->handle, nullptr, 0,
      reinterpret_cast<LPTHREAD_START_ROUTINE>(start_routine), argument, 0,
      nullptr);

  return thread;
}

bool Process::valid() const { return impl && impl->handle; }
