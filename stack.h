/**
 * This interface exposes an abstract stack, used for storing generic pointers
 * in the canonical LIFO ADT form.
 *
 * @author Oliver Dixon
 */

#ifndef STACK_H
#define STACK_H

/**
 * The base opaque type of a stack
 */
struct stack;

/**
 * Initialise and return a stack, with the given initial capacity.
 *
 * @param capacity the initial capacity
 * @return the new stack
 */
struct stack * stack_initialise ( unsigned int capacity );

/**
 * Destroy a stack; its data is unchanged.
 *
 * @param self the stack
 */
void stack_destruct ( struct stack * self );

/**
 * Remove and return the top element from the given stack.
 *
 * @param self the stack
 * @return the topmost element, or NULL if the stack is empty
 */
void * stack_pop ( struct stack * self );

/**
 * Return the top element from the given stack.
 *
 * @param self the stack
 * @return the topmost element, or NULL if the stack is empty
 */
void * stack_peek ( struct stack * self );

/**
 * Push a given element to the given stack.
 *
 * @param self the stack
 * @param node the source node
 * @return the pushed node, or NULL if the stack is incapable of accommodating
 *      more data
 */
void * stack_push ( struct stack * self, void * node );

/**
 * Print the contents of the stack to the standard output
 *
 * @param self the stack
 * @param printer a function to parse all stack nodes into a fixed-size string
 *      buffer, given a node, buffer, and buffer capacity
 */
void stack_print ( struct stack * self, char * ( * printer ) ( void *, char *,
    unsigned int ) );

#endif /* STACK_H */

