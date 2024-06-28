#include "Version.h"
#ifndef MYFREETICKETS
#warning("WARNING - USING THE SOLUTION freetickets.c NOT MY CODE")
#else
#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "parking.h"
#include "freetickets.h"

/*
 * freetickets
 *      tears down the database freeing all allocated memory
 *      count the number of tickets and vehicles freed in
 *      the database 
 *      prints the counts before returning
 */
void
freetickets(void)
{
    unsigned long tcnt = 0UL;
    unsigned long vcnt = 0UL;
	struct vehicle *vptr;  // pointer variable to point at each variable in the database
	struct ticket *tptr;  // pointer variable to point at each ticket
	uint32_t n = 0;  //variable to index through htable
	
	/* loop to iterate through entire database*/
	while (n < tabsz) {
		vptr = *(htable + n);
		while (vptr != NULL) {
			tptr = vptr->head;
			while (tptr != NULL) {
				struct ticket *temp = tptr->next;  //temp variable to save tptr->next before freeing tptr
				free(tptr);
				tcnt++;
				tptr = temp;
			}
			struct vehicle *temp2 = vptr->next;  //temp variable to save vptr->next before freeing vptr
			free(vptr->state);
			free(vptr->plate);
			free(vptr);
			vcnt++;
			vptr = temp2;
		}
		n++;
	}	 
    printf("Total vehicles freed: %lu\n", vcnt);
    printf("Total tickets freed: %lu\n", tcnt);
    return;
}
#endif
