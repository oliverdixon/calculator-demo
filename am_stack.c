/* Arithmetic Stack Implementation
 * OWD, 2023 */

#include <stdlib.h>
#include "am_stack.h"

/* Initialise the arithmetic stack with the given initial capacity, assuming
 * some sensible default if the passed value is zero. This function returns zero
 * on success, or -1 on failure, in which case errno is set. */
int am_stack_initialise ( struct am_stack * stack, size_t capacity )
{
        static const size_t DEFAULT_CAPACITY = 16;

        if ( ! ( stack->data = malloc ( sizeof ( struct am_stack_node ) *
                        ( capacity ) ? capacity : DEFAULT_CAPACITY ) ) )
                return -1;

        stack->size = 0;
        stack->capacity = capacity;

        return 0;
}

/* Destroy the arithmetic stack by freeing all nodes and resetting metadata. */
void am_stack_destroy ( struct am_stack * stack )
{
        free ( stack->data );

        stack->size = 0;
        stack->capacity = 0;
}

