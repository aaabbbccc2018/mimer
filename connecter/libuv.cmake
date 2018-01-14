cmake_minimum_required(VERSION 3.0)
project(libuv)

# This CMakeLists.txt
# - builds and installs `libuv` for all the platforms `uv.gyp` supports
# - installs `libuv-config.cmake` config module which provides an import
#   library target `libuv`
# - optionally builds run-tests and run-benchmarks (enable with the
#   UV_ENABLE_TESTS option)
#
#
# This file was created from uv.gyp, common.gypi and
# Makefile.am @f198b82f597570c3ba36b91cb5113695de80d346
#
#
# Differences from the gyp files:
#
# - From uv.gyp the cflags, xcode_settings, and msvs-setting sections has
#   been left out
# - From common.gypi only the definitions have been used
# - Add: debug postfix for libuv (libuvd)
# - Add: create and install libuv-config.cmake (creates import library)
# - Add: Optional building of tests and benchmarks (default: OFF)
# - Fix (win): Definitions controlling dll function signatures
#   (USING_UV_SHARED)
# - List of installed headers converted from Makefile.am

option(UV_ENABLE_TESTS "Build tests and benchmark" OFF)

set(CMAKE_DEBUG_POSTFIX d)

if(CMAKE_SYSTEM_NAME MATCHES "[dD][rR][aA][gG][oO][nN][fF][lL][yY][bB][sS][dD]")
    set(DRAGONFLYBSD 1)
endif()

set(LIBUV_SOURCES
    include/uv.h
    include/tree.h
    include/uv-errno.h
    include/uv-threadpool.h
    include/uv-version.h
    src/fs-poll.c
    src/heap-inl.h
    src/inet.c
    src/queue.h
    src/threadpool.c
    src/uv-common.c
    src/uv-common.h
    src/version.c
)

set(include_HEADERS include/uv.h include/uv-errno.h include/uv-threadpool.h include/uv-version.h)

if(WIN32)
    list(APPEND LIBUV_SOURCES
        include/uv-win.h
        src/win/async.c
        src/win/atomicops-inl.h
        src/win/core.c
        src/win/dl.c
        src/win/error.c
        src/win/fs.c
        src/win/fs-event.c
        src/win/getaddrinfo.c
        src/win/getnameinfo.c
        src/win/handle.c
        src/win/handle-inl.h
        src/win/internal.h
        src/win/loop-watcher.c
        src/win/pipe.c
        src/win/thread.c
        src/win/poll.c
        src/win/process.c
        src/win/process-stdio.c
        src/win/req.c
        src/win/req-inl.h
        src/win/signal.c
        src/win/stream.c
        src/win/stream-inl.h
        src/win/tcp.c
        src/win/tty.c
        src/win/timer.c
        src/win/udp.c
        src/win/util.c
        src/win/winapi.c
        src/win/winapi.h
        src/win/winsock.c
        src/win/winsock.h
    )
    list(APPEND include_HEADERS include/uv-win.h include/tree.h)
else()
    list(APPEND LIBUV_SOURCES
        include/uv-unix.h
        include/uv-linux.h
        include/uv-sunos.h
        include/uv-darwin.h
        include/uv-bsd.h
        include/uv-aix.h
        src/unix/async.c
        src/unix/atomic-ops.h
        src/unix/core.c
        src/unix/dl.c
        src/unix/fs.c
        src/unix/getaddrinfo.c
        src/unix/getnameinfo.c
        src/unix/internal.h
        src/unix/loop.c
        src/unix/loop-watcher.c
        src/unix/pipe.c
        src/unix/poll.c
        src/unix/process.c
        src/unix/signal.c
        src/unix/spinlock.h
        src/unix/stream.c
        src/unix/tcp.c
        src/unix/thread.c
        src/unix/timer.c
        src/unix/tty.c
        src/unix/udp.c
    )
    list(APPEND include_HEADERS include/uv-unix.h)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR APPLE OR ANDROID) # OS in "linux mac ios android"
    list(APPEND LIBUV_SOURCES src/unix/proctitle.c)
