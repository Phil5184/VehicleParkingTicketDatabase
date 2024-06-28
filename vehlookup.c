#include "Version.h"
#ifndef MYVEHLOOKUP
#warning("WARNING - USING THE SOLUTION vehlookup.c NOT MY CODE")
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
#include "vehlookup.h"
#include "subs.h"

/*
 * vehiclelookup
 *          look for vehicle in the database
 *          vehicle must match both plate and state strings
 *
 * Arguments:
 *
 *  plate:
 *          plate id string to be found
 *  state:
 *          state id string to be found
 *
 * return:
 *          pointer to vehicle if found
 *          NULL otherwise
 */

struct vehicle *
vehiclelookup(char *plate, char *state)
{
	uint32_t hashval; //declaring hashval to store hash value from plate string
	uint32_t index; //declaring index variable to store index of hash chain
	hashval = hash(plate); // hashing plate string
	index = hashval % tabsz; // getting index of htable
	struct vehicle *ptr; // ptr to point at hash chain to find vehicle
	ptr = *(htable + index);
	//searching through hash chain for vehicle
	while (ptr != NULL) {
		if (strcmp(plate, ptr->plate) == 0 && strcmp(state, ptr->state) == 0)
			return ptr;
		ptr = ptr->next;
	}
	return NULL;
}
#endif
