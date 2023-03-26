/* Arithmetic Stack API
 * OWD, 2023 */

#ifndef _AM_STACK_H
#define _AM_STACK_H

#include <stddef.h>

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
        AM_OP_POWER,
        AM_OP_DIVIDE,
        AM_OP_MULTIPLY,
        AM_OP_ADD,
        AM_OP_SUBTRACT
};

/* Individual stack nodes for tokenising arithmetic expressions */
struct am_stack_node {
        enum am_data_type type;
        union {
                am_number_t value;
                enum am_operator op;
        };
};

/* The arithmetic stack superstructure */
struct am_stack {
        struct am_stack_node ** data;
        size_t capacity, size;
};

int am_stack_initialise ( struct am_stack *, size_t );
void am_stack_destroy ( struct am_stack * );

#endif /* _AM_STACK_H */

