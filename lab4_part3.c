#include "lab4.h"
#include <stdlib.h>


char c;
/*
int main(void)
{   
    void test2();
    void test3();

    test2();
    test3();
	
}

void test2(){
	printf("===== Test 2: counter_intelligence =====\n");
	char* load_fn = "test/test_data_LOAD.txt";
	char* update_fn = "test/test_data_UPDATE.txt";
	char* validate_fn = "test/test_data_VALIDATE.txt";
	char* spies_fn = "test/spies3.txt";
	counter_intelligence(load_fn, update_fn, validate_fn, spies_fn);
}

void test3()
{
    printf("===== Test 3: counter_intelligence full data =====\n");
	char* load_fn = "test/full_data_LOAD.txt";
	char* update_fn = "test/full_data_UPDATE.txt";
	char* validate_fn = "test/full_data_VALIDATE.txt";
	char* spies_fn = "test/full_spies3.txt";
	counter_intelligence(load_fn, update_fn, validate_fn, spies_fn);
}*/



PersonalData** parse_data(char* fn)
{
    
    FILE* in = fopen(fn, "r");

    long int lines = 0;
    char current_line[256];
    current_line[0] = '\0';
    while(fgets(current_line, 256, in))
    {
        ++lines;
    }
    rewind(in);
    --lines;
    //printf("Number of line is: %i\n", lines);
    PersonalData** data = malloc(sizeof(PersonalData*) * lines);

    long int index = 0;
    fgets(current_line, 256, in);
    PersonalData* current_entry;
    while(fgets(current_line, 256, in))
    {   
        current_entry = malloc(sizeof(PersonalData));

        INT_SIN num = atoi(strtok(current_line, "\t"));
        current_entry->SIN = num;
        current_entry->gender = *strtok(NULL, "\t");
        strcpy(current_entry->first_name, strtok(NULL, "\t"));
        strcpy(current_entry->last_name, strtok(NULL, "\t"));
        strcpy(current_entry->passport_num, strtok(NULL, "\t"));
        strcpy(current_entry->bank_acc_num, strtok(NULL, "\t"));
        current_entry->dob_year = atoi(strtok(NULL, "-"));
        current_entry->dob_month = atoi(strtok(NULL, "-"));
        current_entry->dob_day = atoi(strtok(NULL, "\n"));

        data[index] = current_entry;
        ++index;
    } 
    fclose(in);

    return data;
}

