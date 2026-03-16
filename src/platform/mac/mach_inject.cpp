#include "process.h"

#include <mach/arm/thread_status.h>
#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <mach/thread_act.h>

#include <cstring>
#include <dlfcn.h>

unsigned int Process::execute(void *address, void *arg) {
  if (!valid()) {
    return 0;
  }

  mach_vm_address_t stack = 0;
  const size_t stack_size = 0x4000;

  // allocate stack
  if (mach_vm_allocate(task, &stack, stack_size, VM_FLAGS_ANYWHERE) !=
      KERN_SUCCESS) {
    return 0;
  }

  mach_vm_address_t stack_top = stack + stack_size;

  // thread stack
  arm_thread_state64_t state;
  memset(&state, 0, sizeof(state));

  state.__pc = (uint64_t)address;
  state.__x[0] = (uint64_t)arg;
  state.__x[1] = RTLD_NOW;

  state.__sp = stack_top & ~0xFull;
  state.__fp = stack_top;

  thread_act_t thread;

  kern_return_t result =
      thread_create_running(task, ARM_THREAD_STATE64, (thread_state_t)&state,
                            ARM_THREAD_STATE64_COUNT, &thread);

  if (result != KERN_SUCCESS) {
    return 0;
  }

  return thread;
}
