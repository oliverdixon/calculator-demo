/* Arithmetic Stack Implementation
 * OWD, 2023 */

#include <stdlib.h>
#include "am_stack.h"

/* Complete definition for the arithmetic stack */
struct am_stack {
        struct am_node ** data;
        size_t capacity, size;
};

/* Initialise the arithmetic stack with the given initial capacity, assuming
 * some sensible default if the passed value is zero. This function returns the
 * address of the new stack on success, or NULL on failure, in which case errno
 * is set. The pointers (i.e., stack nodes) are initialised to NULL. */
struct am_stack * am_stack_initialise ( size_t cap )
{
        struct am_stack * stack;

        if ( !cap )
                cap = DEFAULT_NODE_COUNT;

        if ( ! ( stack = malloc ( sizeof ( struct am_stack ) ) ) ||
                        ! ( stack->data = calloc ( cap, sizeof ( struct
                                am_node * ) ) ) ) {
                /* If the data malloc failed, the superstructure may still be
                 * hanging around in memory. */
                free ( stack );
                return NULL;
        }

        stack->size = 0;
        stack->capacity = cap;

        return stack;
}

/* Destroy the arithmetic stack by deep-freeing the entire structure. */
void am_stack_destroy ( struct am_stack * stack )
{
        if ( stack )
                free ( stack->data );

        free ( stack );
}

/* Peek the node atop the given arithmetic stack and return its address. If the
 * stack is empty, NULL is returned. */
struct am_node * am_stack_peek ( struct am_stack * stack )
{
        return ( stack->size ) ? stack->data [ stack->size - 1 ] : NULL;
}

/* Pop the topmost node of the given arithmetic stack, returning the popped node
 * on success, and NULL on failure (i.e., the stack is empty). */
struct am_node * am_stack_pop ( struct am_stack * stack )
{
        struct am_node * node;

        if ( ( node = am_stack_peek ( stack ) ) )
                /* We do not explicitly remove the node pointer in the original
                 * stack, but merely make it inaccessible by am_stack_*
                 * functions. This means popped nodes will still be deleted when
                 * calling the stack destructor: this is desirable behaviour for
                 * our use-case. */

                stack->size--;

        return node;
}

/* Push the given node to the given arithmetic stack. If this is successful, the
 * address of the newly pushed node is returned for convenience; otherwise, NULL
 * is returned. If this function fails (i.e., the stack cannot be resized to
 * accommodate the incoming node), the node is not pushed, but the existing
 * stack remains intact and usable. */
struct am_node * am_stack_push ( struct am_stack * stack,
                struct am_node * node )
{
        if ( stack->size >= stack->capacity ) {
                /* If we are at capacity, we try to double the pointer data. */
                struct am_node ** new_stack_data;

                if ( ! ( new_stack_data = realloc ( stack->data, sizeof ( struct
                                am_node * ) * ( stack->capacity << 1 ) ) ) )
                        return NULL;

                stack->data = new_stack_data;
                stack->capacity <<= 1;
        }

        stack->data [ stack->size ] = node;
        stack->size++;

        return node;
}

