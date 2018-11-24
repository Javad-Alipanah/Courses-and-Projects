#include "dma.h"
#include "common_header.h"
dataset* find_ds(database* db, char* name) { /* Finds a specific dataset */
	
	dataset* curr_ds;
	int ds_hash = ds_hash_func(name, db->size);
	
	curr_ds = db->ds_hash[ds_hash];
	
	if(curr_ds == NULL)
		return NULL;
	
	while(strcmp(curr_ds->name , name)) {
		curr_ds = curr_ds->next;
		if(curr_ds == NULL)
			return NULL;
	
	}

	return curr_ds;

}