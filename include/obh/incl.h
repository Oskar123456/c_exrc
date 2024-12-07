/* -----------------------
 * includes in one place..
 * ***********************
 * Oskar Bahner Hansen....
 * cph-oh82@cphbusiness.dk
 * 2024-11-12.............
 * ----------------------- */
#ifndef INCLS_H
#define INCLS_H
/* defaults */
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <assert.h>
/* linux */
#if defined(__unix__) || defined(__linux__)
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/termios.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <regex.h>
#endif
/* util */
#include "../sds/sds.h"
#include "../cJSON/cJSON.h"
#include "../stb/stb_ds.h"
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
/* own */
/* typedefs, macros, defs */
typedef  uint8_t  u8;
typedef  uint16_t u16;
typedef  uint32_t u32;
typedef  uint64_t u64;

typedef  int8_t   i8;
typedef  int16_t  i16;
typedef  int32_t  i32;
typedef  int64_t  i64;

#define max(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

#define min(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })

#endif
