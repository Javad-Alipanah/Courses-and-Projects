#include "dma.h"
#include "common_header.h"
int select(database* db, char* name, linked_data** ld, query* Query){
	
	int i, j, counter = ZERO;
	attr_prime* curr_attr;
	linked_data* curr_ld , *tmp;
	dataset* curr_ds = db->ds_hash[ds_hash_func(name, db->size)];
	
	if(curr_ds == NULL)
		return SEMANTICALLYFAULT;
	
	while(strcmp(curr_ds->name , name)) {
		curr_ds = curr_ds->next;
		if(curr_ds == NULL)
			return SEMANTICALLYFAULT;
	}
	
	curr_ld = curr_ds->ld;
	
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
			default:
				return SEMANTICALLYFAULT;
			}
		
		}
		
		if(i == j) /* i = amount of expression , j = amount of features that are compatible with expressions */
			counter++;
		if(i == j && counter == ONE) { /* Creating head of selected datas */
			*ld = create_and_initialize_ld(curr_ds, curr_ld);
			if(*ld == NULL)
				return FAILED;
			tmp = *ld;
		}
		else if(i == j) {
			tmp->next = create_and_initialize_ld(curr_ds, curr_ld);
			if(tmp->next == NULL)
				return FAILED;
			tmp = tmp->next;
		}

		curr_ld = curr_ld->next;
	}

	if(counter != ZERO)
		tmp->next = NULL;
	else
		*ld = NULL;
	
	return counter;

}