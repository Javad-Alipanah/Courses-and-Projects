#include "dma.h"
#include "common_header.h"
int delete_database(database *db) {

	int i , flag = ZERO;
	linked_data* next_ld, *curr_ld;
	dataset* next_ds, *curr_ds;
	attr_prime* curr_attr, *next_attr;

	for(i = ZERO; i < db->size; i++)
		
		if(db->ds_hash[i] != NULL) {
			flag = ONE;
			curr_ld = db->ds_hash[i]->ld;
			curr_ds = db->ds_hash[i];
			
			do {
				
				do {
					
					if(curr_ld == NULL)
						break;
					
					next_ld = curr_ld->next;
					free(curr_ld);
					curr_ld = next_ld;
				
				}while(next_ld != NULL);
				
				next_ds = curr_ds->next;
				
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
				curr_ds = next_ds;
			
			}while(next_ds != NULL);
		
		}
		
		free(db->ds_hash);
		free(db->name);
		free(db);
		
		return SUCCEEDED;

}