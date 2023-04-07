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
#include "stack.h"

/**
 * Pull some nodes from the pool, populate them, and push them to the
 * stack. Then, the stack is printed for convenience.
 *
 * @param pool the node pool to test
 * @param stack the stack to test
 */
static void test_routine ( struct node_pool * pool, struct stack * stack )
{
        struct node * node;

        for ( unsigned int i = 0; ( node = pool_new_node ( pool ) ); i++ ) {
                node_encode_literal ( node, ( number_t ) i );
                if ( !stack_push ( stack, node ) ) {
                        perror ( "stack_push" );
                        break;
                }
        }

        stack_print ( stack, node_format );
}

int main ( void )
{
        struct node_pool * pool;
        struct stack * stack;

        if ( ! ( pool = pool_initialise ( 0 ) ) ) {
                perror ( "pool_initialise" );
                return EXIT_FAILURE;
        }

        if ( ! ( stack = stack_initialise ( 0 ) ) ) {
                perror ( "stack_initialise" );

                pool_destruct ( pool );
                return EXIT_FAILURE;
        }

        test_routine ( pool, stack );

        stack_destruct ( stack );
        pool_destruct ( pool );

        return EXIT_SUCCESS;
}

