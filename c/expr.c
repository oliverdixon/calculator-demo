/**
 * Implement the expression interface; see 'expr.h'.
 *
 * @author Oliver Dixon
 */

#include <stdlib.h>
#include <stdio.h>

#include "node.h"
#include "queue.h"
#include "debug.h"

#include "expr.h"

/**
 * The transparent expression
 */
struct expression {
        /**
         * The current read-head of the expression
         */
        const char * expr_head;

        /**
         * The transient status of the expression
         */
        enum expr_status status;

        /**
         * Internal nodal representation of the expression
         */
        struct queue * nodes;
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
        const char * new_rh;
        enum expr_status status = EXPR_OK;

        for ( ; *self->expr_head && status == EXPR_OK;
                        self->expr_head = new_rh ) {
                /* Pull a new node from the pool */
                if ( ! ( node = pull_node ( pools, &pool_idx, pool_count ) ) )
                        status = EXPR_NONODE;

                /* Tokenise. If the new read head matches the old one, then we
                 * have encountered a troublesome symbol. */
                if ( ( new_rh = node_encode ( node, self->expr_head ) ) ==
                                self->expr_head )
                        status = EXPR_BADSYMBOL;
        }

        self->status = status;
        return status;
}

enum expr_status expression_get_status ( struct expression * self )
{
        return self->status;
}

void expression_status_print ( struct expression * self )
{
        ( void ) status_str;

        if ( self->status == EXPR_BADSYMBOL )
                debug_printf ( "%s at: \"%s\"\n", status_str ( self->status ),
                        self->expr_head );
        else
                debug_puts ( status_str ( self->status ) );
}

struct expression * expression_initialise ( const char * expr,
                struct node_pool ** pools, unsigned int pool_count )
{
        struct expression * self;

        if ( ( self = malloc ( sizeof ( struct expression ) ) ) )
                if ( ! ( self->nodes = queue_initialise ( 0 ) ) ) {
                        expression_destruct ( self );
                        self = NULL;
                } else {
                        self->expr_head = expr;
                        self->status = EXPR_OK;

                        tokenise ( self, pools, pool_count );
                        debug_puts ( "Expression initialised" );
                }

        return self;
}

void expression_destruct ( struct expression * self )
{
        if ( self )
                queue_destruct ( self->nodes );

        free ( self );
        debug_puts ( "Expression destructed" );
}

