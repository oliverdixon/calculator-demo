/**
 * This is the primary testing driver of the calculator demonstration. As the
 * testing routines evolve into the full calculator, this will become the
 * standard entry point.
 *
 * @author Oliver Dixon
 */

#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "expression.h"

int main ( int argc, char ** argv )
{
        struct node_pool * pool;
        struct expression * expression;

        if ( argc < 2 ) {
                fputs ( "No expression provided!\n", stderr );
                return EXIT_FAILURE;
        }

        if ( ! ( pool = pool_initialise ( 0 ) ) ) {
                perror ( "pool_initialise" );
                return EXIT_FAILURE;
        }

        expression = expression_initialise ( argv [ 1 ], &pool, 1 );
        expression_status_print ( expression, ( expression_get_status
                ( expression ) == EXPR_OK ) ? stdout : stderr );

        expression_destruct ( expression );
        pool_destruct ( pool );

        return EXIT_SUCCESS;
}

