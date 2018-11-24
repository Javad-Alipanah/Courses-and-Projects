#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#define SEMANTICALLYFAULT	-2
#define FAILED				-1
#define SUCCEEDED			0
#define ZERO				0
#define ONE					1
#define TWO					2

int ds_hash_func(char* name, int size);

int hash_attr_func(char* name);

typedef struct query query;

typedef struct linked_data linked_data;

typedef struct attr_prime { /* Defined to save an attribute with its index and type and to make possible having a linked list of attributes */
	struct attribute attr;
	int index;
	enum type type;
	struct attr_prime* next;
}attr_prime;

typedef struct dataset {
	char* name;
	linked_data* ld;
	struct dataset* next;
	int size;
	attr_prime* hash_attr[MAX_NUM_OF_ATTRS];
	int ld_flag; /* If first linked data initialized this variable equals to one */
}dataset;

typedef struct database {
	dataset** ds_hash;
	char* name;
	int size;
}database;

dataset* find_ds(database* db, char* name);

attr_prime* find_attr_prime(int i, dataset* curr_ds, query* Query);