#include "dma.h"
#include "common_header.h"
int delete_data(database* db, query* Query, char* name) {
	
	dataset* ds = find_ds(db, name);
	linked_data** ld;
	linked_data* curr_ld = ds->ld, *prev_ld;
	int size = select_for_deleting(db, name, &ld, Query), i = ZERO, j;
	
	while(i < size) {
		
		if(curr_ld == ld[i] && curr_ld == ds->ld) {
			
			for(j = ZERO; ld[i]->attributes[j].name != NULL; j++)
				free(curr_ld->attributes[j].name);
			
			ds->ld = curr_ld->next;
			free(curr_ld);
			curr_ld = ds->ld;
			i++;
			continue;
		}
		
		else if(curr_ld == ld[i]) {
			
			prev_ld->next = curr_ld->next;
			
			for(j = ZERO; (**ld).attributes[j].name != NULL; j++)
				free(curr_ld->attributes[j].name);
			
			free(curr_ld->attributes[j].name);
			free(curr_ld);
			curr_ld = prev_ld;
			i++;
		}
		
		prev_ld = curr_ld;
		curr_ld = curr_ld->next;
	
	}
	
	return i == size ? SUCCEEDED : FAILED; /* size = selected datas , i = deleted datas */

}