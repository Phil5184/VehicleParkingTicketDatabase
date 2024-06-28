#include "Version.h"
#ifndef MYDELTICKET
#warning("WARNING - USING THE SOLUTION delticket.c NOT MY CODE")
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
#include "delticket.h"
#include "subs.h"

/*
 * delticket
 *      remove ticket (summons) from the database
 *      look up the summons for a vehicle and pay it (remove the ticket) 
 *
 *      Find the vehicle by the state and plate. if not found return -1
 *
 *      convert the summ string to a unsigned int using using strtosumid.
 *
 *      After you have the vehicle, find the summons on the ticket chain
 *      that matches the converted summons string 
 *           
 *      If found, delete it from the ticket chain. If the vehicle
 *      no longer has any tickets, then remove the vehicle from the hash chain
 *      Otherwise you must update the tot_fine and cnt_tickets fields in
 *      the struct vehicle so the are accurate after removing the summons
 *      Make sure to free() all space that was malloc()'d 
 *
 * Arguments:
 *  plate   plate id string to be found
 *  state   state id string to be found
 *  summ    summon id string to be found
 *
 * returns 0 if ok -1 if not found or error
 */

int
delticket(char *plate, char *state, char *summ)
{
	struct vehicle *car;
	struct vehicle *prevcar = NULL; //pointer to keep track of previous car
	unsigned long summonsID; //variable to store converted summons string
	uint32_t hashval; //declaring hashval to store hash value from plate string
	uint32_t index; //declaring index variable to store index of hash chain
	hashval = hash(plate); // hashing plate string
	index = hashval % tabsz; // getting index of htable
	car = *(htable + index);
	//searching through hash chain for vehicle
	while (car != NULL) {
		if (strcmp(plate, car->plate) == 0 && strcmp(state, car->state) == 0)
			break;
		prevcar = car;
		car = car->next;
	}
	if (car == NULL) //car not found
		return -1;
	if (strtosumid(summ, &summonsID) != 0)
		return -1;

	struct ticket *paidticket = car->head; //pointer to search through ticket linked list
	struct ticket *prevtick = NULL; //pointer to keep track of previous node
	while (paidticket != NULL) {
		if (paidticket->summons == summonsID) {
			break;
		}
		prevtick = paidticket;
		paidticket = paidticket->next;
	}
	if (paidticket == NULL)  //ticket not found
		return -1;
	unsigned int fine = fineTab[paidticket->code].fine;
	if (paidticket == car->head)  //ticket is the first ticket in the list
		car->head = paidticket->next;
	else   //ticket was found and is not the head
		prevtick->next = paidticket->next;
	car->cnt_ticket -= 1;
	car->tot_fine -= fine;
	free(paidticket);

	if (car->cnt_ticket == 0 && car->tot_fine == 0) {
		if (prevcar == NULL) //car found was the first one in the chain
			*(htable + index) = car->next;
		else 
			prevcar->next = car->next;
		free(car->state);
		free(car->plate);
		free(car);
	}

    return 0;
}
#endif
