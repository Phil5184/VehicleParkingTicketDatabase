#include "Version.h"
#ifndef MYLARGEST
#warning("WARNING - USING THE SOLUTION largest.c NOT MY CODE")
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
#include "largest.h"

/*
 * largest
 *     find the vehicle with the largest number of
 *     tickets and largest total fines in database
 *     prinf("Empty database\n"); if the database has no tickets
 */
void
largest(void)
{
    struct vehicle *count = NULL;  // vehicle with greatest number of tickets
    struct vehicle *fine = NULL;  // vehicle with largest total fine
    unsigned int large_fine = 0U; // inital value for largest total fine seen
    unsigned int large_ticket = 0U; // initial value for most tickets seen
	struct vehicle *badcar; // pointer variable to point at each vehicle in the database
	uint32_t idx = 0; // variable to index through htable
	int checker = 0; // variable to act as a boolean flag to check if database is empty or not
	/*loop to iterate through each hash chain*/
	while (idx < tabsz) {
		badcar = *(htable + idx);
		while (badcar != NULL) {
			checker = 1;
			if (badcar->tot_fine >= large_fine) { 
				fine = badcar;
				large_fine = fine->tot_fine;
			}
			if (badcar->cnt_ticket >= large_ticket) {
				count = badcar;
				large_ticket = count->cnt_ticket;
			}
			badcar = badcar->next;
		}
		idx++;
	}
	if (checker == 0) {
   		printf("Empty database\n");
		return;
	}
    printf("Most tickets Plate: %s, State: %s tickets: %u, total fine: $%u\n",
            count->plate, count->state, count->cnt_ticket, count->tot_fine);
    printf("Largest fine Plate: %s, State: %s tickets: %u, total fine: $%u\n",
            fine->plate, fine->state, fine->cnt_ticket, fine->tot_fine);
    return;
}
#endif
