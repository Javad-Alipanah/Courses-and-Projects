#include "dma.h"
#include "common_header.h"
int initialize_database(database** db, unsigned int size, char *name) {
	
	unsigned int i, j;
	
	(*db) = (database*)malloc(sizeof(database));
	
	if(*db == NULL) {
		return FAILED;
	}
	
	(**db).ds_hash = (dataset**)malloc(size * sizeof(dataset*));
	
	if((**db).ds_hash == NULL) {
		free(*db);
		return FAILED;
	}
	
	for(j = ZERO; j < size; j++)
		(**db).ds_hash[j] = NULL;
	
	(**db).name = (char*)malloc((strlen(name) + ONE) * sizeof(char));
	
	if((**db).name == NULL) {
		free(*db);
		free((**db).ds_hash);
		return FAILED;
	
	}
	
	for(i = ZERO; i < size; i++)
		(**db).ds_hash[i] = (dataset*)NULL;
	
	strcpy((**db).name , name);
	(**db).size = size;
	
	return SUCCEEDED;
}