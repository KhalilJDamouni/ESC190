#include "lab4.h"
#include <math.h>
#include "lab4_utilities.h"
#define NUM_HASH_FUNCS 3

char c;
/*
int main(void)
{	
//OG

	HashTable* table = create_hash_table(0, 3);
	PersonalData data1 = {781192679, 'F', "Alice","Xu", "AB12345", "99999-999-999999999999", 1960, 31, 12};
	print_buckets(table);	
	printf("\n");
	update_key(&data1, &table);

	PersonalData data2 = (PersonalData) {438815292, 'M', "Bob", "Kim", "XXX", "YYY", 1999, 5, 12};
	print_buckets(table);
	printf("\n");
	update_key(&data2, &table);

	PersonalData data3 = {839575255, 'F', "Alice","Xu", "AB12345", "99999-999-999999999999", 1960, 31, 12};
	print_buckets(table);	
	printf("\n");
	update_key(&data3, &table);

	PersonalData data4 = (PersonalData) {604563791, 'M', "Bob", "Kim", "XXX", "YYY", 1999, 5, 12};
	print_buckets(table);
	printf("\n");
	update_key(&data4, &table);

	PersonalData data5 = {987599156, 'F', "Alice","Xu", "AB12345", "99999-999-999999999999", 1960, 31, 12};
	print_buckets(table);	
	printf("\n");
	update_key(&data5, &table);

	PersonalData data6 = (PersonalData) {973880511, 'M', "Bob", "Kim", "XXX", "YYY", 1999, 5, 12};
	print_buckets(table);
	printf("\n");
	update_key(&data6, &table);


	PersonalData data7 = {552568917, 'F', "Alice","Xu", "AB12345", "99999-999-999999999999", 1960, 31, 12};
	print_buckets(table);	
	printf("\n");
	update_key(&data7, &table);

	PersonalData data8 = (PersonalData) {641315320, 'M', "Bob", "Kim", "XXX", "YYY", 1999, 5, 12};
	print_buckets(table);
	printf("\n");
	update_key(&data8, &table);

	PersonalData data9 = {399673474, 'F', "Alice","Xu", "AB12345", "99999-999-999999999999", 1960, 31, 12};
	print_buckets(table);	
	printf("\n");
	update_key(&data9, &table);

	print_buckets(table);


//UPDATE
	
	PersonalData data10 = {781192679, 'F', "Alice","Xu", "AB12345", "99999-999-999999999999", 1960, 31, 12};
	//print_buckets(table);	
	printf("\n");
	update_key(&data10, &table);

	PersonalData data11 = (PersonalData) {438815292, 'M', "Bob", "Kim", "XXX", "YYY", 1999, 5, 12};
	//print_buckets(table);
	printf("\n");
	update_key(&data11, &table);

	PersonalData data12 = {552568917, 'F', "Alice","Xu", "AB12345", "99999-999-999999999999", 1960, 31, 12};
	//print_buckets(table);	
	printf("\n");
	update_key(&data12, &table);

	PersonalData data13 = (PersonalData) {641315320, 'M', "Bob", "Kim", "XXX", "YYY", 1999, 5, 12};
	//print_buckets(table);
	printf("\n");
	update_key(&data13, &table);

	PersonalData data14 = {399673474, 'F', "Alice","Xu", "AB12345", "99999-999-999999999999", 1960, 31, 12};
	//print_buckets(table);	
	printf("\n");
	update_key(&data14, &table);

	printf("\n");
	print_buckets(table);	

	//print_personal_data(lookup_key(641315320, table));

	delete_table(table);
}*/

HashTable *create_hash_table(int m, int mode)
{
	//printf("*create_hash_table\n");
    hash_funcs[0] = trivial_hash;
    hash_funcs[1] = pearson_hash;
    hash_funcs[2] = fibonacci_hash;

	HashTable* table = malloc(sizeof(HashTable));

	table->num_buckets = pow(2,m);
	table->num_keys = 0;
	table->mode = mode;

	Node** buckets = malloc(sizeof(Node*) * pow(2,m));

	for(int i = 0; i < pow(2,m); ++i)
	{
		buckets[i] = NULL;
	}

	table->buckets = buckets;

	return table;	
}

