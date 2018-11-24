#include "dma.h"
#include "common_header.h"
int data_set(database* db, char* name, query* Query) {
	
	attr_prime* ap , *curr_attr;
	dataset* curr_ds;
	int i, hash = ds_hash_func(name, db->size);
	dataset* ds = (dataset*)malloc(sizeof(dataset));
	
	if(ds == NULL)
		return FAILED;
	
	ds->name = (char*)malloc((strlen(name) + ONE) * sizeof(char));
	
	if(ds->name == NULL) {
		free(ds);
		return FAILED;
	}
	
	strcpy(ds->name, name);
	
	if(db->ds_hash[hash] == NULL)
		db->ds_hash[hash] = ds;
	
	else {
		curr_ds = db->ds_hash[hash];
		
		while(curr_ds->next != NULL) {
			curr_ds = curr_ds->next;
		}

		curr_ds->next = ds;
		curr_ds->next->next = NULL;
	}
	
	ds->ld = (linked_data*)malloc(sizeof(linked_data));
	
	if(ds->ld == NULL) {
		free(ds->name);
		free(ds);
		return FAILED;
	}
	
	for(i = ZERO; i < MAX_NUM_OF_ATTRS; i++)
		ds->hash_attr[i] = NULL;
	
	for(i = ZERO; Query->expression[i].name != NULL; i++) {
		
		ds->ld->attributes[i].name = (char*)malloc((strlen(Query->expression[i].name) + ONE) * sizeof(char));
		
		if(ds->ld->attributes[i].name == NULL) {
			free(ds->name);
			free(ds->ld);
			free(ds);
			return FAILED;
		}
		
		strcpy(ds->ld->attributes[i].name , Query->expression[i].name);
		ap = (attr_prime*)malloc(sizeof(attr_prime));
		
		if(ap == NULL) {
			free(ds->name);
			free(ds->ld->attributes[i].name);
			free(ds->ld);
			free(ds);
			return FAILED;
		}
		
		ap->attr.name = (char*)malloc((strlen(Query->expression[i].name) + ONE) * sizeof(char));
		if(ap->attr.name == NULL) {
			free(ds->name);
			free(ds->ld->attributes[i].name);
			free(ds->ld);
			free(ds);
			free(ap);
			return FAILED;
		}
		
		strcpy(ap->attr.name , Query->expression[i].name);
		curr_attr = ds->hash_attr[hash_attr_func(Query->expression[i].name)];
		
		if(curr_attr == NULL) {
			ds->hash_attr[hash_attr_func(Query->expression[i].name)] = ap;
			ds->hash_attr[hash_attr_func(Query->expression[i].name)]->index = i;
			ds->hash_attr[hash_attr_func(Query->expression[i].name)]->next = NULL;
			curr_attr = ds->hash_attr[hash_attr_func(Query->expression[i].name)];
		}
		
		else {
			while(curr_attr->next != NULL) {
				curr_attr = curr_attr->next;
			}
			curr_attr->next = ap;
			curr_attr->index = i;
			curr_attr->next->next = NULL;
		}
		
		curr_attr->type = Query->expression[i].val._type;
	
	}

	ds->ld->attributes[ZERO].name = NULL;
	ds->ld->attributes[i].name = NULL;
	ds->size = ZERO;
	ds->next = NULL;
	ds->ld->next = NULL;
	ds->ld_flag = ZERO;
	
	return SUCCEEDED;

}