#include "dma.h"
#include "common_header.h"
int show_dataset_schemas(struct database* db) {
	
	char integer[] = "integer";
	char Float[] = "float";
	char string[] = "string";
	dataset* curr_ds;
	attr_prime* curr_attr;
	int i, j;
	
	if(db == NULL)
		return FAILED;
	
	for(i = ZERO; i < db->size; i++) {
		if(db->ds_hash[i] != NULL) {
			curr_ds = db->ds_hash[i];
			
			while(curr_ds != NULL) {
				printf("Dataset name: %s\n" , curr_ds->name);
				
				for(j = ZERO; j < MAX_NUM_OF_ATTRS; j++) {
					if(curr_ds->hash_attr[j] != NULL) {
						curr_attr = curr_ds->hash_attr[j];
						
						while(curr_attr != NULL) {
							printf("\tField: %s\tType: %s\n",curr_attr->attr.name, curr_attr->type == _integer ? integer : curr_attr->type == _float ? Float : string );
							curr_attr = curr_attr->next;
						}

					}

				}

				curr_ds = curr_ds->next;
			}
		}

	}

	return SUCCEEDED;

}