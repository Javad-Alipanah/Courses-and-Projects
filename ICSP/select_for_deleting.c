#include "dma.h"
#include "common_header.h"
int select_for_deleting(database* db, char* name, linked_data*** ld, query* Query) { /* Makes an array of pointers to "linked_data"s which most be deleted */
	
	dataset* curr_ds;
	linked_data* curr_ld;
	attr_prime* curr_attr;
	int i, j, t = ZERO, counter = ZERO, count = ZERO;
	
	curr_ds = find_ds(db, name);
	
	if(curr_ds == NULL)
		return SEMANTICALLYFAULT;
	
	while(t < TWO) {
		t++;
		curr_ld = curr_ds->ld;
		if(curr_ld == NULL)
			return SEMANTICALLYFAULT;
		
		while(curr_ld != NULL) {
			j = ZERO;
			
			for(i = ZERO; Query->expression[i].name != NULL; i++) {
				curr_attr = find_attr_prime(i, curr_ds, Query);
				if(curr_attr == NULL)
					return SEMANTICALLYFAULT;
				
				switch(curr_attr->type) {
			
				case _integer:
					if(curr_ld->attributes[curr_attr->index].val.int_val == Query->expression[i].val.int_val) {
						j++;
						break;
					}
				
				case _float:
					if(curr_ld->attributes[curr_attr->index].val.float_val == Query->expression[i].val.float_val) {
						j++;
						break;
					}
				
				case _string:
					if(!strcmp(curr_ld->attributes[curr_attr->index].val.string_val , Query->expression[i].val.string_val)) {
						j++;
						break;
					}
				
				}

			}
			
			if( i == j && t == ONE)
				counter++;
			
			else if(i == j) {
				(*ld)[count] = curr_ld;
				count++;
			}
			
			curr_ld = curr_ld->next;
		
		}
		
		if( t == ONE ) {
			
			if( counter == ZERO )
				return counter;
			
			*ld = (linked_data**)malloc(counter * sizeof(linked_data*));
			
			if(ld == NULL)
				return FAILED;
		
		}
	
	}

	return counter;

}