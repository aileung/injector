cmake_minimum_required(VERSION 3.16)
project(injector LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-03")

# core source files

set(CORE_SOURCES
  src/main.cpp
  src/core/injector.cpp
  src/core/process.cpp
  src/core/memory.cpp
)

set(METHOD_SOURCES
  src/methods/manual_map.cpp
  src/methods/loadlibrary.cpp
  src/methods/thread_hijack.cpp
)

set(STEALTH_SOURCES
  src/stealth/header_erase.cpp
  src/stealth/dll_scramble.cpp
)

# platform specific sources

if (WIN32)
  set(PLATFORM_SOURCES
    src/platform/windows/process_windows.cpp
    src/platform/windows/memory_windows.cpp
  )
elseif(APPLE)
  set(PLATFORM_SOURCES
    src/platform/mac/process_macos.cpp
  )
elseif(UNIX)
  set(PLATFORM_SOURCES
    src/platform/linux/process_linux.cpp
  )
endif()

# executable

add_executable(injector
  ${CORE_SOURCES}
  ${METHOD_SOURCES}
  ${STEALTH_SOURCES}
  ${PLATFORM_SOURCES}
)

target_include_directories(injector
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# platform libraries

if (WIN32)
  target_link_libraries(injector
    PRIVATE
    kernel32
    ntdll
  )
endif()
