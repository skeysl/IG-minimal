#pragma once

#if defined(_WIN32) && !defined(IGX_STATIC)
#  if (defined(IGX_EXPORTS) || defined(igx_EXPORTS))  
#	  define IGX_DLL  __declspec(dllexport)
#	  define IGX_TEMPLATE_DLL 
#  else 
#	  define IGX_DLL  __declspec(dllimport)
#	  define IGX_TEMPLATE_DLL extern
#  endif
#else
#  define IGX_DLL
#  define IGX_TEMPLATE_DLL
#endif
