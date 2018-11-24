#include "dma.h"
#include "common_header.h"
int insert_into(database* db, char* name, query* Query, linked_data* ld) {

	dataset* curr_ds;
	linked_data* curr_ld, *prev_ld;
	attr_prime* curr_attr;
	int i;

	curr_ds = find_ds(db, name);

	if(curr_ds == NULL)
		return SEMANTICALLYFAULT;

	curr_ld = curr_ds->ld;

	if(curr_ld != NULL) {
		while(curr_ld->next != NULL) {
			prev_ld = curr_ld;
			curr_ld = curr_ld->next;
		}

		if(curr_ds->ld_flag == ONE) { /* ld_flag shows that if first ld initialized or not */
			curr_ld->next = (linked_data*)malloc(sizeof(linked_data));
			curr_ld->next->next = NULL;
			prev_ld = curr_ld;
			curr_ld = curr_ld->next;
		}
	}

	else {
		curr_ds->ld = (linked_data*)malloc(sizeof(linked_data));
		curr_ld = curr_ds->ld;
		curr_ld->next = NULL;
	}

	for(i = ZERO; Query->expression[i].name != NULL; i++) {
		curr_attr = find_attr_prime(i, curr_ds, Query);
		if(curr_attr == NULL) {
			if(curr_ld != curr_ds->ld) {
				prev_ld->next = NULL;
				free(curr_ld);
			}
			else {
				curr_ds->ld = NULL;
				free(curr_ld);
			}
			return SEMANTICALLYFAULT;
		}

		switch(curr_attr->type) {

		case _integer:

			curr_ld->attributes[curr_attr->index].name = (char*) malloc((strlen(Query->expression[i].name) + ONE) * sizeof(char));
			if(curr_ld->attributes[curr_attr->index].name == NULL)
				return FAILED;
			strcpy(curr_ld->attributes[curr_attr->index].name , Query->expression[i].name);
			curr_ld->attributes[curr_attr->index].val.int_val = Query->expression[i].val.int_val;
			break;

		case _float:

			curr_ld->attributes[curr_attr->index].name = (char*) malloc((strlen(Query->expression[i].name) + ONE) * sizeof(char));
			if(curr_ld->attributes[curr_attr->index].name == NULL)
				return FAILED;
			strcpy(curr_ld->attributes[curr_attr->index].name , Query->expression[i].name);
			curr_ld->attributes[curr_attr->index].val.float_val = Query->expression[i].val.float_val;
			break;

		case _string:

			curr_ld->attributes[curr_attr->index].name = (char*) malloc((strlen(Query->expression[i].name) + ONE) * sizeof(char));
			if(curr_ld->attributes[curr_attr->index].name == NULL)
				return FAILED;
			strcpy(curr_ld->attributes[curr_attr->index].name , Query->expression[i].name);
			curr_ld->attributes[curr_attr->index].val.string_val = (char*)malloc((strlen(Query->expression[i].val.string_val) + ONE) * sizeof(char));
			if(curr_ld->attributes[curr_attr->index].val.string_val == NULL) {
				free(curr_ld);
				return FAILED;
			}
			strcpy(curr_ld->attributes[curr_attr->index].val.string_val , Query->expression[i].val.string_val);
			break;
		}

	}

	curr_ld->attributes[i].name = (char*)malloc(sizeof(char));

	if(curr_ld->attributes[i].name == NULL)
		return FAILED;

	curr_ld->attributes[i].name = NULL;
	curr_ds->size++;
	curr_ds->ld_flag = ONE;
	ld = curr_ld;

	return ONE;

}
