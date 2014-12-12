#ifndef DOSBOX_CONFIG_H
#define DOSBOX_CONFIG_H

#include <inttypes.h>

//#define PROFILE_SIMPLE_FILE_CALL_TRACE 1
//#define PROFILE_UNIQUE_CALL_LIST 1
//#define PROFILE_TREE_CALL_TRACE 1

/* Define to 1 to enable internal debugger, requires libcurses */
//#define C_DEBUG 1
/* Define to 1 to enable heavy debugging, also have to enable C_DEBUG */
//#define C_HEAVY_DEBUG 1

/* Determines if the compilers supports always_inline attribute. */
//#define C_ATTRIBUTE_ALWAYS_INLINE 1

/* Determines if the compilers supports fastcall attribute. */
/* #undef C_ATTRIBUTE_FASTCALL */

/* Define to 1 to use inlined memory functions in cpu core */
/* #undef C_CORE_INLINE */

/* Define to 1 if you want serial passthrough support (Win32, Posix and OS/2).
 */
//#define C_DIRECTSERIAL 1

/* Define to 1 to enable floating point emulation */
#define C_FPU 1

/* Determines if the compilers supports attributes for structures. */
#define C_HAS_ATTRIBUTE 1

/* Determines if the compilers supports __builtin_expect for branch
   prediction. */
#define C_HAS_BUILTIN_EXPECT 1

/* Define to 1 if you have the mprotect function */
//#define C_HAVE_MPROTECT 1

/* Define to 1 to use a unaligned memory access */
#define C_UNALIGNED_MEMORY 1

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

#if C_ATTRIBUTE_ALWAYS_INLINE
#define INLINE inline __attribute__((always_inline))
#else
#define INLINE inline
#endif

#if C_ATTRIBUTE_FASTCALL
#define DB_FASTCALL __attribute__((fastcall))
#else
#define DB_FASTCALL
#endif

#if C_HAS_ATTRIBUTE
#define GCC_ATTRIBUTE(x) __attribute__ ((x))
#else
#define GCC_ATTRIBUTE(x) /* attribute not supported */
#endif

#if C_HAS_BUILTIN_EXPECT
#define GCC_UNLIKELY(x) __builtin_expect((x),0)
#define GCC_LIKELY(x) __builtin_expect((x),1)
#else
#define GCC_UNLIKELY(x) (x)
#define GCC_LIKELY(x) (x)
#endif

typedef double Real64;
typedef uint8_t Bit8u;
typedef int8_t Bit8s;
typedef uint16_t Bit16u;
typedef int16_t Bit16s;
typedef uint32_t Bit32u;
typedef int32_t Bit32s;
typedef uint64_t Bit64u;
typedef int64_t Bit64s;

#if defined __x86_64__ || defined HOST_64BIT
typedef Bit64u Bitu;
typedef Bit64s Bits;
#else
typedef Bit32u Bitu;
typedef Bit32s Bits;
#endif

#endif //DOSBOX_CONFIG_H
