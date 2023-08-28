/**
 * This interface handles arithmetic expressions for the calculator to evaluate.
 * Exposed routines are capable of undertaking the core stages of the arithmetic
 * computation:
 *
 *   - Tokenisation of an expression from a string form into an equivalent
 *     internal representation (IR);
 *
 *   - Conversion of the IR from the infix order to postfix order with an
 *     implementation of operator-precedence parsing; and
 *
 *   - [TODO] Stack-based evaluation of the expression to a numerical value.
 *
 * @author Oliver Dixon
 */

#ifndef EXPR_H
#define EXPR_H

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
        EXPR_BADSYMBOL,
        EXPR_NOEXPR,
        EXPR_INTERR,
};

/**
 * Initialise an expression type with the given string. This string is taken as
 * an infix expression. If this function fails, then 'errno' is set
 * appropriately.
 *
 * @param expr the infix string expression to be tokenised
 * @return the created expression, or NULL on failure
 */
struct expression * expression_initialise ( const char * expr,
        unsigned int capacity );

/**
 * Shallow-destruct an entire expression type: constituent nodes are not freed.
 *
 * @param self the expression to be destructed
 */
void expression_destruct ( struct expression * self );

/**
 * Tokenise the expression in the given expression to its equivalent internal
 * representation, according to the standard rules of arithmetic defined by the
 * Node interface.
 *
 * @param self the expression
 * @param pools the list of available node pools
 * @param pool_count the number of available given pools
 * @return a status code according to the standard expression error schema
 */
enum expr_status expression_tokenise ( struct expression * self,
        struct node_pool ** pools, unsigned int pool_count );

/**
 * Format and print a human-readable report of the status of the given
 * expression, prefixed with an optional message, to the standard error buffer.
 *
 * @param self the expression
 * @param msg a prefix string, or NULL
 * @param status the status to interpret
 */
void expression_perror ( struct expression * self, const char * msg,
        enum expr_status status );

/**
 * Convert the tokenised expression into an equivalent postfix (a.k.a.
 * Reverse-Polish notation.
 *
 * @param self the expression to convert
 * @return the new status of the given expression
 */
enum expr_status expression_postfix ( struct expression * self );

#endif /* EXPR_H */

