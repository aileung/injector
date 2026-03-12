#pragma once

#include <cstddef>
#include <cstdint>
#include <system_error>

class Process {
public:
  bool attach(int pid);
  void detach();

  void *allocate(std::size_t size);
  bool free(void *address);

  bool write(void *address, const void *data, std::size_t size);
  bool read(void *address, void *buffer, std::size_t size);

  void *create_thread(void *start_routine, void *arguement);

  bool valid() const;

private:
  int pid = -1;
};
