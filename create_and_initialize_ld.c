#include "dma.h"
#include "common_header.h"
linked_data* create_and_initialize_ld(dataset* ds, linked_data* sample) { /* Used in "select" func for making copy from selected data */
	
	int i, temp1, temp2, flag = ZERO , idx = ZERO;
	attr_prime* curr_attr;
	
	linked_data* ld = (linked_data*)malloc(sizeof(linked_data));
	
	if(ld == NULL)
		return NULL;
	
	for(i = ZERO; i < MAX_NUM_OF_ATTRS; i++) {
		curr_attr = ds->hash_attr[i];
		if(curr_attr == NULL)
			continue;
		
		while(curr_attr != NULL) {
			temp1 = ds->hash_attr[i]->index;
			if(flag)
				idx = temp1 > temp2 ? (temp1 > idx ? temp1 : idx) : (temp2 > idx ? temp2 : idx); /* idx equals with last index */
			ld->attributes[curr_attr->index].name = (char*)malloc((strlen(curr_attr->attr.name) + ONE) * sizeof(char));
			if(ld->attributes[curr_attr->index].name == NULL) {
				free(ld);
				return NULL;
			}
			
			strcpy(ld->attributes[curr_attr->index].name , curr_attr->attr.name);
			
			switch(curr_attr->type) { /* Finds and initializes correct variable */
			
			case _integer:
				ld->attributes[curr_attr->index].val.int_val = sample->attributes[curr_attr->index].val.int_val;
				break;
			
			case _float:
				ld->attributes[curr_attr->index].val.float_val = sample->attributes[curr_attr->index].val.float_val;
				break;
			
			case _string:
				ld->attributes[curr_attr->index].val.string_val = (char*) malloc((strlen(sample->attributes[curr_attr->index].val.string_val) + ONE) * sizeof(char));
				if(ld->attributes[curr_attr->index].val.string_val == NULL) {
					free(ld);
					return NULL;
				}
				strcpy(ld->attributes[curr_attr->index].val.string_val , sample->attributes[curr_attr->index].val.string_val);
				break;
			
			}
			
			curr_attr = curr_attr->next;
			temp2 = ds->hash_attr[i]->index;
			flag = ONE;
		}

	}
	ld->next = NULL;
	ld->attributes[idx + ONE].name = (char*)malloc(sizeof(char));
	
	if(ld->attributes[idx + ONE].name == NULL)
		return NULL;
	
	strcpy(ld->attributes[idx + ONE].name , "");
	
	return ld;
}