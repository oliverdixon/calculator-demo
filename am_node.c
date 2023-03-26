/* Arithmetic Node Implementation
 * OWD, 2023 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <string.h>

#include "am_node.h"

/* Arithmetic stack node types */
enum am_data_type {
        AM_DATA_UNKNOWN,

        AM_DATA_NUMBER,
        AM_DATA_OPERATOR,
        AM_DATA_LPAREN,
        AM_DATA_RPAREN,

        AM_DATA_TYPES_COUNT
};

/* Complete definition for the arithmetic node */
struct am_node {
        enum am_data_type type;
        union {
                am_number_t value;
                enum am_operator op;
        };
};

/* Create a node pool of a given size on the heap. If the suggested size is
 * zero, a sensible default is assumed. This function returns the address of the
 * pool on success, or zero on error. */
struct am_node * am_node_create_pool ( size_t cap )
{
        return ( calloc ( ( cap ) ? cap : DEFAULT_NODE_COUNT,
                sizeof ( struct am_node ) ) );
}

/* Destroy the node pool. */
void am_node_destroy_pool ( struct am_node * pool )
{
        free ( pool );
}

/* Encode a number into the given node, returning the packed node. */
struct am_node * am_node_encode_number ( struct am_node * pool, size_t idx,
                am_number_t value )
{
        pool [ idx ].type = AM_DATA_NUMBER;
        pool [ idx ].value = value;

        return & ( pool [ idx ] );
}

/* Encode an operator into the given node, returning the packed node. */
struct am_node * am_node_encode_operator ( struct am_node * pool, size_t idx,
                enum am_operator op )
{
        pool [ idx ].type = AM_DATA_OPERATOR;
        pool [ idx ].op = op;

        return & ( pool [ idx ] );
}

/* Write a constant source string to a destination string, performing basic
 * length checks as necessary. This is a simple alternative for snprintf(3) for
 * the node formatters. */
static inline size_t _simple_writeout ( char * dest, const char * src,
                size_t len )
{
        const size_t srclen = strlen ( src );
        assert ( len > 0 );

        if ( srclen >= len ) {
                len--;
                for ( size_t i = 0; i < len; i++ )
                        /* The strings are short, and since we already know the
                         * source and destination string lengths, this linear
                         * approach is desirable to strncpy(3), which invariably
                         * recalculates the source length. We cannot necessarily
                         * just end the string early by replacing a character
                         * with a NULL-terminator, since the source string may
                         * be a string literal, existing in read-only memory. */

                        dest [ i ] = src [ i ];
        } else
                strcpy ( dest, src );

        return srclen;
}

/* Format a number as a string */
static inline size_t _formatter_number ( char * str, size_t len,
                struct am_node * node )
{
        return snprintf ( str, len, "Literal: %.3f", node->value );
}

/* Catch-all string formatter for node types with no defined string
 * representation. */
static inline size_t _formatter_not_implemented ( char * str, size_t len,
                struct am_node * node )
{
        ( void ) node;
        return _simple_writeout ( str, "Not Implemented", len );
}

/* Format an operator as a string */
static inline size_t _formatter_operator ( char * str, size_t len,
                struct am_node * node )
{
        static char * opstr [ ] = { "Unknown", "Power", "Divide",
                "Multiply", "Add", "Subtract" };
        size_t used;
        assert ( sizeof ( opstr ) / sizeof ( *opstr ) == AM_OP_TYPES_COUNT );

        used = _simple_writeout ( str, "Operator: ", len );

        return used + _simple_writeout ( & ( str [ used ] ), ( node->op >= 1 &&
                node->op < AM_OP_TYPES_COUNT ) ? opstr [ node->op ] :
                        opstr [ 0 ], len - used );
}

/* Format a parenthesis (left or right) as a string */
static inline size_t _formatter_paren ( char * str, size_t len,
                struct am_node * node )
{
        return _simple_writeout ( str, ( node->type == AM_DATA_LPAREN ) ?
                "Left Parenthesis" : "Right Parenthesis", len );
}

/* Format the given node into the given string, using a variety of formatters.
 * If the string has an unreasonably small capacity, NULL is returned and errno
 * is set. Otherwise, the string is compiled as necessary and then returned for
 * the caller's convenience. */
char * am_node_tostring ( char * str, size_t len, struct am_node * node )
{
        static const size_t MINIMUM_LENGTH = 16;
        static size_t ( * formatter [ AM_DATA_TYPES_COUNT + 1 ] )
                ( char *, size_t, struct am_node * )
                        = { _formatter_not_implemented, /* Unknown         */
                            _formatter_number,          /* Number          */
                            _formatter_operator,        /* Operator        */
                            _formatter_paren,           /* (L) Parenthesis */
                            _formatter_paren,           /* (R) Parenthesis */
                            _formatter_not_implemented  /* Type-counter    */
                    };

        if ( len < MINIMUM_LENGTH ) {
                errno = EINVAL;
                return NULL;
        }

        assert ( sizeof ( formatter ) / sizeof ( *formatter ) ==
                AM_DATA_TYPES_COUNT + 1 );

        /* The formatters will always add a NULL-terminator; if it had intended
         * to overwrite the available string space, then the output will be
         * truncated, so we add a three-character marker to indicate as such. */
        if ( formatter [ node->type ] ( str, len, node ) >= len ) {
                str [ len - 2 ] = '.';
                str [ len - 3 ] = '.';
                str [ len - 4 ] = '.';
        }

        return str;
}

