/**
 * Implement the expression interface; see 'expression.h'.
 *
 * @author Oliver Dixon
 */

#include <stdlib.h>
#include <stdio.h>

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

        /**
         * The transient status of the expression
         */
        enum expr_status status;
};

/**
 * Grab the next available node from the provided pools.
 *
 * @param pools the list of available node pools
 * @param pool_idx the current position in the node pool list
 * @param pool_count the number of available given pools
 * @return the requested node, or NULL if no such node is available
 */
static inline struct node * pull_node ( struct node_pool ** pools,
                unsigned int * pool_idx, unsigned int pool_count )
{
        struct node * node = NULL;

        /* If we could not grab a node from the current pool, look to the next
         * one, et cetera, until we've expended all available pools. If, at any
         * point, we successfully grab a node, then we are done. */
        while ( ! ( node = pool_new_node ( pools [ *pool_idx ] ) ) &&
                        *pool_idx++ < pool_count );

        return node;
}

/**
 * Parse the expression status into a human-readable string.
 *
 * @param status the expression status under examination
 * @return a human-readable representation of the expression status
 */
static inline const char * status_str ( enum expr_status status )
{
        switch ( status ) {
                case EXPR_OK:        return "Expression OK";
                case EXPR_NONODE:    return "Insufficient nodes";
                case EXPR_BADSYMBOL: return "Unexpected symbol";

                default: return "Unknown expression status";
        }
}

/**
 * Tokenise the expression in the given expression to its equivalent internal
 * representation, according to the standard rules of arithmetic defined by the
 * Node interface.
 *
 * @param self the expression of concern
 * @param pools the list of available node pools
 * @param pool_count the number of available given pools
 * @return a status code according to the standard expression error schema
 */
static enum expr_status tokenise ( struct expression * self,
                struct node_pool ** pools, unsigned int pool_count )
{
        struct node * node;
        unsigned int pool_idx = 0;
        char fmt_buffer [ 32 ];
        const char * new_rh;
        enum expr_status status = EXPR_OK;

        while ( *self->expr_head ) {
                /* Pull a new node from the pool */
                if ( ! ( node = pull_node ( pools, &pool_idx, pool_count ) ) ) {
                        status = EXPR_NONODE;
                        break;
                }

                /* Tokenise. If the new read head matches the old one, then we
                 * have encountered a troublesome symbol. */
                if ( ( new_rh = node_encode ( node, self->expr_head ) ) ==
                                self->expr_head ) {
                        status = EXPR_BADSYMBOL;
                        break;
                }

                self->expr_head = new_rh;
                node_format ( node, fmt_buffer, 32 );
                printf ( "New head: \"%s\"\tNode: %s\n", self->expr_head,
                        fmt_buffer );
        }

        self->status = status;
        return status;
}

enum expr_status expression_get_status ( struct expression * self )
{
        return self->status;
}

void expression_status_print ( struct expression * self, FILE * buffer )
{
        fputs ( status_str ( self->status ), buffer );

        if ( self->status == EXPR_BADSYMBOL ) {
                fputs ( " at: \"", buffer );
                fputs ( self->expr_head, buffer );
                fputc ( '\"', buffer );
        }

        fputc ( '\n', buffer );
}

struct expression * expression_initialise ( const char * expr,
                struct node_pool ** pools, unsigned int pool_count )
{
        struct expression * self;

        if ( ( self = malloc ( sizeof ( struct expression ) ) ) ) {
                self->expr_head = expr;
                self->status = EXPR_OK;

                tokenise ( self, pools, pool_count );
        }

        return self;
}

void expression_destruct ( struct expression * self )
{
        free ( self );
}

