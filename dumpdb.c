#include "Version.h"
#ifndef MYDUMPDB
#warning("WARNING - USING THE SOLUTION dumpdb.c NOT MY CODE")
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
#include "subs.h"
#include "dumpdb.h"

/*
 * dumpchain
 *
 * dump the linked list chain on a single hash chain
 * print each vehicle on the chain using printvehicle()
 *
 * Arguments:
 *
 * index:
 *          hash table chain index number
 * cnt:
 *          a pointer to an output variable that contains the count
 *          of the total number of tickets on the hash chain
 *          set *cnt = 0UL if no tickets are found
 *
 * return:
 *          number of vehicles found on the hash chain
 *          total number of tickets in chain returned in output variable *cnt
 */
unsigned long
dumpchain(uint32_t index, unsigned long *cnt)
{
    //printf("Chain %u: \n", index);
  	 
    // use printvehicle() to print each vehicle on the hash chain
	struct vehicle *vehptr = *(htable + index); //pointer to run through each vehicle in the chain
	if (vehptr != NULL) {
		printf("Chain %u: \n", index);
	}
	*cnt = 0UL; //initializing cnt to 0
	unsigned long count = 0UL; //variable to keep track of how many vehicles are in the hash chain
	while (vehptr != NULL) {
		(*cnt) += printvehicle(vehptr);
		count += 1;
		vehptr = vehptr->next;
	}
	return count;
}

/*
 * dumpdb
 *
 * dump the entire database one hash chain at a time from chain 0
 * in the hash table to the last entry in the table
 *
 * call dumpchain() to print each hash chain.
 *
 * Prints the total number of vehicles and tickets in the database
 *
 */
void
dumpdb(void)
{
    unsigned long vcnt = 0UL; // total vehicle count
    unsigned long tcnt = 0UL; // total ticket count

    uint32_t hashIdx = 0; // index variable to index through htable
	unsigned long tickets; // variable to use as argument to dumpchain, adding to tcnt after each dumpchain call
	while (hashIdx < tabsz) {
		vcnt += dumpchain(hashIdx, &tickets);
		tcnt += tickets;
		hashIdx++;
	}
	
 
    // print the summaries
    printf("Total vehicles in database: %lu\n", vcnt);
    printf("Total tickets in database: %lu\n", tcnt);
    return;
}
#endif
