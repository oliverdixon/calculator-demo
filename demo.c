/* Calculator Demonstration
 * OWD, 2023 */

#include <stdio.h>
#include <stdlib.h>

#include "am_stack.h"

int main ( int argc, char ** argv )
{
        struct am_stack stack;

        if ( am_stack_initialise ( &stack, 0 ) == -1 ) {
                perror ( "am_stack_initialise" );
                return EXIT_FAILURE;
        }

        am_stack_destroy ( &stack );
        puts ( "OK" );

        return EXIT_SUCCESS;
}

