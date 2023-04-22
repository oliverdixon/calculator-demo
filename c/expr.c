/**
 * Implement the expression interface; see 'expr.h'.
 *
 * @author Oliver Dixon
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "node.h"
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
        struct node ** data;

        /**
         * The (variable) number of nodes allowed by this expression
         */
        unsigned int capacity;

        /**
         * The current node-under-inspection in the nodal array. During the
         * initial tokenisation, this should be incremented as usual. When the
         * conversion to postfix notation is occurring, it should be reset to
         * indicate that future commits should overwrite from the front.
         */
        unsigned int idx;
};

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
                case EXPR_NOEXPR:    return "Insufficient expression capacity";

                default: return "Unknown expression status";
        }
}

/**
 * Determine whether the given expression node list must be increased to
 * accommodate a new (unseen) node. If the current capacity is insufficient, its
 * size is doubled; otherwise, nothing is done.
 *
 * @param self the node list
 * @return the ability to add a new node
 */
static bool realloc_check ( struct expression * self )
{
        struct node ** new_data;

        if ( self->idx + 1 >= self->capacity ) {
                if ( ! ( new_data = realloc ( self->data,
                                sizeof ( struct node * ) *
                                ( self->capacity << 1 ) ) ) )
                        return false;

                self->data = new_data;
                self->capacity <<= 1;
        }

        return true;
}

/**
 * Commit (append) a structured node to the expression
 *
 * @param self the expression
 * @param node the node to be added
 * @return the committed node, or NULL if the operation could not be completed
 */

static inline bool commit_node ( struct expression * self, struct node * node )
{
        if ( !realloc_check ( self ) )
                return NULL;

        self->data [ self->idx++ ] = node;
        return true;
}

/**
 * Extract the next node from the start of the expression.
 *
 * @param self the expression
 * @return the next germane node, or NULL if no nodes are encoded
 */

static inline struct node * extract_node ( struct expression * self )
{
        return ( self->idx ) ? self->data [ --self->idx ] : NULL;
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
        const char * new_rh = NULL;
        enum expr_status status = EXPR_OK;

        for ( ; *self->expr_head && status == EXPR_OK;
                        self->expr_head = new_rh )
                /* Pull a new node from the pool */
                if ( ! ( node = pool_pull_node ( pools, &pool_idx,
                                pool_count ) ) )
                        status = EXPR_NONODE;

                /* Tokenise. If the new read head matches the old one, then we
                 * have encountered a troublesome symbol. */
                else if ( ( new_rh = node_encode ( node, self->expr_head ) ) ==
                                self->expr_head )
                        status = EXPR_BADSYMBOL;

                else if ( !commit_node ( self, node ) )
                        status = EXPR_NOEXPR;

        self->idx = 0;
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

        if ( ( self = malloc ( sizeof ( struct expression ) ) ) ) {
                self->data = NULL;
                self->capacity = 1;
                self->idx = 0;
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
                free ( self->data );

        free ( self );
        debug_puts ( "Expression destructed" );
}

