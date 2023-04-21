/**
 * This interface exposes an abstract queue (FIFO) type, and some associated
 * functions.
 *
 * @author Oliver Dixon
 */

#ifndef QUEUE_H
#define QUEUE_H

struct queue;

struct queue * queue_initialise ( unsigned int capacity );
void queue_destruct ( struct queue * self );

#endif /* QUEUE_H */

