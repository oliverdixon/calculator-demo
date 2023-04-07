/**
 * Implement the node and node pool interface; see node.h
 *
 * @author Oliver Dixon
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <errno.h>

#include "node.h"

/**
 * The transparent node
 */
struct node {
        /**
         * The internal type of the node: use this to correctly decode the node.
         */
        enum node_type type;
        
        /**
         * The data encoded by the node.
         */
        union {
                enum node_operator op;
                number_t value;
        };
};

/**
 * The transparent node pool
 */
struct node_pool {
        /**
         * The fixed capacity of the pool
         */
        unsigned int capacity;

        /**
         * The current occupation of the pool
         */
        unsigned int used;

        /**
         * The node array
         */
        struct node * data;
};

/**
 * Is the pool full?
 *
 * @param self the status of the pool
 */
static inline bool is_full ( struct node_pool * self )
{
        return self->used == self->capacity;
}

/**
 * Write a constant source string to a destination string, performing basic
 * length checks as necessary. This is a lightweight alternative to snprintf(3)
 * for the node formatters.
 *
 * @param dest the destination string
 * @param src the source string
 * @param size the capacity of the destination string
 * @return the number of characters written to the destination string
 */
static unsigned int simple_writeout ( char * dest, const char * src,
                unsigned int size )
{
        const unsigned int srclen = ( unsigned int ) strlen ( src );
        assert ( size > 0 );

        if ( srclen >= size ) {
                size--;
                for ( unsigned int i = 0; i < size; i++ )
                        /* The strings are short, and since we already know the
                         * source and destination string lengths, this linear
                         * approach is desirable to strncpy(3), which invariably
                         * recalculates the source length. We cannot necessarily
                         * just end the string early by replacing a character
                         * with a NULL-terminator, since the source string may
                         * be a string literal, existing in read-only memory. */

                        dest [ i ] = src [ i ];
        } else
                strcpy ( dest, src );

        return srclen;
}

/**
 * Format a node literal to the given buffer.
 *
 * @param self the node
 * @param buffer the destination string buffer
 * @param size the capacity of the destination
 */
static inline unsigned int formatter_literal ( struct node * self,
                char * buffer, unsigned int size )
{
        int retval = snprintf ( buffer, size, "Literal: %.3f", ( double )
                self->value );

        return ( retval < 0 ) ? 0 : ( unsigned int ) retval;
}

/**
 * Format a node of unknown type to the given buffer.
 *
 * @param self the node
 * @param buffer the destination string buffer
 * @param size the capacity of the destination
 */
static inline unsigned int formatter_not_implemented ( struct node * self,
                char * buffer, unsigned int size )
{
        ( void ) self;
        return simple_writeout ( buffer, "Not Implemented", size );
}

/**
 * Format a node operator to the given buffer.
 *
 * @param self the node
 * @param buffer the destination string buffer
 * @param size the capacity of the destination
 */
static unsigned int formatter_operator ( struct node * self, char * buffer,
                unsigned int size )
{
        static char * opstr [ ] = { "Unknown", "Power", "Divide",
                "Multiply", "Add", "Subtract" };
        unsigned int used;

        assert ( sizeof ( opstr ) / sizeof ( *opstr ) == NODE_OP_COUNT );
        used = simple_writeout ( buffer, "Operator: ", size );

        return used + simple_writeout ( & ( buffer [ used ] ),
                ( self->op >= 1 && self->op < NODE_OP_COUNT ) ?
                        opstr [ self->op ] : opstr [ 0 ], size - used );
}

/**
 * Format a node parenthesis to the given buffer.
 *
 * @param self the node
 * @param buffer the destination string buffer
 * @param size the capacity of the destination
 */
static inline unsigned int formatter_paren ( struct node * self, char * buffer,
                unsigned int size )
{
        return simple_writeout ( buffer, ( self->type == NODE_LPAREN ) ?
                "Left Parenthesis" : "Right Parenthesis", size );
}

struct node_pool * pool_initialise ( unsigned int capacity )
{
        static const unsigned int DEFAULT_CAPACITY = 16;
        struct node_pool * self;

        if ( !capacity )
                capacity = DEFAULT_CAPACITY;

        if ( ( self = malloc ( sizeof ( struct node_pool ) ) ) )
                if ( ! ( self->data = malloc ( sizeof ( struct node ) *
                                capacity ) ) )
                        free ( self );
                else {
                        self->capacity = capacity;
                        self->used = 0;
                }

        return self;
}

void pool_destruct ( struct node_pool * self )
{
        if ( self )
                free ( self->data );

        free ( self );
}

struct node * pool_new_node ( struct node_pool * self )
{
        struct node * node = NULL;

        if ( !is_full ( self ) )
                node = & ( self->data [ self->used++ ] );

        return node;
}

char * node_format ( void * self, char * buffer, unsigned int size )
{
        const unsigned int MINIMUM_LENGTH = 16;
        static unsigned int ( * const formatter [ NODE_COUNT ] )
                ( struct node *, char *, unsigned int )
                        = { formatter_not_implemented, /* Unknown         */
                            formatter_operator,        /* Operator        */
                            formatter_literal,         /* Literal         */
                            formatter_paren,           /* (L) Parenthesis */
                            formatter_paren,           /* (R) Parenthesis */
                    };

        if ( size < MINIMUM_LENGTH ) {
                errno = EINVAL;
                return NULL;
        }

        assert ( sizeof ( formatter ) / sizeof ( *formatter ) == NODE_COUNT );

        /* The formatters will always add a NULL-terminator; if it had intended
         * to overwrite the available string space, then the output will be
         * truncated, so we add a three-character marker to indicate as such. */
        if ( formatter [ ( ( struct node * ) self )->type ]
                        ( self, buffer, size ) >= size ) {
                buffer [ size - 2 ] = '.';
                buffer [ size - 3 ] = '.';
                buffer [ size - 4 ] = '.';
        }

        return buffer;
}

void node_encode_literal ( struct node * self, number_t value )
{
        self->type = NODE_LITERAL;
        self->value = value;
}
