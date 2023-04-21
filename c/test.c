/**
 * This is the primary testing driver of the calculator demonstration. As the
 * testing routines evolve into the full calculator, this will become the
 * standard entry point.
 *
 * @author Oliver Dixon
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "node.h"
#include "expr.h"
#include "debug.h"

int main ( int argc, char ** argv )
{
        struct node_pool * pool;
        struct expression * expression;

        if ( argc < 2 ) {
                debug_puts ( "No expression provided!" );
                return EXIT_FAILURE;
        }

        if ( ! ( pool = pool_initialise ( 0 ) ) ) {
                debug_puts ( "Could not initialise the pool!" );
                debug_puts ( strerror ( errno ) );
                return EXIT_FAILURE;
        }

        expression = expression_initialise ( argv [ 1 ], &pool, 1 );
        expression_status_print ( expression );

        expression_destruct ( expression );
        pool_destruct ( pool );

        return EXIT_SUCCESS;
}

