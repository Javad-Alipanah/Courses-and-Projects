#include "dma.h"
#include "common_header.h"
int delete_set(char* name, database* db) {
	
	int i , flag = ZERO;
	linked_data* next_ld, *curr_ld;
	dataset* prev_ds, *curr_ds;
	attr_prime* curr_attr, *next_attr;
	
	curr_ds = db->ds_hash[ds_hash_func(name, db->size)];
	
	if(curr_ds == NULL)
		return FAILED;
	
	while(strcmp(curr_ds->name , name) &&  curr_ds->next != NULL) {
		flag = ONE;
		prev_ds = curr_ds;
		curr_ds = curr_ds->next;
	}
		
	if(curr_ds->next == NULL && strcmp(curr_ds->name , name))
		return SEMANTICALLYFAULT;
	
	if(flag) {
		prev_ds->next = curr_ds->next;
		curr_ld = curr_ds->ld;
	}
	
	else
		db->ds_hash[ds_hash_func(name, db->size)] = curr_ds->next;
	
	curr_ld = curr_ds->ld;
			
	do {
		
		if(curr_ld == NULL)
			break;
				
		next_ld = curr_ld->next;
				
		for(i = ZERO; i < MAX_NUM_OF_ATTRS; i++) {
					
			if(curr_ld->attributes[i].name == NULL)
				break;
					
			free(curr_ld->attributes[i].name);
		}
				
		free(curr_ld);
		curr_ld = next_ld;
	
	}while(next_ld != NULL);
		
	for(i = ZERO; i < MAX_NUM_OF_ATTRS; i++) {
		
		curr_attr = curr_ds->hash_attr[i];
		
		do {
			
			if(curr_attr == NULL)
				break;
			
			next_attr = curr_attr->next;
			free(curr_attr->attr.name);
			free(curr_attr);
			curr_attr = next_attr;
		
		}while(next_attr != NULL);
	}
			
	free(curr_ds->name);
	free(curr_ds);
	
	return SUCCEEDED;

}