# injector

A cross-platform cli injection tool for **Windows**, **Linux**, and **macOS** for debugging and low level os internals

# features
- dll, gdb and mach injection
- process listing and PID resolution by name
- payload path validation before injection
- verbose logging mode for debugging injection

---

## build section
TODO: 
- prerequisites section for each OS
- building each release


## usage

```
injector [OPTIONS] <pid|process-name> <payload-path>

Options:
  -p, --pid       <PID>        Target process by PID
  -n, --name      <name>       Target process by name
  -m, --method    <method>     Injection method: dll | gdb | mach
  -v, --verbose                Enable verbose output
  -l, --list                   List running processes
  -h, --help                   Show this help message
```

## general process (maybe put into blog)

### windows - dll injection

1. *open* the target process with `OpenProcess` (requires `PROCESS_ALL_ACCESS`)
2. *allocate* memory in the remote process with `VirtualAllocEx`
3. *write* the DLL path into that memory with `WriteProcessMemory`
4. *spawn* a remote thread with `CreateRemoteThread`, pointing at `LoadLibraryA`
5. *The target process loads your DLL and calls its `DllMain`

### linux — gdb injection

1. *attach* gdb to the target pid
2. *find* `dlopen` in the target's memory map (`/proc/<pid>/maps`)
3. *call* `dlopen("mylib.so", RTLD_NOW)` via gdb's `call` command
4. *detach* gdb

### macOS — mach injection

1. *aquire* a Mach task port via `task_for_pid` (requires entitlements or root)
2. *allocate* remote memory with `mach_vm_allocate`
3. *write* the dylib path with `mach_vm_write`
4. *create* a remote thread with `thread_create_running` pointing at `dlopen`
