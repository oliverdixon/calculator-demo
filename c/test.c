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

int main ( void )
{
        struct node_pool * pool;
        struct expression * expression;

        if ( ! ( pool = pool_initialise ( 0 ) ) ) {
                perror ( "pool_initialise" );
                return EXIT_FAILURE;
        }

        expression = expression_initialise ( "1+2", &pool, 1 );

        expression_destruct ( expression );
        pool_destruct ( pool );

        return EXIT_SUCCESS;
}

