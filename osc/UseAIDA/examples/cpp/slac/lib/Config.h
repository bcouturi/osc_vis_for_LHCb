#ifndef Config_h
#define Config_h

#ifdef WIN32
#ifndef GNU_GCC
// Disable warning C4786: identifier was truncated to '255' characters in the debug information
  #pragma warning ( disable : 4786 )
#endif
#endif

#endif