void counter_intelligence(char* load, char* update, char* validate, char* outfile)
{
    PersonalData** load_data = parse_data(load);
    HashTable* table = create_hash_table(0, 0);
    FILE* f_load = fopen(load, "r");
   

    char current_line[256];
    current_line[0] = '\0';
    long int load_lines = 0;
    
    fgets(current_line, 256, f_load); //Get rid of header
    while(fgets(current_line, 256, f_load))
    {
        ++load_lines;
    }
    char c;
    for(long int i = 0; i < load_lines; ++i)
    {        
        //print_buckets(table);
        //printf("Placing %lu\n", load_data[i]->SIN);
        //printf("Done %0.2f%% or %i\n", ((float)i / (float)load_lines) * 100, i);
        update_key(load_data[i], &table);
        //printf("\n");
        //scanf(" %c", &c);
    }
    //print_buckets(table);
    //print_status(table);
    //exit(1);
   
    FILE* f_update = fopen(update, "r");
    PersonalData** update_data = parse_data(update);

    fgets(current_line, 256, f_update); //Get rid of header
    long int update_lines = 0;
    while(fgets(current_line, 256, f_update))
    {
        ++update_lines;
    }
    
    for(long int i = 0; i < update_lines; ++i)
    {
        //printf("\n");
        //scanf(" %c", &c);
        //print_personal_data(update_data[i]);
        //printf("Done %0.2f%% or %i\n", ((float)i / (float)update_lines) * 100, i);
        update_key(update_data[i], &table);
    }
    //print_buckets(table);
    //print_status(table);


    FILE* f_validate = fopen(validate, "r");
    FILE* out = fopen(outfile, "w");
    PersonalData** validate_data = parse_data(validate);
    fgets(current_line, 256, f_validate); //Get rid of header
    long int validate_lines = 0;
    while(fgets(current_line, 256, f_validate))
    {
        ++validate_lines;
    }
    
    for(long int i = 0; i < validate_lines; ++i)
    {
        char sin[100];
        sin[0] = '\0';
        PersonalData* found = lookup_key(validate_data[i]->SIN, table);
        
	int real = 1;
        if(found == NULL)
            real = 0;
        //if(found->SIN != validate_data[i]->SIN)
            //real = 0;
        else if(found->gender != validate_data[i]->gender)
            real = 0;
        else if(strcmp(found->first_name, validate_data[i]->first_name))
            real = 0;
        else if(strcmp(found->last_name, validate_data[i]->last_name))
            real = 0;
        else if(strcmp(found->passport_num, validate_data[i]->passport_num))
            real = 0;
        else if(strcmp(found->bank_acc_num, validate_data[i]->bank_acc_num))
            real = 0;
        else if(found->dob_day != validate_data[i]->dob_day)
            real = 0;
        else if(found->dob_month != validate_data[i]->dob_month)
            real = 0;
        else if(found->dob_year != validate_data[i]->dob_year)
            real = 0;
        
        
        if(!real) //Not in the table
        {
            //printf("%lu IS NOT IN TABLE\n", validate_data[i]->SIN);
            sprintf(sin, "%lu", validate_data[i]->SIN);
            fputs(sin, out);
            fputs("\n", out);
	}
	free(validate_data[i]);
    }
    //Free all the arrays
    for(long int i = 0; i < load_lines; ++i)
    {
	free(load_data[i]);
    }

    for(long int i = 0; i < update_lines; ++i)
    {
        free(update_data[i]);
    }
    free(validate_data);
    fclose(f_load);
    fclose(f_update);
    fclose(f_validate);
    fclose(out);
    //print_buckets(table);
    //print_status(table);
    //if(lookup_key(data->SIN, *table) != NULL)
    delete_table(table);
    free(load_data);
    free(update_data);

}


/*
void counter_intelligence2(char* load, char* update, char* validate, char* outfile)
{
    PersonalData** load_data = parse_data(load);
    int lines = 0;
    FILE* in1 = fopen(load, "r");
    HashTable* table = create_hash_table(0, 0);

    char val[256];
    char up[256];
    char lo[256];
    char current_line[256];
    current_line[0] = '\0';
    fgets(val, 256, in1);
    printf("Here1\n");
    while(fgets(current_line, 256, in1))
    {
        ++lines;
    }
    for(int i = 0; i < lines; ++i)
    {
        print_personal_data(load_data[i]);
        printf("\n");
        update_key(load_data[i], &table);
    }


    PersonalData** update_data = parse_data(update);
    lines = 0;
    FILE* in2 = fopen(update, "r");
    printf("Here2-\n");
    while(fgets(current_line, 256, in2))
    {
        ++lines;
    }
    for(int i = 0; i < lines; ++i)
    {   
        print_personal_data(update_data[i]);
        printf("\n");
        update_key(update_data[i], &table);
    }

    FILE* f_validate = fopen(validate,"r");
    PersonalData** p_validate = parse_data(validate);
    while(fgets(current_line, 256, f_validate))
    {
        ++lines;
    }
    PersonalData* found;
    

    FILE* out = fopen(outfile, "w");
    
    for(int i = 0; i < lines; ++i)
    {
        int real = 1;
        found = lookup_key(validate[i], table);
        
        if(found->SIN != p_validate[i]->SIN)
            real = 0;
        if(found->gender == p_validate[i]->gender)
            real = 0;
        if(strcmp(found->first_name, p_validate[i]->first_name)
            real = 0;
        if(strcmp(found->last_name, p_validate[i]->last_name)
            real = 0;
        if(strcmp(found->passport_num, p_validate[i]->passport_num)
            real = 0;
        if(strcmp(found->bank_acc_num, p_validate[i]->bank_acc_num)
            real = 0;
        if(found->dob_day != p_validate[i]->dob_day)
            real = 0;
        if(found->dob_month != p_validate[i]->dob_month)
            real = 0;
        if(found->dob_year != p_validate[i]->dob_year)
            real = 0;
        
        if(real == 0)
        {
            char* fake_sin = strtok(val, "\t");
            fputs(fake_sin, out);
            fputs("\n", out);
        }
    }
}
*/
