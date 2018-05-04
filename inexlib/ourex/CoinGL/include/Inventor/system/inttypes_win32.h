/* include/Inventor/system/inttypes.h.  Generated by configure.  */
#ifndef COIN_INTTYPES_H
#define COIN_INTTYPES_H

/*G.Barrand*/
#ifndef COIN_NO_INT_TYPEDEFS /*G.Barrand*/
/*  There is a clash with SDL with the below int8_t typedef with */
/* the one found in SDL/1.2.11/include/SDL_config_win32.h */
typedef	char int8_t;
#endif
typedef	unsigned char uint8_t;
typedef	signed short int16_t;
typedef	unsigned short uint16_t;
typedef	signed int int32_t;
typedef	unsigned int uint32_t;
typedef unsigned __int64 uint64_t;

/*uintptr_t*/
#include <stdarg.h> 

/* uintptr_t, intptr_t */
/* off_t (for zlib.h) */
#include <wchar.h> 

#endif /* !COIN_INTTYPES_H */
