#pragma once

#include <cstddef>
#include <cstdint>

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

  unsigned int create_thread(void *start_routine, void *argument);
  bool wait_thread(unsigned int thread);
  uint64_t thread_result(unsigned int thread);

  bool valid() const;
  int pid() const;

  unsigned int execute(void *address, void *arg);

#ifdef __APPLE__
  unsigned int task_port() const;
#endif

private:
  int pid_ = -1;

  struct Impl;
  Impl *impl = nullptr;

#ifdef __APPLE__
  unsigned int task = 0;
#endif
};