void update_without_resize(PersonalData * data, HashTable *table) 
{
	//printf("update_without_resize\n");
	INT_HASH hash;
	if(table->mode != 3)
		hash = hash_funcs[table->mode](data->SIN,table->num_buckets);
	else
		hash = hash_funcs[0](data->SIN,table->num_buckets);
	//printf("Hash found: %lu\n", hash);
	//scanf(" %c", &c);
	Node* n = malloc(sizeof(Node));

	n->value = data;
	n->next = NULL;

	
	if(table->buckets[hash])
	{
		//printf("Theres collision\n");
		if(table->mode == CLOSED_ADDRESSING)
		{			
			if(table->buckets[hash] != NULL)
			{
				Node* temp;
				Node* previous = NULL;
				temp = table->buckets[hash];
                if(temp->value != NULL && temp->value->SIN == data->SIN)
                {
                    //printf("Getting here?\n");
                    //exit(1);
                    n->next = temp->next;
                    free(temp);
                    table->buckets[hash] = n;
                }
                else
                {
                    while(temp->next != NULL && temp->value->SIN != data->SIN)
                    {
                        previous = temp;
                        temp = temp->next;
                    }

                    if(temp->value->SIN != data->SIN)
                    {
                        free(temp->next);
                        temp->next = n;;
                        table->num_keys += 1;
                    }
                    else if(previous == NULL)
                    {
                        free(table->buckets[hash]);
                        table->buckets[hash] = n;
                    }
                    else
                    {
                        n->next = temp->next;
                        free(temp);
                        previous->next = n;
                        //table->num_keys += 1;
                    }
                }
			}
		}
		if(table->mode == LINEAR_PROBING)
		{
			INT_HASH new_hash = hash;
			for(int i = 0; i < table->num_buckets; ++i)
			{
				new_hash += i;
				new_hash %= table->num_buckets;
				
				if(table->buckets[new_hash] == NULL)
				{
                			//printf("Coming here1\n");
					table->buckets[new_hash] = n;
					table->num_keys += 1;
					break;
				}	
				else if(table->buckets[new_hash]->value->SIN == data->SIN)
				{
					//printf("Coming here2\n");
					free(table->buckets[new_hash]);
					table->buckets[new_hash] = n;
					break;
				}
			}
		}
		else if(table->mode == QUADRATIC_PROBING)
		{
			INT_HASH new_hash = hash;
			for(int i = 0; pow(i,2) < table->num_buckets; ++i)
			{
				new_hash = hash + pow(i,2);
				new_hash %= table->num_buckets;
				//printf("Current Hash is: %lu\n", new_hash);
				if(table->buckets[new_hash] == NULL)
				{
					//printf("Placing Bucket at hash %lu\n", new_hash);
					table->buckets[new_hash] = n;
					table->num_keys += 1;
					break;
				}	
				else if(table->buckets[new_hash]->value->SIN == data->SIN)
				{
					free(table->buckets[new_hash]);
					//printf("Updating Bucket at hash %lu\n", new_hash);
					table->buckets[new_hash] = n;
					break;
				}
				//new_hash = new_hash + pow(i,2);
				//new_hash %= table->num_buckets;
			}
		}
		else if(table->mode == CUCKOO)
		{	
			//Need to account for the need to replace. Cuz it wont find it by itself.	
			if(lookup_key(data->SIN, table) != NULL)
			{
				//printf("Person In Table\n");
                //print_buckets(table);
				//exit(1);
				for(int i = 0; i < 3; ++i)
				{
					hash = hash_funcs[i](data->SIN,table->num_buckets);
					if(table->buckets[hash] != NULL && table->buckets[hash]->value->SIN == data->SIN)
					{
						//free(table->buckets[hash]->value);
						free(table->buckets[hash]);
						table->buckets[hash] = n;
						break;
					}
				}
			}

			else
			{	
				//printf("Person not in Table\n");
				Node* temp = n;
                Node* temp2;
				INT_HASH old_hash = -1;
				int placed = 0;
				while(!placed && old_hash != hash)
				{
                    
					for(int i = 0; i < 3; ++i)
					{
						hash = hash_funcs[i](temp->value->SIN,table->num_buckets);
						//printf("Trying hash: %lu\n", hash);
						if(table->buckets[hash] == NULL) //This is an empty spot
						{
							//printf("Placing at hash: %lu\n", hash);
							table->buckets[hash] = temp;
							placed = 1;
							table->num_keys++;
							break;
						}
                        //DEBUG
                        else
                        {
                            //printf("I am trying to place\n");
                            //print_personal_data(temp->value);
                            //printf("at %lu, but\n", hash);
                            //print_personal_data(table->buckets[hash]->value);
                            //printf("Is there\n");

                        }
					}
					if(!placed && old_hash != hash) //None of the three spots are empty.
					{
						//printf("Replacing\n");
                        //print_buckets(table);
						temp2 = table->buckets[hash];
						table->buckets[hash] = temp;
                        temp = temp2;
						old_hash = hash;
						hash = 0;
                        //print_buckets(table);
					}
				}
				if(!placed && old_hash == hash)
				{
					//printf("Throw away\n");
                    //print_status(table);
                    //print_buckets(table);
                    //exit(1);
					free(temp);
				}	
			}
		}
	}
	else
	{
		free(table->buckets[hash]);
		table->buckets[hash] = n;
		table->num_keys += 1;

	}
}

