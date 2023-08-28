/**
 * Implement the stack interface; see 'stack.h'.
 *
 * @author Oliver Dixon
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "debug.h"
#include "stack.h"

/**
 * The transparent stack
 */
struct stack {
        /**
         * The variable capacity of the stack
         */
        unsigned int capacity;

        /**
         * The current usage of the stack (number of elements)
         */
        unsigned int size;

        /**
         * The stack contents
         */
        void ** data;
};

/**
 * Is the stack empty?
 *
 * @param self the stack
 * @return the status of the stack
 */
static inline bool is_empty ( struct stack * self )
{
        return !self->size;
}

/**
 * Determine whether the given stack capacity must be increased to accommodate a
 * new (unseen) node. If the current capacity is insufficient, its size is
 * doubled; otherwise, nothing is done.
 *
 * @param self the stack
 * @return the ability to add a new node
 */
static bool realloc_check ( struct stack * self )
{
        void ** new_data;

        if ( self->size + 1 >= self->capacity ) {
                if ( ! ( new_data = realloc ( self->data, sizeof ( void * ) *
                                ( self->capacity << 1 ) ) ) )
                        return false;

                self->data = new_data;
                self->capacity <<= 1;
        }

        return true;
}

struct stack * stack_initialise ( unsigned int capacity )
{
        const unsigned int DEFAULT_CAPACITY = 16;
        struct stack * self;

        if ( !capacity )
                capacity = DEFAULT_CAPACITY;

        if ( ( self = malloc ( sizeof ( struct stack ) ) ) )
                if ( ! ( self->data = malloc ( sizeof ( void * ) *
                                capacity ) ) ) {
                        free ( self );
                        self = NULL;
                } else {
                        self->capacity = capacity;
                        self->size = 0;
                        debug_puts ( "Stack initialised" );
                }

        return self;
}

void stack_destruct ( struct stack * self )
{
        if ( self ) {
                free ( self->data );
                free ( self );
                debug_puts ( "Stack destructed" );
        }
}

void * stack_pop ( struct stack * self )
{
        return ( is_empty ( self ) ) ? NULL : self->data [ --self->size ];
}

void * stack_peek ( struct stack * self )
{
        return ( is_empty ( self ) ) ? NULL : self->data [ self->size - 1 ];
}

void * stack_push ( struct stack * self, void * node )
{
        if ( !realloc_check ( self ) )
                return NULL;

        self->data [ self->size++ ] = node;
        return node;
}

void stack_print ( struct stack * self, char * ( * printer ) ( void *,
                char *, unsigned int ) )
{
        const unsigned int BUFFER_SIZE = 20;
        char buffer [ BUFFER_SIZE ];

        printf ( "Stack Capacity: %d\nStack Size: %d\n", self->capacity,
                self->size );

        if ( is_empty ( self ) )
                puts ( "The stack is empty!" );
        else {
                puts ( "Stack Contents: ...\n" );
                for ( unsigned int i = self->size; i != 0; i-- )
                        printf ( "\t%d\t%s\n", i - 1,
                                ( printer ( self->data [ i - 1 ], buffer,
                                        BUFFER_SIZE ) )
                                ? buffer : "Formatting Error" );
        }
}

