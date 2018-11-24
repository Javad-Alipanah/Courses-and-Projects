#ifndef  _DMA_
#define _DMA_
#define MAX_NUM_OF_ATTRS 50
#define MAX_NUM_OF_EXPRS 50
#define MAX_STRING_SIZE 100

enum directive {
	_dataset,	
	_delete_set,
	_insert_into,
	_delete_data,
	_select
};
/**
 * types of query directives 
 * 
 * _dataset :
 *              Creates a new dataset, 
 *              The caller must determine following values in the query structure :
 *                       char target[] = a null terminated string identifying the name of the data set, 
 *                       struct expr[] expression  = for each field variable one expession indicating its name and type
 *_delete_set:
 *              Deletes a dataset, completely removing its elements and freeing all the memory associated with it
 *              The caller must determine following values in the query structure:
 *                      char target[] : name of the target dataset
 *_insert_into:
 *              Inserts a single data record into the target set
 *              The caller must determine following values in the query structure :
 *                      char target[] : a null terminated string identifying name of the target dataset
 *                      struct expr[] expression :  an array whose each element determines the value of one field variable and its name
 *_delete:
 *              Deletes single/multiple data records from the target set  which satisfy all the conditions specified by expression field
 *              The caller must determine the following values in the query structure:
 *                      char target[] : a null terminated string identifying name of the target dataset
 *                      struct expr[] expression :  each element descibing one aspect of the data to be removed ( can be more than one ) 
 * _select:
 *              Selects single/multiple data records from the target set which satisfy all the conditions specified by expression field
 *              The caller must determine the following values in the query structure:
 *                      char target[] : a null terminated string identifying name of the target dataset
 *                      struct expr[] expression :  each element descibing one aspect of the data to be removed ( can be more than one ) 
 */

enum type {
	_integer,
    _float,
	_string
};
/*
 * primitive data types accepted
 */

union value{
	enum type _type;
	int int_val;
    float float_val;
	char* string_val;
};
struct attribute{
	char *name;
	union value val;
};
struct linked_data{
        struct linked_data* next;
        struct attribute attributes[MAX_NUM_OF_ATTRS];
};
/*
 * Each single data record is saved in this form, in the database
 * Attributes represent input struct's field variables
 */
/*
struct expr {
        char *attr_name;
        union value val;
};*/
/*
 * For now, this struct is the same as "struct attribute". It will be expanded in the next phase.
 * In this phase : each expression describes an equality : 
 *              attr_name = val
 */

struct query {
    struct query *next_q;
    char target[MAX_STRING_SIZE];
	enum directive q_type; 
	struct attribute expression[MAX_NUM_OF_EXPRS];
}; 
/*
 * Represents queries to the system :
 *      struct query* next_q : points to the next query
 *      enum directive q_type : specifies the kind of query 
 *      char target[] : name of  target data set
 *      the last expression( = attribute)'s name is NULL
 */

struct database; 
/**
 * you should specify the details of this struct in your project,
 * this is the main struct containing all the tables and datasets
 * The client will pass this struct to your functions, each time they need to execute a query
 */

int initialize_database(struct database** db, unsigned int size, char *name);
/*
 * Allocates and initializes all the memory needed with respect to the internal structure you define for the system.
 * arguments:
 *      struct database **db :  after returning from the function it should point to the initialized database structure
 *      unsigned int size : size of datasets hash table
 *      char* name: null termianted string, name of the database
 * returns :
 *      0      :        on success 
 *      -1     :        on failure ( for instance if there is not enough space to allocate, null argument , ... )
 */

int delete_database(struct database *db);
/**
 * Destroys the database, freeing all the allocated memory.
 * arguments:
 *      struct database* :      points to the dataabse structure
 * returns :
 *      0 :     on success
 *      -1:     on failure ( for instance if one of the pointers in the structure was corrupted and freeing has failed )
 */

int exec_query(struct database* db,struct query* query, struct linked_data **data);
/*
 * Executes the query on the database.
 * arguments:
 *              struct database* db:    pointer the database on which the query is to be executed
 *              struct query*query:
 *                      the query to be executed on the database
 *              struct data **data:
 *                      the address of the memory which points to the data.
 *                      after returning from the function this variable points to the data collection returned as a result of perfoming the query
 *						query types returning data : _insert_into: inserted records, _select: all the records satisfying the conditions,	
 *                      in case of queries which do not need it, it will be untouched
 * returns :
 *      n:      on success, n is the number of data elements returned as a result of executing the query
 *      0:      on success, in case of queries that do not return data 
 *      -1:     on structurally invalid input ( for instance, NULL pointer )
 *      -2:     on semantically invalid input (for instance, invalid data in query object )
 */

int show_dataset_schemas(struct database* db);
/*
 * Prints to the standard output for each dataset its internal structure ( = fields' names and types)
 * arguments:
 *              struct database* db :   pointer to the database object
 * returns:
 *      0 : on success
 *      -1: on failure ( NULL input , uninitialized db)
 */
#endif