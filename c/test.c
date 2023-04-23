/**
 * This is the primary testing driver of the calculator demonstration. As the
 * testing routines evolve into the full calculator, this will become the
 * standard entry point.
 *
 * @author Oliver Dixon
 */

#include <stdlib.h>
#include <stdio.h>

#include "node.h"
#include "expr.h"

/**
 * A wrapper to test all aspects of the Expression interface, including
 * initialisation, tokenisation, and conversion. A single pool is supported, and
 * any errors are printed directly to stderr.
 *
 * @param pool the node pool
 * @param expr_str the string-infix representation of the expression
 * @return zero on success, -1 on error
 */
static int test_expression ( struct node_pool * pool, const char * expr_str )
{
        struct expression * expr;
        enum expr_status status = EXPR_OK;

        if ( ! ( expr = expression_initialise ( expr_str, 0 ) ) )

                /* Do not use 'expression_perror' here, since we do not have a
                 * function expression object on which the method can be called.
                 * Furthermore, 'errno' is well-defined to be set upon failure
                 * of 'expression_initialise'. */
                perror ( "Could not initialise the expression" );

        else if ( ( status = expression_tokenise ( expr, &pool, 1 ) )
                        != EXPR_OK )

                expression_perror ( expr, "Could not tokenise the expression",
                        status );

        else if ( ( status = expression_postfix ( expr ) ) != EXPR_OK )

                expression_perror ( expr, "Could not convert the expression " \
                        "to an equivalent postfix form", status );

        expression_destruct ( expr );
        return ( expr && status == EXPR_OK ) ? 0 : -1;
}

int main ( int argc, char ** argv )
{
        struct node_pool * pool = NULL;
        int status = EXIT_SUCCESS;

        if ( argc < 2 ) {
                fputs ( "No expression provided!\n", stderr );
                status = EXIT_FAILURE;
        } else if ( ! ( pool = pool_initialise ( 0 ) ) ) {
                perror ( "Could not initialise the node pool" );
                status = EXIT_FAILURE;
        } else if ( test_expression ( pool, argv [ 1 ] ) == -1 )
                status = EXIT_FAILURE;

        return status;
}

