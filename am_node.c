/* Arithmetic Node Implementation
 * OWD, 2023 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "am_node.h"

/* Arithmetic stack node types */
enum am_data_type {
        AM_DATA_UNKNOWN,

        AM_DATA_NUMBER,
        AM_DATA_OPERATOR,
        AM_DATA_LPAREN,
        AM_DATA_RPAREN,
};

/* Supported arithmetic operators, in descending order of precedence */
enum am_operator {
        AM_OP_UNKNOWN,

        AM_OP_POWER,
        AM_OP_DIVIDE,
        AM_OP_MULTIPLY,
        AM_OP_ADD,
        AM_OP_SUBTRACT
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
struct am_node * am_node_format_number ( struct am_node * pool, size_t idx,
                am_number_t value )
{
        pool [ idx ].type = AM_DATA_LPAREN;
        pool [ idx ].value = value;

        return & ( pool [ idx ] );
}

#if 0
static const char * stringify_operator ( enum am_operator op )
{
        static const char * dictionary [ ] = { "Unknown", "Power", "Divide",
                "Multiply", "Add", "Subtract" };

        return ( op >= 0 && op <= sizeof ( dictionary ) /
                sizeof ( *dictionary ) ) ? dictionary [ op ] : dictionary [ 0 ];
}
#endif

char * am_node_tostring ( char * str, size_t len, struct am_node * node )
{
        static const size_t MINIMUM_LENGTH = 16;
        size_t chrs = 0;

        if ( len < MINIMUM_LENGTH ) {
                errno = EINVAL;
                return NULL;
        }

        /* TODO: have this as an array of function pointers (formatters for
         * their respective types), permuted over am_data_type. */
        switch ( node->type ) {
                case AM_DATA_NUMBER:
                        chrs = snprintf ( str, len, "Literal: %.3f",
                                node->value);
                        break;
                default:
                        chrs = snprintf ( str, len, "Not Implemented" );
        }

        /* snprintf(3) will always add a NULL-terminator; if it had intended to
         * overwrite the available string space, then the output will be
         * truncated, so we add a three-character marker to indicate as such. */
        if ( chrs >= len ) {
                str [ len - 2 ] = '.';
                str [ len - 3 ] = '.';
                str [ len - 4 ] = '.';
        }

        return str;
}

