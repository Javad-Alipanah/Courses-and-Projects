#include "dma.h"
#include "common_header.h"
int exec_query(struct database* db,struct query* query, struct linked_data **data) {
	
	struct query* curr_query = query;
	int counter = ZERO, status , flag = ZERO; /* counter = num of queries that executed successfully*/
	
	while(curr_query != NULL) {
		
		if(query->q_type == _dataset) {

			status = data_set(db, query->target, query);
			if (status == SUCCEEDED)
				counter++;
			if(query->next_q == NULL)
				return status;
		}

		else if(query->q_type == _delete_set) {

			status = delete_set(query->target , db);
			
			if(status == SUCCEEDED)
				counter++;
			if(query->next_q == NULL)
				return status;
		}
		
		else if(query->q_type == _insert_into) {
			data = (linked_data**)malloc(sizeof(linked_data*));
			if(data != NULL) {

				status = insert_into(db, query->target, query, *data);

				if(status >= ZERO) 
					counter++;
				if(query->next_q == NULL)
					return status;
			}
		}
		
		else if(query->q_type == _delete_data) {
			status = delete_data(db, query, query->target);
			
			if(status != ZERO)
				counter++;
			if(query->next_q == NULL)
				return status;
		}
		
		else if(query->q_type == _select) {
			
			status = select(db, query->target, data, query);
			
			if(status >= ONE)
				counter++;
			if(query->next_q == NULL)
				return status;
		}
		
		else {
			flag++;
			if(query->next_q == NULL)
				return SEMANTICALLYFAULT;
		}

		curr_query = curr_query->next_q;
	
	}
	
	return counter;

}