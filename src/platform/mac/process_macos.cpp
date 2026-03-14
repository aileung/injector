#include "process.h"

#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <cstring>

bool Process::attach(int pid){
    pid_ = pid;

    kern_return_t result = task_for_pid(
        mach_task_self(),
        pid,
        reinterpret_cast<mach_port_t*>(&task)
    );

    return result == KERN_SUCCESS;
}

void Process::detach(){
    task = MACH_PORT_NULL;
    pid_ = -1;
}

bool Process::valid() const {
    return task != MACH_PORT_NULL;
}

int Process::pid() const {
    return pid_;
}

void* Process::allocate(std::size_t size) {
    mach_vm_address_t address = 0;

    kern_return_t result = mach_vm_allocate(
        task,
        &address,
        size,
        VM_FLAGS_ANYWHERE
    );

    if (result != KERN_SUCCESS)
        return nullptr;

    return reinterpret_cast<void*>(address);
}

bool Process::free(void* address) {
    return mach_vm_deallocate(
        task,
        reinterpret_cast<mach_vm_address_t>(address),
        0
    ) == KERN_SUCCESS;
}

bool Process::write(void* address, const void* data, std::size_t size) {
    return mach_vm_write(
        task,
        reinterpret_cast<mach_vm_address_t>(address),
        reinterpret_cast<vm_offset_t>(const_cast<void*>(data)),
        static_cast<mach_msg_type_number_t>(size)
    ) == KERN_SUCCESS;
}

bool Process::read(void* address, void* buffer, std::size_t size) {
    mach_vm_size_t outsize = 0;

    return mach_vm_read_overwrite(
        task,
        reinterpret_cast<mach_vm_address_t>(address),
        size,
        reinterpret_cast<mach_vm_address_t>(buffer),
        &outsize
    ) == KERN_SUCCESS;
}

Process::execute()


