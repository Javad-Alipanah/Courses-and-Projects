#include "dma.h"
#include "common_header.h"
attr_prime* find_attr_prime(int i, dataset* curr_ds, query* Query) { /* Finds a specific attr_prime (a structure) */
	
	attr_prime* curr_attr;
	int attr_hash = hash_attr_func(Query->expression[i].name);
	
	if(curr_ds->hash_attr[attr_hash] == NULL) {
		return NULL;
	}
	
	curr_attr = curr_ds->hash_attr[attr_hash];
	
	while(strcmp(curr_attr->attr.name , Query->expression[i].name)) {
		curr_attr = curr_attr->next;
		
		if(curr_attr == NULL) {
			return NULL;
		}
	
	}
	
	return curr_attr;

}