void update_key(PersonalData * data, HashTable** table)
{ 
    //printf("update_key\n");
	//printf("\n");
    //scanf(" %c", &c);
	
	if(lookup_key(data->SIN, *table) != NULL)
	{
        //Person is in table, just update.
		update_without_resize(data, (*table));
	}

	else
	{
        //Person not in table, need to add.
		float max_load = max_load = ((float)((*table)->num_keys + 1) / (*table)->num_buckets);	

		while(max_load > MAX_LOAD_FACTOR)
		{	
			//printf("Making a new table\n");
			HashTable* temp = (*table);
			temp = resize_table(temp);
			*table = temp;
			table = &temp;
			//print_status(*table);
			max_load = ((float)((*table)->num_keys + 1)/ (*table)->num_buckets);
		}
		update_without_resize(data, (*table));
	}
	
}

int delete_key(INT_SIN SIN, HashTable *table)
{
	//printf("delete_key\n");

	if(table->mode != CUCKOO)
	{
		INT_HASH hash = hash_funcs[table->mode](SIN,table->num_buckets);

		if(table->buckets[hash])
		{
			if(table->buckets[hash]->value->SIN == SIN) 
			{
				Node* delete = table->buckets[hash];
				table->buckets[hash] = table->buckets[hash]->next;
				free(delete);
				table->num_keys -= 1;
				return 1;
			}
			else if(table->mode == CLOSED_ADDRESSING) 
			{
				if(table->buckets[hash]->next != NULL)
				{
					Node* delete = table->buckets[hash]->next;
					Node* previous = table->buckets[hash];

					while(delete->next != NULL && delete->value->SIN != SIN)
					{
						previous = delete;
						delete = delete->next;
					}
					if(delete->value->SIN == SIN)
					{
						previous->next = delete->next;				
						free(delete);	
						table->num_keys -= 1;
						return 1;		
					}
					else
						return 0;
				}
			}
			
			else if(table->mode == LINEAR_PROBING)
			{
				for(int i = 0; i < table->num_buckets; ++i)
				{
					hash += 1;
					hash %= table->num_buckets;
					if(table->buckets[hash] && table->buckets[hash]->value->SIN == SIN)
						break;
				}
				if(table->buckets[hash] && table->buckets[hash]->value->SIN == SIN)
				{
					free(table->buckets[hash]);
					table->buckets[hash] = NULL;
					return 1;
				}
				else
					return 0;
			}

			else if(table->mode == QUADRATIC_PROBING)
			{
				for(int i = 1; i <= table->num_buckets; ++i)
				{
					hash += pow(i,2);
					hash %= table->num_buckets;
					if(table->buckets[hash] && table->buckets[hash]->value->SIN == SIN)
						break;
				}
				if(table->buckets[hash] && table->buckets[hash]->value->SIN == SIN)
				{
					free(table->buckets[hash]);
					table->buckets[hash] = NULL;
					return 1;
				}
				else
					return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else if(table->mode == CUCKOO)
	{
		INT_HASH hash = hash_funcs[0](SIN,table->num_buckets);
		Node* delete;
		INT_HASH old_hash = -1;

		for(int i = 0; i < 3; ++i)
		{
			hash = hash_funcs[i](SIN, table->num_buckets);
			if(table->buckets[hash] != NULL && table->buckets[hash]->value->SIN == SIN)
			{
				delete = table->buckets[hash];
				table->buckets[hash] = NULL;
				free(delete);
				table->num_keys -= 1;
				return 1;
			
			}
		}
	}
	return 0;
}

PersonalData* lookup_key(INT_SIN SIN, HashTable *table)
{	
	//printf("lookup_key\n");

	PersonalData* return_value = NULL;

	if(table->mode != CUCKOO)
	{
		INT_HASH hash = hash_funcs[table->mode](SIN,table->num_buckets);
		//printf("Checking Hash: %lu\n", hash);
		if(table->buckets[hash])
		{
			if(table->buckets[hash]->value->SIN == SIN) 
			{
				return_value = table->buckets[hash]->value;
			}
			else if(table->mode == CLOSED_ADDRESSING)
			{
				if(table->buckets[hash]->next != NULL)
				{

					Node* delete = table->buckets[hash]->next;
					Node* previous = table->buckets[hash];

					while(delete->next != NULL && delete->value->SIN != SIN)
					{

						previous = delete;
						delete = delete->next;
					}
					if(delete->value->SIN == SIN)
					{
						return_value = delete->value;		
					}
				}
			}
			
			else if(table->mode == LINEAR_PROBING)
			{	
				//INT_HASH originalHash = hash;
				for(int i = 0; i < table->num_buckets; ++i)
				{
					hash += 1;
					hash %= table->num_buckets;
					if(table->buckets[hash] && table->buckets[hash]->value->SIN == SIN)
						break;
				}
				if(table->buckets[hash] && table->buckets[hash]->value->SIN == SIN)
				{
					return_value = table->buckets[hash]->value;
				}
			}

			else if(table->mode == QUADRATIC_PROBING)
			{
				INT_HASH originalHash = hash;
				for(int i = 0; pow(i,2) < table->num_buckets; ++i)
				{
					//printf("Looking by Quadratic_Probing\n");
					hash = originalHash +  pow(i,2);
					hash %= table->num_buckets;
					//printf("Checking Hash: %lu\n", hash);
					if(table->buckets[hash] && table->buckets[hash]->value->SIN == SIN)
					{
						break;
					}
				}
				if(table->buckets[hash] && table->buckets[hash]->value->SIN == SIN)
				{
					//printf("Found at hash %lu\n", hash);
					return_value = table->buckets[hash]->value;
				}
			}
		}
	}
	else if(table->mode == CUCKOO)
	{
		INT_HASH hash;

		for(int i = 0; i < 3; ++i)
		{
			hash = hash_funcs[i](SIN, table->num_buckets);
			//printf("Checking hash %lu\n", hash);
            //print_buckets(table);
            //print_status(table);
			if(table->buckets[hash] != NULL && table->buckets[hash]->value->SIN == SIN) 
			{
				//printf("Found %lu at %lu\n", SIN, hash);
				return_value = table->buckets[hash]->value;
				break;
			}
            //printf("Am i getting here before crashing\n");
		}
	}

	return return_value;
}

void delete_table(HashTable *table)
{	
	//printf("delete_table\n");

	Node* temp;
	if(table->mode != CUCKOO)
	{	
		for (INT_HASH b = 0; b < table->num_buckets; b++)
		{ 
			Node * bucket = table->buckets[b];
			//printf("Bucket %ld:\t", b);
			while (bucket != NULL)
			{
				if(bucket->value == NULL) 
				{
					//printf(" - \t");
					continue;
				}
				else
				{
					temp = bucket->next;; 
					free(bucket);
				}
				
				bucket = temp;
			}
		}			
		free(table->buckets);		
		free(table);
		return;
	}
	else
	{
		for (INT_HASH b = 0; b < table->num_buckets; b++)
		{ 
			if(table->buckets[b] != NULL) 
			{
				//free(table->buckets[b]->value);
				free(table->buckets[b]);
			}
		}			
		free(table->buckets);		
		free(table);
		return;
	}
}

HashTable *resize_table(HashTable *table) //This works.
{
	//printf("resize_table\n");
    
	INT_HASH m;
	if(table->num_buckets == 1)
		m = 1;
	else
		m = log2(table->num_buckets) + 1;
	HashTable* new_table = create_hash_table(m, table->mode);
	//printf("\n The New TABLE IS:\n");
	//print_status(new_table);
	new_table->num_buckets = pow(2,m);
	new_table->mode = table->mode;

	for(INT_HASH b = 0; b < table->num_buckets; b++)
	{
		Node* bucket = table->buckets[b];
		if(bucket != NULL) 
		{
			if(table->mode == CLOSED_ADDRESSING)
			{	
				Node* temp = table->buckets[b];
				while(temp != NULL)
				{
					update_without_resize(temp->value, new_table);
					temp = temp->next;
				}
				new_table->num_keys = table->num_keys;
			}
			else
			{
				PersonalData* current_person = table->buckets[b]->value;
				//printf("Moving :");
				//print_personal_data(current_person);
				//print_buckets(new_table);
				update_without_resize(current_person, new_table);
			}
		}
	}
	delete_table(table);
    //print_buckets(new_table);
	return new_table;
}

