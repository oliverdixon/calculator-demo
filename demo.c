/* Calculator Demonstration
 * OWD, 2023 */

#include <stdio.h>
#include <stdlib.h>

#include "am_stack.h"

static void clean_all ( struct am_stack * stack, struct am_node * pool )
{
        am_node_destroy_pool ( pool );
        am_stack_destroy ( stack );
}

int main ( int argc, char ** argv )
{
        struct am_stack * stack;
        struct am_node * pool;
        char output_string [ 16 ];

        /* Create the stack and a node pool, assuming default node counts. */

        if ( ! ( stack = am_stack_initialise ( 0 ) ) ) {
                perror ( "am_stack_initialise" );
                return EXIT_FAILURE;
        }

        if ( ! ( pool = am_node_create_pool ( 0 ) ) ) {
                perror ( "am_node_create_pool" );
                clean_all ( stack, pool );
                return EXIT_FAILURE;
        }

        /* Write some example nodes and push them to the stack. */
        for ( size_t i = 0; i < DEFAULT_NODE_COUNT; i++ )
                if ( !am_stack_push ( stack, am_node_format_number
                                ( pool, i, i ) ) ) {
                        perror ( "am_stack_push" );
                        clean_all ( stack, pool );
                        return EXIT_FAILURE;
                }

        /* Pop them all off again, for some reason. */
        for ( size_t i = 0; i < DEFAULT_NODE_COUNT; i++ )
                puts ( am_node_tostring ( output_string, 16,
                        am_stack_pop ( stack ) ) );

        /* Destroy the node pool and stack. */
        clean_all ( stack, pool );

        return EXIT_SUCCESS;
}

