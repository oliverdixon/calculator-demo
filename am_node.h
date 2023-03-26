/* Arithmetic Node API
 * OWD, 2023 */

#ifndef _AM_NODE_H
#define _AM_NODE_H

#include <stddef.h>

#define DEFAULT_NODE_COUNT ( 16 )

/* Individual stack nodes for tokenising arithmetic expressions */
struct am_node;

struct am_node * am_node_create_pool ( size_t );
void am_node_destroy_pool ( struct am_node * );

#endif /* _AM_NODE_H */

