/**
 * This is the primary testing driver of the calculator demonstration. As the
 * testing routines evolve into the full calculator, this will become the
 * standard entry point.
 *
 * @author Oliver Dixon
 */

#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "node.h"
#include "expr.h"
#include "debug.h"

int main ( int argc, char ** argv )
{
        struct node_pool * pool;
        struct expression * expr;
        enum expr_status status = EXPR_OK;

        if ( argc < 2 ) {
                fputs ( "No expression provided!\n", stderr );
                return EXIT_FAILURE;
        }

        if ( ! ( pool = pool_initialise ( 0 ) ) ) {
                perror ( "Could not initialise the pool" );
                return EXIT_FAILURE;
        }

        if ( ! ( expr = expression_initialise ( argv [ 1 ], 0 ) ) ) {
                perror ( "Could not initialise the expression" );
                pool_destruct ( pool );
                return EXIT_FAILURE;
        }

        if ( ( status = expression_tokenise ( expr, &pool, 1 ) ) != EXPR_OK ) {
                expression_perror ( expr, "Could not tokenise the expression",
                        status );
                expression_destruct ( expr );
                pool_destruct ( pool );
                return EXIT_FAILURE;
        }

        if ( ( status = expression_postfix ( expr ) ) != EXPR_OK ) {
                expression_perror ( expr, "Could not convert the expression " \
                        "to an equivalent postfix form", status );
                expression_destruct ( expr );
                pool_destruct ( pool );
                return EXIT_FAILURE;
        }

        expression_destruct ( expr );
        pool_destruct ( pool );

        return EXIT_SUCCESS;
}