endif()

if(APPLE) # OS in "mac ios"
    list(APPEND LIBUV_SOURCES
        src/unix/darwin.c
        src/unix/fsevents.c
        src/unix/darwin-proctitle.c
    )
    list(APPEND include_HEADERS include/uv-darwin.h)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND LIBUV_SOURCES
        src/unix/linux-core.c
        src/unix/linux-inotify.c
        src/unix/linux-syscalls.c
        src/unix/linux-syscalls.h
    )
    list(APPEND include_HEADERS include/uv-linux.h)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
    list(APPEND LIBUV_SOURCES src/unix/sunos.c)
    list(APPEND include_HEADERS include/uv-sunos.h)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "AIX")
    list(APPEND LIBUV_SOURCES src/unix/aix.c)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" OR DRAGONFLYBSD)
    list(APPEND LIBUV_SOURCES src/unix/freebsd.c)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
    list(APPEND LIBUV_SOURCES src/unix/openbsd.c)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "NetBSD")
    list(APPEND LIBUV_SOURCES src/unix/netbsd.c)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "FreeBSD|OpenBSD|NetBSD" OR DRAGONFLYBSD)
    list(APPEND include_HEADERS include/uv-bsd.h)
endif()

if(APPLE OR CMAKE_SYSTEM_NAME MATCHES "FreeBSD|OpenBSD|NetBSD" OR DRAGONFLYBSD) # OS in "ios mac freebsd dragonflybsd openbsd netbsd".split()
    list(APPEND LIBUV_SOURCES src/unix/kqueue.c)
endif()

if(ANDROID)
    list(APPEND LIBUV_SOURCES
        src/unix/linux-core.c
        src/unix/linux-inotify.c
        src/unix/linux-syscalls.c
        src/unix/linux-syscalls.h
        src/unix/pthread-fixes.c
        src/unix/android-ifaddrs.c
    )
    list(APPEND include_HEADERS include/android-ifaddrs.h include/pthread-fixes.h)
endif()

add_library(libuv ${LIBUV_SOURCES})

target_include_directories(libuv
    PRIVATE src
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    )

# find pthreads on all platforms, on Windows it's a no-op
set(CMAKE_THREAD_PREFER_PTHREAD 1)
set(THREADS_PREFER_PTHREAD_FLAG 1)
include(FindThreads)

