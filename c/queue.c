/**
 * Implements the queue ADT interface; see 'queue.h'.
 */

#include <stdlib.h>
#include <stdbool.h>

#include "debug.h"
#include "queue.h"

/**
 * Transparent linked list node definition
 */
struct queue_data {
        /**
         * The data encoded within the linked list node
         */
        // void * data;
        unsigned int data;

        /**
         * The next and previous nodes
         */
        struct queue_data * next, * prev;
};

/**
 * Transparent queue definition
 */
struct queue {
        /**
         * The variable capacity of the queue
         * TODO: This is currently fixed
         */
        unsigned int capacity;

        /**
         * The head of the linked list
         */
        struct queue_data * head;
};

/**
 * Checks if the given queue is empty
 *
 * @param self the queue
 * @return the state of the queue
 */
static inline bool is_empty ( struct queue * self )
{
        return !self->head->data;
}

/**
 * Creates and links a fixed number of nodes
 *
 * @param capacity the number of nodes to allocate and link
 * @return the head of the newly created nodes
 */
static struct queue_data * initialise_list ( unsigned int capacity )
{
        struct queue_data * ll_pool = NULL;
        capacity--;

        /* We allocate the LL node pool in a single malloc, and then iterate
         * through the array to link up the next and previous associations. For
         * obvious reasons, the first and final nodes need to be treated as
         * special cases. */

        if ( ( ll_pool = malloc ( sizeof ( struct queue_data ) *
                        ( capacity + 1 ) ) ) ) {

                ll_pool [ 0 ].data = 0;
                ll_pool [ 0 ].next = & ( ll_pool [ 1 ] );
                ll_pool [ 0 ].prev = NULL;

                for ( unsigned int i = 1; i < capacity; i++ ) {
                        ll_pool [ i ].data = i;
                        ll_pool [ i ].next = & ( ll_pool [ i + 1 ] );
                        ll_pool [ i ].prev = & ( ll_pool [ i - 1 ] );
                }

                ll_pool [ capacity ].data = capacity;
                ll_pool [ capacity ].next = NULL;
                ll_pool [ capacity ].prev = & ( ll_pool [ capacity - 1 ] );
        }

        return ll_pool;
}

struct queue * queue_initialise ( unsigned int capacity )
{
        const unsigned int DEFAULT_CAPACITY = 16;
        struct queue * self;

        if ( capacity < 2 )
                capacity = DEFAULT_CAPACITY;

        if ( ( self = malloc ( sizeof ( struct queue ) ) ) )

                if ( ! ( self->head = initialise_list ( capacity ) ) )
                        free ( self );
                else {
                        self->capacity = capacity;
                        debug_puts ( "Queue initialised" );
                }

        return self;
}

void queue_destruct ( struct queue * self )
{
        if ( self )
                free ( self->head );

        free ( self );
        debug_puts ( "Queue destructed" );
}

