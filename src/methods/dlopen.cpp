#include "methods/dlopen.h"
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
  void *local_dlopen = (void *)dlopen;

  if (!local_dlopen) {
    return false;
  }

  // start remote thread
  void *thread = proc.execute(local_dlopen, remote_string);

  if (!thread) {
    return false;
  }

  return true;
}
