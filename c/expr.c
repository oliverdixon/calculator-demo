/**
 * Implement the expression interface; see 'expr.h'.
 *
 * @author Oliver Dixon
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "node.h"
#include "debug.h"
#include "stack.h"

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
         * Internal nodal representation of the infix expression
         */
        struct node ** data;

        /**
         * The (variable) number of nodes allowed by this expression
         */
        unsigned int capacity;

        /**
         * The current node-under-inspection in the nodal array.
         */
        unsigned int idx;

        /**
         * The postfix stack
         */
        struct stack * postfix;
};

/**
 * Parse the expression status into a human-readable string.
 *
 * @param status the expression status under examination
 * @return a human-readable representation of the expression status
 */
static const char * status_str ( enum expr_status status )
{
        switch ( status ) {
                case EXPR_OK:        return "Expression OK";
                case EXPR_NONODE:    return "Insufficient nodes";
                case EXPR_BADSYMBOL: return "Unexpected symbol";
                case EXPR_NOEXPR:    return "Insufficient expression capacity";
                case EXPR_INTERR:    return "Internal error; please report!";

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
 * Handle an incoming operator node during the execution of the Shunting Yard
 * algorithm, as according to the rules defined by the 'postfix' function.
 *
 * @param op_stack the operator stack
 * @param out_stack the output stack
 * @param node the incoming operator node
 */
static void sya_handle_op ( struct stack * op_stack, struct stack * out_stack,
                struct node * node )
{
        struct node * top;
        enum node_precedence prec;

        assert ( node_get_type ( node ) == NODE_OPERATOR );

        while ( ( top = stack_peek ( op_stack ) ) &&
                        node_get_type ( top ) != NODE_LPAREN &&
                        ( ( prec = node_test_prec ( top, node ) ) ==
                                NODE_PREC_GREATER ||
                                prec == NODE_PREC_LASSOC ) )

                stack_push ( out_stack, stack_pop ( op_stack ) );

        stack_push ( op_stack, node );
}

/**
 * Handle an incoming right parenthesis node during the execution of the
 * Shunting Yard algorithm, as according to the rules defined by the "postfix"
 * function.
 *
 * @param op_stack the operator stack
 * @param out_stack the output stack
 */
static void sya_handle_rparen ( struct stack * op_stack,
                struct stack * out_stack )
{
        struct node * top;

        while ( ( top = stack_peek ( op_stack ) ) &&
                        node_get_type ( top ) != NODE_LPAREN )
                stack_push ( out_stack, stack_pop ( op_stack ) );

        assert ( node_get_type ( top ) == NODE_LPAREN );
        stack_pop ( op_stack );
}

/* NOTES FOR THE POSTFIX CONVERTER
 *
 * Reverse-Polish notation) using the Shunting Yard algorithm (SYA). This
 * function implements a variant of the SYA by executing the following rules:
 *
 *  - If the next node is a literal: push it to the output stack.
 *
 *  - If the next node is an operator: while the top of the operator stack
 *    exists and is not a left parenthesis, and has greater-or-equivalent
 *    precedence as the incoming operator, pop from the operator stack onto the
 *    output stack. Then, push the incoming operator onto the operator stack.
 *
 *  - If the next node is a left parenthesis: push it to the operator stack.
 *
 *  - If the next node is a right parenthesis: pop the operator stack symbols
 *    onto the output stack until a left parenthesis is found. Then, discard
 *    both the left and right parentheses.
 *
 * TODO: Implement a proper error-handling interface, detecting mismatched
 *      parentheses or operands, etc. Remove as many assertions as reasonable.
 * TODO: Implement a stack-based evaluator for the resultant output stack.
 */

enum expr_status expression_postfix ( struct expression * self )
{
        struct stack * op_stack = stack_initialise ( 0 );
        struct stack * out_stack = self->postfix;
        struct node * node;

        if ( !op_stack ) {
                stack_destruct ( op_stack );
                return EXPR_NOEXPR;
        }

        for ( unsigned int i = 0; i < self->idx; i++ ) {
                node = self->data [ i ];
                switch ( node_get_type ( node ) ) {
                        case NODE_LITERAL:
                                stack_push ( out_stack, node );
                                break;

                        case NODE_OPERATOR:
                                sya_handle_op ( op_stack, out_stack, node );
                                break;

                        case NODE_LPAREN:
                                stack_push ( op_stack, node );
                                break;

                        case NODE_RPAREN:
                                sya_handle_rparen ( op_stack, out_stack );
                                break;

                        case NODE_UNKNOWN:
                        case NODE_COUNT:
                                return EXPR_INTERR;
                }
        }

        while ( ( node = stack_peek ( op_stack ) ) )
                stack_push ( out_stack, stack_pop ( op_stack ) );

        stack_destruct ( op_stack );
        debug_puts ( "Expression converted to RPN" );
        stack_print ( out_stack, node_format );
        return EXPR_OK;
}

struct expression * expression_initialise ( const char * expr,
                unsigned int capacity )
{
        struct expression * self;

        if ( ! ( self = malloc ( sizeof ( struct expression ) ) ) ||
                        ! ( self->postfix = stack_initialise ( capacity ) ) ) {
                free ( self );
                self = NULL;
        } else {
                self->data = NULL;
                self->capacity = 1;
                self->idx = 0;
                self->expr_head = expr;

                debug_puts ( "Expression initialised" );
        }

        return self;
}

enum expr_status expression_tokenise ( struct expression * self,
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

                /* Now the token is successfully parsed, we can attempt to
                 * commit the populated node to the expression storage array. */
                else if ( !commit_node ( self, node ) )
                        status = EXPR_NOEXPR;

        debug_puts ( ( status == EXPR_OK ) ? "Expression tokenised" :
                "Expression tokenised with faults" );

        return status;
}

void expression_destruct ( struct expression * self )
{
        if ( self ) {
                free ( self->data );
                stack_destruct ( self->postfix );
		free ( self );
		debug_puts ( "Expression destructed" );
        }
}

void expression_perror ( struct expression * self, const char * msg,
                enum expr_status status )
{
        if ( msg ) {
                fputs ( msg, stderr );
                fputs ( ": ", stderr );
        }

        fputs ( status_str ( status ), stderr );

        if ( status == EXPR_BADSYMBOL && *self->expr_head != '\0' ) {
                fputs ( " starting at \"", stderr );
                fputs ( self->expr_head, stderr );
                fputc ( '\"', stderr );
        }

        fputs ( ".\n", stderr );
}

