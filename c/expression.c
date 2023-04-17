/**
 * Implement the expression interface; see 'expression.h'.
 *
 * @author Oliver Dixon
 */

#include <stdlib.h>

#include "stack.h"
#include "node.h"
#include "expression.h"

/**
 * The transparent expression
 */
struct expression {
        /**
         * The operator stack, used according to the Shunting-Yard algorithm
         */
        struct stack * op_stack;

        /**
         * The current read-head of the expression
         */
        const char * expr_head;

        /**
         * The internal nodal representation of the expression
         */
        struct node ** nodes;
};

struct expression * expression_initialise ( const char * expr,
                struct node_pool ** pools, unsigned int pool_count )
{
        struct expression * self;

        ( void ) pools;         // TODO
        ( void ) pool_count;    // TODO

        if ( ( self = malloc ( sizeof ( struct expression ) ) ) )
                self->expr_head = expr;

        return self;
}

void expression_destruct ( struct expression * self )
{
        free ( self );
}

