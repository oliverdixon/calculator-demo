/**
 * This interface handles arithmetic expressions for the calculator to evaluate.
 * Exposed routines are capable of undertaking the:
 *
 *  - ...\ tokenisation of an expression from a string form into an equivalent
 *         internal representation (IR);
 *  - ...\ subsequent conversion of the IR from the infix order to postfix
 *         order; and
 *  - ...\ evaluation of the expression to a numerical value.
 *
 * @author Oliver Dixon
 */

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "node.h"

/**
 * The base opaque type of an expression
 */
struct expression;

/**
 * Initialise an expression type with the given string. This string is taken as
 * an infix expression and tokenised as such using the given node pool array. If
 * insufficient nodes are available, NULL is returned. TODO error-handling.
 *
 * @param expr the infix string expression to be tokenised
 * @param pools a list of node pools available to the parser
 * @param pool_count the number of node pools provided
 * @return the created expression type
 */
struct expression * expression_initialise ( const char * expr,
        struct node_pool ** pools, unsigned int pool_count );

// int expression_postfix ( struct expression * self );

// number_t expression_evaluate ( struct expression * self );

/**
 * Shallow-destruct an entire expression type: constituent nodes are not freed.
 *
 * @param self the expression to be destructed
 */
void expression_destruct ( struct expression * self );

#endif /* EXPRESSION_H */

