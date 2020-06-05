#ifndef DEBUG_H
#define DEBUG_H 1

#include <QtCore/QDebug>
#include <stdbool.h>

/*
 * Debug print utils
 */
#ifndef NO_DEBUG

#define dprint(s)                   qDebug() << s
#define dprintln(s)                 qDebug() << s
#define dprintf(fmt, ...)           qDebug() << QString().sprintf(fmt, ##__VA_ARGS__);
#define dmsg(s)                     dprintf("%s at %s: %S\n", __FILE__, __LINE__, PSTR(s))

#if 0
/* Deprecated. DO NOT USE these anymore, use dprintf instead. */
#define debug(s)                    do { if (debug_enable) print(s); } while (0)
#define debugln(s)                  do { if (debug_enable) println(s); } while (0)
#define debug_msg(s)                do { \
    if (debug_enable) { \
        print(__FILE__); print(" at "); print_dec(__LINE__); print(" in "); print(": "); print(s); \
    } \
} while (0)
#define debug_dec(data)             do { if (debug_enable) print_dec(data); } while (0)
#define debug_decs(data)            do { if (debug_enable) print_decs(data); } while (0)
#define debug_hex4(data)            do { if (debug_enable) print_hex4(data); } while (0)
#define debug_hex8(data)            do { if (debug_enable) print_hex8(data); } while (0)
#define debug_hex16(data)           do { if (debug_enable) print_hex16(data); } while (0)
#define debug_hex32(data)           do { if (debug_enable) print_hex32(data); } while (0)
#define debug_bin8(data)            do { if (debug_enable) print_bin8(data); } while (0)
#define debug_bin16(data)           do { if (debug_enable) print_bin16(data); } while (0)
#define debug_bin32(data)           do { if (debug_enable) print_bin32(data); } while (0)
#define debug_bin_reverse8(data)    do { if (debug_enable) print_bin_reverse8(data); } while (0)
#define debug_bin_reverse16(data)   do { if (debug_enable) print_bin_reverse16(data); } while (0)
#define debug_bin_reverse32(data)   do { if (debug_enable) print_bin_reverse32(data); } while (0)
#define debug_hex(data)             debug_hex8(data)
#define debug_bin(data)             debug_bin8(data)
#define debug_bin_reverse(data)     debug_bin8(data)
#endif

#else /* NO_DEBUG */

#define dprint(s)                   ((void)0)
#define dprintln(s)                 ((void)0)
#define dprintf(fmt, ...)           ((void)0)
#define dmsg(s)                     ((void)0)
#if 0
#define debug(s)                    ((void)0)
#define debugln(s)                  ((void)0)
#define debug_msg(s)                ((void)0)
#define debug_dec(data)             ((void)0)
#define debug_decs(data)            ((void)0)
#define debug_hex4(data)            ((void)0)
#define debug_hex8(data)            ((void)0)
#define debug_hex16(data)           ((void)0)
#define debug_hex32(data)           ((void)0)
#define debug_bin8(data)            ((void)0)
#define debug_bin16(data)           ((void)0)
#define debug_bin32(data)           ((void)0)
#define debug_bin_reverse8(data)    ((void)0)
#define debug_bin_reverse16(data)   ((void)0)
#define debug_bin_reverse32(data)   ((void)0)
#define debug_hex(data)             ((void)0)
#define debug_bin(data)             ((void)0)
#define debug_bin_reverse(data)     ((void)0)
#endif
#endif /* NO_DEBUG */

#endif