# Repeat last section of FindThreads.cmake from CMake > 3.1
# This is to support CMake versions < 3.1 where
# FindThreads did not create import lib
if(THREADS_FOUND AND NOT TARGET Threads::Threads)
  add_library(Threads::Threads INTERFACE IMPORTED)

  if(THREADS_HAVE_PTHREAD_ARG)
    set_property(TARGET Threads::Threads PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
  endif()

  if(CMAKE_THREAD_LIBS_INIT)
    set_property(TARGET Threads::Threads PROPERTY INTERFACE_LINK_LIBRARIES "${CMAKE_THREAD_LIBS_INIT}")
  endif()
endif()

target_link_libraries(libuv PUBLIC Threads::Threads)

target_compile_definitions(libuv
    PRIVATE $<$<CONFIG:Debug>:DEBUG> $<$<CONFIG:Debug>:_DEBUG>
    $<$<NOT:$<CONFIG:Debug>>:NDEBUG>)


file(STRINGS include/uv-version.h version_lines REGEX "#define UV_VERSION_")
foreach(i UV_VERSION_MAJOR UV_VERSION_MINOR UV_VERSION_PATCH)
    string(REGEX MATCH "${i}[^0-9]+([0-9]+)" _ "${version_lines}")
    set(${i} "${CMAKE_MATCH_1}")
endforeach()

set_target_properties(libuv PROPERTIES
    SOVERSION 1 # defined on all platforms, ignored when not used
    VERSION ${UV_VERSION_MAJOR}.${UV_VERSION_MINOR}.${UV_VERSION_PATCH}
    )

if(UNIX)
    set_target_properties(libuv PROPERTIES OUTPUT_NAME uv)
endif()

if(BUILD_SHARED_LIBS)
    target_compile_definitions(libuv
        PRIVATE BUILDING_UV_SHARED=1
        INTERFACE USING_UV_SHARED=1)
endif()

if(WIN32)
    target_compile_definitions(libuv
        PUBLIC _WIN32_WINNT=0x0600
        PRIVATE WIN32 _CRT_SECURE_NO_DEPRECATE _CRT_NONSTDC_NO_DEPRECATE)
    # advapi32 and shell32 seem to be default, add them if still missing
    target_link_libraries(libuv PRIVATE ws2_32 iphlpapi psapi userenv)
else()
    # this should be PUBLIC since the interface (uv-unix.h) refers to off_t
    target_compile_definitions(libuv
        PUBLIC _LARGEFILE_SOURCE _FILE_OFFSET_BITS=64
        PRIVATE EV_VERIFY=2) # Note: EV_VERIFY is not mentioned anywhere
    target_link_libraries(libuv PRIVATE m)
endif()

if(APPLE) # OS in "mac ios"
    target_compile_definitions(libuv PRIVATE _DARWIN_USE_64_BIT_INODE=1 _DARWIN_UNLIMITED_SELECT=1)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
# -D_POSIX_C_SOURCE=200112 is given in uv.gyp but is not mentioned in Makefile.am
# and also made the build fail, so omit it
    target_compile_definitions(libuv
        PRIVATE _GNU_SOURCE)
    target_link_libraries(libuv PRIVATE dl rt)
endif()

if(ANDROID)
    target_link_libraries(libuv PRIVATE dl)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "FreeBSD|OpenBSD|NetBSD" OR DRAGONFLYBSD) # OS in "freebsd dragonflybsd openbsd netbsd".split()
    target_link_libraries(libuv PRIVATE kvm)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
    target_compile_definitions(libuv PUBLIC __EXTENSIONS__ _XOPEN_SOURCE=500)
    target_link_libraries(libuv PRIVATE kstat nsl sendfile socket)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "AIX")
    target_compile_definitions(libuv PUBLIC _ALL_SOURCE _XOPEN_SOURCE=500 _LINUX_SOURCE_COMPAT)
    target_link_libraries(libuv PRIVATE perfstat)
endif()

if(UV_ENABLE_TESTS)
    add_subdirectory(test)
endif()

set(config_module_dest share/cmake/libuv)
install(TARGETS libuv EXPORT libuv-targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)
install(EXPORT libuv-targets FILE DESTINATION "${config_module_dest}")
install(FILES ${include_HEADERS} DESTINATION include)

set(config_module [[
    # Creates import library target 'libuv'
    # Usage:
    #     find_package(libuv ...)
    #     target_link_libraries(... libuv ...)




    # Repeat last section of FindThreads.cmake from CMake > 3.1
    # This is to support CMake versions < 3.1 where
    # FindThreads did not create import lib
    if(NOT THREADS_FOUND)
        set(CMAKE_THREAD_PREFER_PTHREAD 1)
        set(THREADS_PREFER_PTHREAD_FLAG 1)
        include(FindThreads)
    endif()
    if(THREADS_FOUND AND NOT TARGET Threads::Threads)
        add_library(Threads::Threads INTERFACE IMPORTED)
        if(THREADS_HAVE_PTHREAD_ARG)
            set_property(TARGET Threads::Threads PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
        endif()
        if(CMAKE_THREAD_LIBS_INIT)
            set_property(TARGET Threads::Threads PROPERTY INTERFACE_LINK_LIBRARIES "${CMAKE_THREAD_LIBS_INIT}")
        endif()
    endif()
    include(${CMAKE_CURRENT_LIST_DIR}/libuv-targets.cmake)
    ]])
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/hide/libuv-config.cmake "${config_module}")

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/hide/libuv-config.cmake DESTINATION "${config_module_dest}")
