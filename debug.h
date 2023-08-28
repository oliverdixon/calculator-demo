/**
 * A primitive debugging interface, exposing compile-time debugging analogues
 * for the printf(3) and puts(3) standard library functions. By default, these
 * are no-ops, however a couple of macros can be defined to determine the
 * verbosity of the messages:
 *
 *  - DEBUG_VERBOSE: prefix each message with a "[DEBUG]" indicator and
 *    corresponding the file and line number of the caller; or
 *
 *  - DEBUG_QUIET: prefix each message with a "[DEBUG]" indicator.
 *
 * @author Oliver Dixon
 */

#ifndef DEBUG_H
#define DEBUG_H

#if defined DEBUG_VERBOSE
#    define DEBUG_XSTR_(x) #x
#    define DEBUG_XSTR(x) DEBUG_XSTR_ ( x )
#    define DEBUG_PREFIX "[DEBUG @ " __FILE__ ":" DEBUG_XSTR ( __LINE__ ) "] "
#elif defined DEBUG_QUIET
#    define DEBUG_PREFIX "[DEBUG] "
#endif

#ifdef DEBUG_PREFIX
#    include <stdio.h>
#    define debug_printf(fmt, ...) \
        fprintf ( stderr, DEBUG_PREFIX fmt, __VA_ARGS__ )
#    define debug_puts(str)                 \
        do {                                \
            fputs ( DEBUG_PREFIX, stderr ); \
            fputs ( str, stderr );          \
            fputc ( '\n', stderr );         \
        } while ( 0 )
#    define debug_perror(str) \
        perror ( DEBUG_PREFIX str )
#else
#    define debug_printf(fmt, ...)
#    define debug_puts(str)
#    define debug_perror(str)
#endif

#endif /* DEBUG_H */

