#pragma once
/* 
 * indexio.h --- 
 * 
 * Author: 
 * Created:
 * Version: 1.0
 * 
 * Description: 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <webpage.h>
#include <sys/stat.h>
#include "hash.h"


/*
 * indexsave -- saves index (a hashtable) into indexnm.
 * saves index to file, each line contains:
 * <word> <docID1> <count1> <docID2> <count2> ....<docIDN> <countN>
 * 
 * returns: 0 for success; nonzero otherwise
 *

 */
int32_t indexsave(hashtable_t *index, int id, char *dirnm);

/* 
 * indexload -- loads the numbered filename <id> in direcory <dirnm>
 * into a new hashtable.
 *
 * returns: non-NULL for success; NULL otherwise
 */

hashtable_t *indexload(int id, char *dirnm);
