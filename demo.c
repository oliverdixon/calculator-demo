/* Calculator Demonstration
 * OWD, 2023 */

#include <stdio.h>
#include <stdlib.h>

#include "am_stack.h"

int main ( int argc, char ** argv )
{
        struct am_stack * stack;
        struct am_node * pool;

        /* Create the stack and a node pool, assuming default node counts. */

        if ( ! ( stack = am_stack_initialise ( 0 ) ) ) {
                perror ( "am_stack_initialise" );
                return EXIT_FAILURE;
        }

        if ( ! ( pool = am_node_create_pool ( 0 ) ) ) {
                perror ( "am_node_create_pool" );
                return EXIT_FAILURE;
        }

        /* Destroy the node pool and stack. */

        am_node_destroy_pool ( pool );
        am_stack_destroy ( stack );

        return EXIT_SUCCESS;
}

