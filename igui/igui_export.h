#pragma once

#if defined(_WIN32) && !defined(IGUI_STATIC)
#  if (defined(IGUI_EXPORTS) || defined(igui_EXPORTS))  
#	  define IGUI_DLL  __declspec(dllexport)
#	  define IGUI_TEMPLATE_DLL 
#  else 
#	  define IGUI_DLL  __declspec(dllimport)
#	  define IGUI_TEMPLATE_DLL extern
#  endif
#else
#  define IGUI_DLL
#  define IGUI_TEMPLATE_DLL
#endif
