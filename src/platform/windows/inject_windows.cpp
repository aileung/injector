#include "injector.h"
#include "methods/loadlibrary.h"
#include "process.h"

bool Injector::inject(int pid, const std::string &path,
                      InjectionMethod method) {
  Process proc;

  if (!proc.attach(pid)) {
    return false;
  }
  bool result = false;

  // using switch statement since we will add more methods
  switch (method) {
  case InjectionMethod::LoadLibrary:
    result = inject_loadlibrary(proc, path);
    break;
  case InjectionMethod::ManualMap:
    break;
  case InjectionMethod::ThreadHijack:
    break;
  }

  proc.detach();
  return result;
}
