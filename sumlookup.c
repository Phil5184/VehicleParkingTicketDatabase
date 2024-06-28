#include "Version.h"
#ifndef MYSUMLOOKUP
#warning("WARNING - USING THE SOLUTION sumlookup.c NOT MY CODE")
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
#include "sumlookup.h"
#include "subs.h"

/*
 * summlookup
 *     find the vehicle with a specified summons number
 *
 * Arguments:
 *
 * summ:
 *       string of summons number
 *       make sure to convert summ string to a summons number
 *       use strsumid() - see subs.c
 *
 * returns:
 *       a pointer to the vehicle if found
 *       NULL if not found
 */
struct vehicle *
sumlookup(char *summ)
{
	struct vehicle *vehicleptr; //pointer to point at each vehicle in the database
	struct ticket *ticptr; //pointer to point at each ticket
    unsigned long summID; //variable to store the converted summons string argument in
	if (strtosumid(summ, &summID) != 0) {
		return NULL;
	}
	uint32_t i = 0; // variable to index through hash table

	/*loop to iterate through entire database*/
	while (i < tabsz) {
		vehicleptr = *(htable + i);
		while (vehicleptr != NULL) {
			ticptr = vehicleptr->head;
			while (ticptr != NULL) {
				if (ticptr->summons == summID) {
					return vehicleptr;
				}
				ticptr = ticptr->next;
			}
			vehicleptr = vehicleptr->next;
		}
		i++;
	}
	return NULL;
}
#endif
