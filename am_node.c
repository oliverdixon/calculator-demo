/* Arithmetic Node Implementation
 * OWD, 2023 */

#include <stdlib.h>

#include "am_node.h"

typedef float am_number_t;

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

