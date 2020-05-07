#include <stdio.h>
#include <stdlib.h>
#include "lab3_stack.h"


struct stack* create_stack(void)
{	
	struct stack* temp = malloc(sizeof(struct stack));
	if(temp == NULL)
		return NULL;
	else
	{
		temp->size = 0;
		temp->top  = NULL;
		return temp;
	}
}

void delete_stack(struct stack* s)
{
	if(s == NULL)
	{
		free(s);
		return;
	}
	while(s->top != NULL)
	{
		struct stack_entry* new_top = s->top->next;
		free(s->top);
		s->top = new_top;
		free(new_top);
	}
	free(s);
}

double pop(struct stack* s)
{
	if(s == NULL)
	{
		return 0;
	}
	
	else if(s->top == NULL)
	{
		free(s->top);
		s->size -= 1;
		return 0;
	}
	else if(s->top->next == NULL)
	{
		float return_value = s->top->value;
		free(s->top);
		s->top = NULL;
		s->size -= 1;
		return return_value;
	}
	else
	{
		float return_value = s->top->value;
		struct stack_entry* new_top;// = malloc(sizeof(struct stack_entry));
		new_top = s->top->next;
		free(s->top);
		s->top = new_top;
		s->size -= 1;
		return return_value;
	}
}

int push(struct stack* s, double e)
{
	struct stack_entry* new_entry = malloc(sizeof(struct stack_entry));
	
	if(new_entry == NULL)
	{
		return -1;
	}
	
	else
	{
		new_entry->value = e;
		new_entry->next = NULL;

		if(s->top == NULL)
		{
			s->top = new_entry;
			s->size += 1;
		}
		else
		{
			new_entry->next = s->top;
			s->top = new_entry;
			s->size += 1;
		}
		return 0;
	}
}
/*
void display_stack(struct stack* s)
{
	struct stack_entry* entry = malloc(sizeof(struct stack_entry));
	entry = (s->top);
	
	while(entry)
	{
		entry = entry->next;
	}
	return;
}*/
