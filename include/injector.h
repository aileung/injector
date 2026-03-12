#pragma once
#include <string>

enum class InjectionMethod { LoadLibrary, ManualMap, ThreadHijack };

class Injector {
public:
  bool inject(int pid, const std::string &dll, InjectionMethod method);
  bool uninject(int pid, const std::string &module);
};
