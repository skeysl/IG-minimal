#pragma once

// IGLOG_LEVEL 0 no logs
// IGLOG_LEVEL 1 FATAL
// IGLOG_LEVEL 2 ERROR
// IGLOG_LEVEL 3 WARNING
// IGLOG_LEVEL 4 INFO
// IGLOG_LEVEL 5 DEBUG
// IGLOG_LEVEL 6 TRACE


#ifndef IGLOG_LEVEL
#ifdef _DEBUG
#	define IGLOG_LEVEL 5
#else
#	define IGLOG_LEVEL 4
#endif
#endif


#include <iostream>
#define IGLOG_TRACE_ENTRY(entry) {std::clog << entry << '\n';}
#define IGLOG_DEBUG_ENTRY(entry) {std::clog << entry << '\n';}
#define IGLOG_INFO_ENTRY(entry)	{std::cout << entry << '\n';}
#define IGLOG_WARNING_ENTRY(entry) {std::cout << entry << '\n';}
#define IGLOG_ERROR_ENTRY(entry) {std::cerr << entry << std::endl;}
#define IGLOG_FATAL_ENTRY(entry) {std::cerr << entry << std::endl;}


#if (IGLOG_LEVEL > 5)
#	define IGLOG_TRACE(entry) IGLOG_TRACE_ENTRY(entry)
#else
#  define IGLOG_TRACE(entry) {}
#endif

#if (IGLOG_LEVEL > 4)
#	define IGLOG_DEBUG(entry) IGLOG_DEBUG_ENTRY(entry)
#else
#  define IGLOG_DEBUG(entry) {}
#endif

#if (IGLOG_LEVEL > 3)
#	define IGLOG_INFO(entry) IGLOG_INFO_ENTRY(entry)
#else
#  define IGLOG_INFO(entry) {}
#endif

#if (IGLOG_LEVEL > 2)
#	define IGLOG_WARNING(entry) IGLOG_WARNING_ENTRY(entry)
#else
#  define IGLOG_WARNING(entry) {}
#endif

#if (IGLOG_LEVEL > 1)
#	define IGLOG_ERROR(entry) IGLOG_ERROR_ENTRY(entry)
#else
#  define IGLOG_ERROR(entry) {}
#endif

#if (IGLOG_LEVEL > 0)
#	define IGLOG_FATAL(entry) IGLOG_FATAL_ENTRY(entry)
#else
#  define IGLOG_FATAL(entry) {}
#endif

#define IGLOG(entry) IGLOG_INFO(entry)
