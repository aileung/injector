#pragma once

#include <cstddef>

class Process {
public:
  Process() = default;
  ~Process();

  bool attach(int pid);
  void detach();

  void *allocate(std::size_t size);
  bool free(void *address);

  bool write(void *address, const void *data, std::size_t size);
  bool read(void *address, void *buffer, std::size_t size);

  void *create_thread(void *start_routine, void *arguement);

  bool valid() const;

  int pid() const;

private:
  struct Impl;
  Impl *impl = nullptr;
};
