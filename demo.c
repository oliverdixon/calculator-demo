/* Calculator Demonstration
 * OWD, 2023 */

#include <stdio.h>
#include <stdlib.h>

#include "am_stack.h"

static inline void _clean_all ( struct am_stack * stack, struct am_node * pool )
{
        am_node_destroy_pool ( pool );
        am_stack_destroy ( stack );
}

int main ( int argc, char ** argv )
{
        const size_t TEST_STRING_LENGTH = 20;
        struct am_stack * stack;
        struct am_node * pool;
        char output_string [ TEST_STRING_LENGTH ];

        /* Create the stack and a node pool, assuming default node counts. */

        if ( ! ( stack = am_stack_initialise ( 0 ) ) ) {
                perror ( "am_stack_initialise" );
                return EXIT_FAILURE;
        }

        if ( ! ( pool = am_node_create_pool ( 0 ) ) ) {
                perror ( "am_node_create_pool" );
                _clean_all ( stack, pool );
                return EXIT_FAILURE;
        }

        /* Write some example nodes and push them to the stack. */
        for ( size_t i = 0; i < DEFAULT_NODE_COUNT / 2; i++ )
                if ( !am_stack_push ( stack, am_node_encode_number
                                ( pool, i, i ) ) ) {
                        perror ( "am_stack_push" );
                        _clean_all ( stack, pool );
                        return EXIT_FAILURE;
                }

        /* Some operators... */
        for ( size_t i = DEFAULT_NODE_COUNT / 2; i < DEFAULT_NODE_COUNT; i++ )
                if ( !am_stack_push ( stack, am_node_encode_operator
                                ( pool, i, i % AM_OP_TYPES_COUNT ) ) ) {
                        perror ( "am_stack_push" );
                        _clean_all ( stack, pool );
                        return EXIT_FAILURE;
                }

        /* Pop them all off again, for some reason. */
        for ( size_t i = 0; i < DEFAULT_NODE_COUNT; i++ )
                puts ( am_node_tostring ( output_string, TEST_STRING_LENGTH,
                        am_stack_pop ( stack ) ) );

        /* Destroy the node pool and stack. */
        _clean_all ( stack, pool );

        return EXIT_SUCCESS;
}

