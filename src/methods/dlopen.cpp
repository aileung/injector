#include "methods/dlopen.h"
#include "platform/mac/mac_modules.h"
#include "process.h"

#include <cstring>

#include <dlfcn.h>

bool inject_dlopen(Process &proc, const std::string &dylib_path) {
  if (!proc.valid()) {
    return false;
  }

  size_t size = dylib_path.size() + 1;

  // allocate remote memory
  void *remote_string = proc.allocate(size);
  if (!remote_string) {
    return false;
  }

  // write dylib path
  if (!proc.write(remote_string, dylib_path.c_str(), size)) {
    return false;
  }

  // resolve local dlopen
  uintptr_t local_libdyld = find_local_module("libdyld");
  uintptr_t remote_libdyld = find_remote_module(proc, "libdyld");

  if (!local_libdyld || !remote_libdyld) {
    return false;
  }

  uintptr_t offset = (uintptr_t)dlopen - local_libdyld;

  void *remote_dlopen = (void *)(remote_libdyld + offset);

  unsigned int thread = proc.execute(remote_dlopen, remote_string);

  proc.wait_thread(thread);

  uint64_t result = proc.thread_result(thread);

  if (!result) {
    printf("dlopen failed\n");
    return false;
  }

  printf("dlopen returned handle: %p\n", (void *)result);

  return true;
}
