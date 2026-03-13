#pragma once
#include <string>

enum class InjectionMethod { LoadLibrary, ManualMap };

class Injector {
public:
  bool inject(int pid, const std::string &dll_path, InjectionMethod method);
  bool uninject(int pid, const std::string &module);
};
