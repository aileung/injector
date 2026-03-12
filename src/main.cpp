#include <cstdlib>
#include <iostream>
#include <string>

#include "injector.h"

void print_usage() {
  std::cout << "injector <pid> <dll_path> [method]\n";
  std::cout << "Methods: loadlibrary, manualmap, threadhijack";
}

InjectionMethod parse_method(const std::string &method) {
  if (method == "loadlibrary") {
    return InjectionMethod::LoadLibrary;
  }
  if (method == "manualmap") {
    return InjectionMethod::ManualMap;
  }
  if (method == "threadhijack") {
    return InjectionMethod::ThreadHijack;
  }
  throw std::runtime_error("unknown injection method");
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    print_usage();
    return 1;
  }

  int pid = std::stoi(argv[1]);
  std::string dll_path = argv[2];

  InjectionMethod method = InjectionMethod::LoadLibrary;

  if (argc >= 4) {
    try {
      method = parse_method(argv[3]);
    } catch (const std::exception &e) {
      std::cerr << e.what() << "\n";
      return 1;
    }
  }

  Injector injector;

  bool result = injector.inject(pid, dll_path, method);

  if (!result) {
    std::cerr << "inject failed\n";
    return 1;
  }
  std::cout << "injected\n";
  return 0;
}
