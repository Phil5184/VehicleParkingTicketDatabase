#include "Version.h"
#ifndef MYINSTICKET
#warning("WARNING - USING THE SOLUTION insticket.c NOT MY CODE")
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
#include "insticket.h"
#include "subs.h"
/*
 * insertticket
 *      add a ticket (summons) to the database
 *
 *      YOU CANNOT USE CALLOC(), only malloc() is allowed.
 *
 *      if the vehicle (plate state) is not in the database then
 *      a new struct vehicle entry must be made for it in the hash table.
 *      New vehicle entries are made at the the FRONT of the collison chain
 *
 *      If the vehicle is already in the hash chain, you only need to add 
 *      a new ticket to the ticket chain for this summons.
 *
 *      Tickets are added to the ticket chain linked to the struct vehicle
 *      at the END of the ticket chain.
 *
 *      Also you must update the tot_fine and cnt_tickets in the struct vehicle
 *      to account for the new summons(ticket) just added
 *
 * Arguments
 *
 *  summ:
 *      summons id string to be added
 *      this string needs to be converted to a number to be stored in the
 *      database. This saves space and is faster to compare than a string         
 *      Conversion function is supplied. use example:
 *           unsigned long summid;
 *           if (strtosumid(summ, &summid, argv) != 0)
 *                  error handling
 *           new_ticket->summons = summid;
 *  plate:
 *      plate id string to be added
 *  state:
 *      state id string to be added
 *  code:
 *      summons code integer value used an an index into the fines table
 *      fineTab maps code number to text description & fine cost 
 *      you use this table to get the amount of the fine when calculating
 *      the tot_fine field in struct vehicle.
 *          example:
 *              struct vehicle *vhpt;
 *              vhpt->tot_fine += fineTab[code].fine;
 *
 * return
 *       0 if ok -1 for all errors
 */
int
insertticket(char *summ, char *plate, char *state, int code)
{
	uint32_t hashval; //declaring hashval to store hash value from plate string
	uint32_t index; //declaring index variable to store index of hash chain
	hashval = hash(plate); // hashing plate string
	index = hashval % tabsz; // getting index of htable
	struct vehicle *ptr; // ptr to point at vehicle if it is found
	ptr = *(htable + index);
	//searching through hash chain for vehicle
	while (ptr != NULL) {
		if (strcmp(plate, ptr->plate) == 0 && strcmp(state, ptr->state) == 0)
			break;
		ptr = ptr->next;
	}
	unsigned long summonID; //variable to store converted summons string
	unsigned int fine = fineTab[code].fine; //variable to store fine of the ticket being inserted;
	if (strtosumid(summ, &summonID) != 0) {
		return -1;
	}
	struct ticket *newticket = malloc(sizeof(*newticket));
		if (newticket == NULL) {  //malloc failed
			return -1;
		}
		else {  //malloc succeeded so initialize all fields of newticket
			newticket->summons = summonID;
			newticket->code = code;
			newticket->next = NULL;
		}
	if (ptr == NULL) {   //vehicle was not found if ptr == NULL
		struct vehicle *newcar = malloc(sizeof(*newcar));
		if (newcar != NULL) {  //malloc passed, need to do strdup and check each one now
			if ((newcar->plate = strdup(plate)) == NULL) {
				free(newticket);
				free(newcar);
				return -1;
			}
			if ((newcar->state = strdup(state)) == NULL) {
				free(newticket);
				free(newcar->plate);
				free(newcar);
				return -1;
			}
			newcar->tot_fine = fine;
			newcar->cnt_ticket = 1;
			newcar->next = *(htable + index);
			newcar->head = newticket;
			*(htable + index) = newcar;
			return 0;
		}
		else {  //malloc failed
			return -1;
		}
	}
	else {   //vehicle was found
		struct ticket *dupcheck; //pointer to check if there is a duplicate
		dupcheck = ptr->head;
		struct ticket *prev = NULL; //pointer to keep track of previous node in case of inserting ticket
		while (dupcheck != NULL) {
			if (dupcheck->summons == summonID) {
				fprintf(stderr,"%s: duplicate summons %lu\n", argv0, summonID);
				return -1;
			}
			prev = dupcheck;
			dupcheck = dupcheck->next;
		}
		if (prev == NULL) {  //if previous is null then there are no tickets for this vehicle yet
			ptr->head = newticket;
			ptr->tot_fine += fine;
			ptr->cnt_ticket += 1;
			return 0;
		}
		prev->next = newticket;
		ptr->tot_fine += fine;
		ptr->cnt_ticket += 1;
		return 0;
	}
    return 0;
}

#endif
