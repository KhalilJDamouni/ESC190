#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lab3_calc.h"
#include "lab3_stack.h"



double compute_rpn(char* rpn)
{
	int size = 0;
	while(*(rpn + size) != '\0')
	{
		size += 1;
	}
	int tokens = 0;
	for(int i = 0; i < size; ++i)
	{
		if(*(rpn + i) == ' ')
			++tokens;
	}
	tokens += 1;
//	char array[size];
//	for(int i = 0; i < size; ++i)
//	{
//		array[i] = *(rpn + i);
//	}
	int current_token = 0;
//	//char* token = strtok(array," ");
	char* token = strtok(rpn," ");

	struct stack* values = create_stack();

	while(current_token < tokens)
	{
		current_token += 1;
		if(unary_op(token[0]))
		{
			char current_op = token[0];
			float num1 = pop(values);
			
			if(current_op == 's')
				num1 = sin(num1);
			if(current_op == 'c')
				num1 = cos(num1);
			if(current_op == 't')
				num1 = tan(num1);
			if(current_op == 'e')
				num1 = exp(num1);
			if(current_op == 'i')
				num1 = 1 / (num1);
			if(current_op == 'm')
				num1 = -1 * (num1);
			if(current_op == 'r')
				num1 = sqrt(num1);
				
			push(values, num1);
		}

		else if(binary_op(token[0]))
		{
			char current_op = token[0];
			double num1 = pop(values);
			double num2 = pop(values);

			if(current_op == '+')
			{
				num1 += num2;
				push(values, num1);
			}
			else if(current_op == '-')
			{
				num1 -= num2;
				push(values, num1);
			}
			else if(current_op == '*')
			{
				num1 *= num2;
				push(values, num1);
			}
			else if(current_op == '/')
			{
				num1 = num2 / num1;
				push(values, num1);
			}
			else if(current_op == '^')
			{
				num1 = pow(num2,num1);
				push(values, num1);
			}
			else if(current_op == 'f')
			{
				push(values, num1);
				push(values, num2);
			}
		}
		else
		{
			char* start_digit = &token[0];
			double number = atof(start_digit);
			
			push(values, number);
		}
		token = strtok(NULL," ");
	}
	free(token);
	double return_value = pop(values);
	
	delete_stack(values);

	return return_value;
}

char* get_expressions(char* filename)
{
	char* file_input = filename;
	FILE *input_stream = fopen(file_input, "r"); 
	char* file_line = malloc(sizeof(char) * 256);
	char* all_lines = malloc((sizeof(char) * 256 * 256) + sizeof(char));
	*all_lines = '\0';
	while(fgets(file_line, sizeof(file_line), input_stream) != NULL)
	{
		
		strncat(all_lines, file_line, 256);
		//strcat(all_lines,file_line);
	}
	fclose(input_stream); 
	//char* start_of_arrary = all_lines;
	free(file_line);
	return all_lines;
}

void process_expressions(char* expressions, char* filename)
{
	char *file_output = filename;
	FILE *output_stream = fopen(file_output, "w"); 

	char* buffer = NULL;
	char* output = malloc(sizeof(char) * 256);
	*output = '\0';
	char* token = strtok_r(expressions, "\n", &buffer);
	while(token != NULL)
	{
		sprintf(output,"%f",compute_rpn(token));
		fputs(output, output_stream);
		fputs("\n", output_stream);

		token = strtok_r(NULL, "\n",&buffer);
	}
	free(output);
	//free(token);
	//free(buffer);
	//free(file_output);
	fclose(output_stream);
}
