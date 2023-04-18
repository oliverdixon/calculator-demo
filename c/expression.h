/**
 * This interface handles arithmetic expressions for the calculator to evaluate.
 * Exposed routines are capable of undertaking the core stages of the arithmetic
 * computation:
 *
 *   - Tokenisation of an expression from a string form into an equivalent
 *     internal representation (IR);
 *
 *   - [TODO] Conversion of the IR from the infix order to postfix order with an
 *     implementation of operator-precedence parsing; and
 *
 *   - [TODO] Stack-based evaluation of the expression to a numerical value.
 *
 * @author Oliver Dixon
 */

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdio.h>
#include "node.h"

/**
 * The base opaque type of an expression
 */
struct expression;

/**
 * A status code indicating the status of functions concerned with the direct
 * handling of an arithmetic expression
 */
enum expr_status {
        EXPR_OK,
        EXPR_NONODE,
        EXPR_BADSYMBOL
};

/**
 * Initialise an expression type with the given string. This string is taken as
 * an infix expression and tokenised as such using the given node pool array. If
 * insufficient nodes are available, NULL is returned.
 *
 * @param expr the infix string expression to be tokenised
 * @param pools a list of node pools available to the parser
 * @param pool_count the number of node pools provided
 * @return the created expression type
 */
struct expression * expression_initialise ( const char * expr,
        struct node_pool ** pools, unsigned int pool_count );

/**
 * Shallow-destruct an entire expression type: constituent nodes are not freed.
 *
 * @param self the expression to be destructed
 */
void expression_destruct ( struct expression * self );

/**
 * Get the current status of the given expression.
 *
 * @param self the expression
 * @return the corresponding status
 */
enum expr_status expression_get_status ( struct expression * self );

/**
 * Write a formatted status report of the given expression to the given buffer.
 *
 * @param self the expression
 * @param buffer the buffer to which the string should be written
 */
void expression_status_print ( struct expression * self, FILE * buffer );

#endif /* EXPRESSION_H */

