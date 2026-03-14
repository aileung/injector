#pragma once

#include <string>

class Process;

bool inject_dlopen(Process &proc, const std::string &dylib_path);
