#include "platform/mac/mac_modules.h"
#include "process.h"

#include <mach/mach.h>
#include <mach/mach_vm.h>
#include <mach/task_info.h>

#include <mach-o/dyld.h>
#include <mach-o/dyld_images.h>

#include <cstring>
#include <iostream>
#include <vector>

uintptr_t find_local_module(const char *name) {
  uint32_t image_count = _dyld_image_count();

  for (uint32_t i = 0; i < image_count; i++) {
    const char *path = _dyld_get_image_name(i);

    if (path && strstr(path, name)) {
      return (uintptr_t)_dyld_get_image_header(i);
    }
  }

  return 0;
}

uintptr_t find_remote_module(Process &proc, const char *module_name) {
  task_t task = proc.task_port();

  task_dyld_info_data_t dyld_info{};
  mach_msg_type_number_t count = TASK_DYLD_INFO_COUNT;

  if (task_info(task, TASK_DYLD_INFO, (task_info_t)&dyld_info, &count) !=
      KERN_SUCCESS) {
    return 0;
  }

  dyld_all_image_infos infos{};
  mach_vm_size_t size = sizeof(infos);

  if (mach_vm_read_overwrite(task, dyld_info.all_image_info_addr, sizeof(infos),
                             (mach_vm_address_t)&infos,
                             &size) != KERN_SUCCESS) {
    return 0;
  }

  std::vector<dyld_image_info> images(infos.infoArrayCount);

  size = images.size() * sizeof(dyld_image_info);

  if (mach_vm_read_overwrite(task, (mach_vm_address_t)infos.infoArray, size,
                             (mach_vm_address_t)images.data(),
                             &size) != KERN_SUCCESS) {
    return 0;
  }

  for (uint32_t i = 0; i < infos.infoArrayCount; i++) {
    char path[1024] = {0};

    mach_vm_read_overwrite(task, (mach_vm_address_t)images[i].imageFilePath,
                           sizeof(path), (mach_vm_address_t)path, &size);

    if (strstr(path, module_name)) {
      return (uintptr_t)images[i].imageLoadAddress;
    }
  }

  return 0;
}

void list_remote_modules(Process &proc) {
  task_t task = proc.task_port();

  task_dyld_info_data_t dyld_info{};
  mach_msg_type_number_t count = TASK_DYLD_INFO_COUNT;

  if (task_info(task, TASK_DYLD_INFO, (task_info_t)&dyld_info, &count) !=
      KERN_SUCCESS) {
    return;
  }

  dyld_all_image_infos infos{};
  mach_vm_size_t size = sizeof(infos);

  if (mach_vm_read_overwrite(task, dyld_info.all_image_info_addr, sizeof(infos),
                             (mach_vm_address_t)&infos,
                             &size) != KERN_SUCCESS) {
    return;
  }

  std::vector<dyld_image_info> images(infos.infoArrayCount);

  size = images.size() * sizeof(dyld_image_info);

  if (mach_vm_read_overwrite(task, (mach_vm_address_t)infos.infoArray, size,
                             (mach_vm_address_t)images.data(),
                             &size) != KERN_SUCCESS) {
    return;
  }

  for (uint32_t i = 0; i < infos.infoArrayCount; i++) {
    char path[1024] = {0};

    mach_vm_read_overwrite(task, (mach_vm_address_t)images[i].imageFilePath,
                           sizeof(path), (mach_vm_address_t)path, &size);

    std::cout << path << std::endl;
  }
}
