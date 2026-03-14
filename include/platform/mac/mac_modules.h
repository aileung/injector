#pragma once

#include <cstdint>

class Process;

uintptr_t find_local_module(const char *name);
uintptr_t find_remote_module(Process &proc, const char *module_name);

void list_remote_modules(Process *proc);
