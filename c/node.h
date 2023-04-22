/**
 * This interface handles collections, or "pools", of nodes. These exist to
 * minimise the number of calls to the dynamic allocator, and to maintain the
 * results of such allocations in a number of compartmentalised pools that are
 * easy to free in bulk. Callers should:
 *
 *  - Create a new pool;
 *  - Pluck nodes from that pool as required;
 *  - Freely interact with these nodes with the node API;
 *  - If they cannot receive a new node, request a new pool;
 *  - Once finished, destruct all allocated pools.
 *
 * This file also describes the node API, through which callers may encode and
 * decode nodes during the parsing of an arithmetic expression.
 *
 * @author Oliver Dixon
 */

#ifndef NODE_H
#define NODE_H

/**
 * The base opaque type of an individual node
 */
struct node;

/**
 * The base opaque type for a pool
 */
struct node_pool;

/**
 * An alias for the literal (number) type. This must be type-compatible with
 * double.
 */
typedef float number_t;

/**
 * The type of a node, indicating the type of data encoded within
 */
enum node_type {
        NODE_UNKNOWN,
        NODE_OPERATOR,
        NODE_LITERAL,
        NODE_LPAREN,
        NODE_RPAREN,

        NODE_COUNT
};

/**
 * The type of operator encoded by a node, where applicable
 */
enum node_operator {
        NODE_OP_UNKNOWN,
        NODE_OP_EXP,
        NODE_OP_DIVIDE,
        NODE_OP_MULTIPLY,
        NODE_OP_ADD,
        NODE_OP_SUBTRACT,

        NODE_OP_COUNT
};

/**
 * The result of a comparison between the precedence of two operator nodes
 */
enum node_precedence {
        NODE_PREC_GREATER, /* The first operator has greater precedence */
        NODE_PREC_LESSER,  /* The first operator has lesser precedence  */
        NODE_PREC_SAME,    /* The operators have the same precedence    */
        NODE_PREC_LASSOC,  /* As above, but the first operator is left-
                              associative.                              */
};

/**
 * Initialise a new node pool of a given fixed capacity.
 *
 * @param capacity the fixed capacity of the node pool. If this is zero, a
 *        sensible default is assumed.
 * @return the address of the new pool
 */
struct node_pool * pool_initialise ( unsigned int capacity );

/**
 * Destruct an entire node pool, including its data. 
 *
 * @param self the node pool to be destroyed
 */
void pool_destruct ( struct node_pool * self );

/**
 * Grab a new node from the pool and return its address.
 *
 * @param self the target pool
 * @return the address of the node. If no node is available, NULL is returned.
 */
struct node * pool_new_node ( struct node_pool * self );

/**
 * Format a node into a human-readable form, and place the results in the given
 * buffer.
 *
 * @param self the node
 * @param buffer the destination buffer
 * @param size the capacity of the given buffer
 * @return the populated buffer, or NULL if the given buffer was unreasonably
 *        small
 */
char * node_format ( void * self, char * buffer, unsigned int size );

/**
 * Encode a string, up until a natural delimiter, into a node, while setting the
 * metadata accordingly.
 *
 * @param self the node
 * @param str the string to be parsed
 * @return the destination of the new read head
 */
const char * node_encode ( struct node * self, const char * str );

/**
 * Grab the next available node from the provided pools.
 *
 * @param self the list of available node pools
 * @param pool_idx the current position in the node pool list
 * @param pool_count the number of available given pools
 * @return the requested node, or NULL if no such node is available
 */
struct node * pool_pull_node ( struct node_pool ** self,
        unsigned int * pool_idx, unsigned int pool_count );

/**
 * Retrieves the type of the given node
 *
 * @param self the node
 * @return the type of the node
 */
enum node_type node_get_type ( struct node * self );

/**
 * Retrieves the type of the given operator node
 *
 * @param self the node containing an operator
 * @return the nature of the operator
 */
enum node_operator node_op_get_type ( struct node * self );

/**
 * Determines the precedence relationship between two given operator types
 *
 * @param op1 the first operator
 * @param op2 the second operator
 * @return the precedence of the first operator in relation to the precedence of
 *      the second one
 */
enum node_precedence node_test_precedence ( enum node_operator op1,
        enum node_operator op2 );

#endif /* NODE_H */

