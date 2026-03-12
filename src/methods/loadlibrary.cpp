#include "methods/loadlibrary.h"
#include "process.h"
#include <cstring>

bool inject_loadlibrary(Process &proc, const std::string &dll_path) {
  if (!proc.valid()) {
    return false;
  }

  std::size_t size = dll_path.size() + 1;

  // allocate memory in target process
  void *remote_string = proc.allocate(size);
  if (!remote_string) {
    return false;
  }

  // write dll path into target memory
  if (!proc.write(remote_string, dll_path.c_str(), size)) {
    return false;
  }

  // resolve LoadLibraryA address
  void *load_library_addr = /* resolve symbol*/ nullptr;

  if (!load_library_addr) {
    return false;
  }

  // create remote thread calling LoadLibraryA
  void *thread = proc.create_thread(load_library_addr, remote_string);

  if (!thread) {
    return false;
  }

  return true;
}
