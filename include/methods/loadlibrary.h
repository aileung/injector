#pragma once
#include <string>

class Process;

bool inject_loadlibrary(Process &proc, const std::string &dll_path);
