#ifndef PLATFORM_H
#define PLATFORM_H

// address the operating systems
#if defined(_WIN32) || defined(WIN32)
#  undef  WIN32
#  define WIN32
#  undef  _WIN32
#  define _WIN32
#endif

#if defined(_WIN64) || defined(WIN64)
#  undef  WIN64
#  define WIN64
#  undef  _WIN64
#  define _WIN64
#endif

#if defined(WIN64) || defined(WIN32) || defined(_Windows) || defined(__MINGW32__)
#  define OS_MSWIN
#elif defined (__linux) || defined (__linux__) || defined(_MSYS)
#  define OS_LINUX
#else
#error unsupported operating system
#endif

#ifndef UTIL_EXPORTS
#define UTIL_EXPORTS
#endif

/* Some compilers use a special export keyword */
#ifdef OS_MSWIN
#  ifdef UTIL_EXPORTS
#    define UTIL_API __declspec(dllexport)
#  else
#    define UTIL_API __declspec(dllimport)
#  endif
#else
#  if(defined(__GNUC__) && __GNUC__ >= 4)
#    define UTIL_API __attribute__ ((visibility("default")))
#  else
#    define UTIL_API
#  endif
#endif

/* By default C calling convention */
#ifndef UTIL_CALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define UTIL_CALL __cdecl
#else
#define UTIL_CALL
#endif
#endif /* UTIL_CALL */

#endif // PLATFORM_H

