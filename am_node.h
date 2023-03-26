/* Arithmetic Node API
 * OWD, 2023 */

#ifndef _AM_NODE_H
#define _AM_NODE_H

#include <stddef.h>

#define DEFAULT_NODE_COUNT ( 16 )

typedef float am_number_t;

/* Supported arithmetic operators, in descending order of precedence */
enum am_operator {
        AM_OP_UNKNOWN,

        AM_OP_POWER,
        AM_OP_DIVIDE,
        AM_OP_MULTIPLY,
        AM_OP_ADD,
        AM_OP_SUBTRACT,

        AM_OP_TYPES_COUNT
};

/* Individual stack nodes for tokenising arithmetic expressions */
struct am_node;

struct am_node * am_node_create_pool ( size_t );
void am_node_destroy_pool ( struct am_node * );
struct am_node * am_node_encode_number ( struct am_node *, size_t,
        am_number_t );
struct am_node * am_node_encode_operator ( struct am_node *, size_t,
                enum am_operator );
char * am_node_tostring ( char *, size_t, struct am_node * );

#endif /* _AM_NODE_H */

