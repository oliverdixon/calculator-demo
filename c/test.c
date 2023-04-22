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

        if ( argc < 2 ) {
                fputs ( "No expression provided!\n", stderr );
                return EXIT_FAILURE;
        }

        if ( ! ( pool = pool_initialise ( 0 ) ) ) {
                debug_perror ( "Could not initialise the pool" );
                return EXIT_FAILURE;
        }

        if ( ! ( expr = expression_initialise ( argv [ 1 ], &pool, 1 ) ) ) {
                debug_perror ( "Could not initialise the expression" );
                pool_destruct ( pool );
                return EXIT_FAILURE;
        }

        expression_status_print ( expr );

        expression_destruct ( expr );
        pool_destruct ( pool );

        return EXIT_SUCCESS;
}

