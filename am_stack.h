/* Arithmetic Stack API
 * OWD, 2023 */

#ifndef _AM_STACK_H
#define _AM_STACK_H

#include <stddef.h>
#include "am_node.h"

/* The arithmetic stack superstructure */
struct am_stack;

struct am_stack * am_stack_initialise ( size_t );
void am_stack_destroy ( struct am_stack * );
struct am_node * am_stack_peek ( struct am_stack * );
struct am_node * am_stack_pop ( struct am_stack * );
int am_stack_push ( struct am_stack *, struct am_node * );

#endif /* _AM_STACK_H */

