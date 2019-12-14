//lol

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	unsigned int length;
	bool has_been_compared;
	char *data; //KEIN STRING. ZAHLEN!
} char_array;

typedef struct
{
	unsigned int length;
	char_array **data; //KEIN STRING. ZAHLEN!
} char_array2d;

typedef struct node
{
	struct node *next;
	void *data;
} node;

typedef struct
{
	node *root;
	unsigned int length;
} list;

node *get_at(list *l, unsigned int i);
list *create_empty_list();
node *create_node(void *data);
void free_list(list *l);
void free_meta_list(list *meta_list);
void add_to_end(list *l, void *data);
void remove_at(list *l, unsigned int i);
void remove_duplicates(list *l);
void remove_duplicates_meta(list *l);

unsigned int count_ones(char_array component);
void compare(unsigned int ones, list *current, list *next, list *, list *, list *new_meta_list);
bool is_off_by_one_bit(char_array *currentComponent, char_array *nextComponent);
char_array *combine_components(char_array *currentComponent, char_array *nextComponent);

void add_to_meta_list_at(unsigned int index, list *base, char_array *data);

void foo(char_array2d *args);
void do_the_phase_ONE(list *meta_list, list *);
void do_the_phase_DOS(list *meta_list); //TODO
void parse_args(int argc, char **argv, char_array2d **values);

void print_char_array(char_array *arr);
void print_map(list *);
void print_map_debug(list *results);

int main(int argc, char **argv)
{
	char_array2d *values;
	parse_args(argc, argv, &values); //TODO: wert007
	foo(values);
	return 0;
}

void parse_args(int argc, char **argv, char_array2d **values)
{
	int length = argc - 1;
	*values = malloc(sizeof(char_array2d));
	(*values)->length = length;
	(*values)->data = malloc(length * sizeof(char_array *));
	for (int i = 0; i < length; i++)
	{
		char_array *cur = malloc(sizeof(char_array));
		cur->length = strlen(argv[i + 1]);
		cur->has_been_compared = false;
		cur->data = calloc(length, sizeof(char));
		for (int c = 0; c < cur->length; c++)
		{
			switch (argv[i + 1][c])
			{
			//TODO: Use aBCde
			case '1':
				cur->data[c] = 1;
				break;
			case '0':
				cur->data[c] = 0;
				break;
			case '-':
				cur->data[c] = 2;
				break;
			default:
				printf(">%d<", argv[i + 1][c]);
				break;
			}
		}
		(*values)->data[i] = cur;
	}
}

void foo(char_array2d *args)
{
	list *meta_list = create_empty_list(); // malloc(sizeof(list));
	list *result_list = create_empty_list();
	for (int i = 0; i < args->length; i++)
	{
		int ones = count_ones(*args->data[i]);
		add_to_meta_list_at(ones, meta_list, args->data[i]); //TODO wert007
	}
	do_the_phase_ONE(meta_list, result_list);
	print_map(result_list);
	//print_map_debug(result_list);

	//do_the_phase_DOS(meta_list); //T O D O
}

void do_the_phase_ONE(list *meta_list, list *result_list)
{
	if (meta_list->length == 0)
		return;
	//Reset Loop
	for (int i = 0; i < meta_list->length; i++)
	{
		list *current_list = get_at(meta_list, i)->data;
		for (int j = 0; j < current_list->length; j++)
		{
			char_array *currentComp = get_at(current_list, j)->data;
			currentComp->has_been_compared = false;
		}
	}
	printf("\n---\n\n");
	bool success = false;
	list *new_meta_list = create_empty_list();

	//Main Loop, compares the elemts with each other
	for (int i = 0; i < meta_list->length -1; i++)
	{
		list *current = get_at(meta_list, i)->data;
		list *next = get_at(meta_list, i + 1)->data;
		compare(i, current, next, result_list, meta_list, new_meta_list);
		remove_duplicates_meta(new_meta_list);
	}
	//checks for last element if it was compared
	list *current = get_at(meta_list, (meta_list->length - 1))->data;
	compare(meta_list->length, current, create_empty_list(), result_list, meta_list, new_meta_list);
	remove_duplicates_meta(new_meta_list);


	printf("lol\n");
	//Loop to check, if at least one element has been compared
	for (int i = 0; i < meta_list->length; i++)
	{
		list *comp_list = get_at(meta_list, i)->data;
		for (int j = 0; j < comp_list->length; j++)
		{
			char_array *comp_arr = get_at(comp_list, j)->data;
			if (comp_arr->has_been_compared)
			{
				success = true;
				goto dontdothis;
			}
		}
	}
dontdothis:
	//if so, run it again
	if (success)
	{
		do_the_phase_ONE(new_meta_list, result_list);
	}else
	{
		for(int i = 0; i < new_meta_list->length; i++)
		{
			list* current = get_at(new_meta_list, i)->data;
			for(int j = 0; j < current->length; j++)
			{
				add_to_end(result_list, get_at(current, j)->data);
			}
		}
	}
}

void compare(unsigned int ones, list *current, list *next, list *result_list, list *meta_list, list *new_meta_list)
{
	//Main Loop to compare elements
	for (int i = 0; i < current->length; i++)
	{
		char_array *current_component = get_at(current, i)->data;
		for (int j = 0; j < next->length; j++)
		{
			char_array *next_component = get_at(next, j)->data;
			
			printf("Comparing ");
			print_char_array(current_component);
			printf(" with ");
			print_char_array(next_component);

			//if there is only one digit differnt
			if (is_off_by_one_bit(current_component, next_component))
			{
				current_component->has_been_compared = true;
				next_component->has_been_compared = true;
				char_array *component = combine_components(current_component, next_component);
				printf(": matched to ");
				print_char_array(component);
				//add the combined elemnts to new list
				int newIndex = count_ones(*component);
				add_to_meta_list_at(newIndex, new_meta_list, component);
			}
			printf("\n");
		}
	}
	//if the current element is not comparable, add it directly to the final list
	for (int i = 0; i < current->length; i++)
	{
		char_array *current_component = get_at(current, i)->data;
		if (!current_component->has_been_compared)
		{
			int index = count_ones(*current_component);
			add_to_meta_list_at(index, new_meta_list, current_component);
		}
	}
}

void print_char_array(char_array *arr)
{
	for (int c = 0; c < arr->length; c++)
	{
		printf("%d", arr->data[c]);
	}
}

bool is_off_by_one_bit(char_array *current_component, char_array *next_component)
{
	int count = 0;
	for (int i = 0; i < current_component->length; i++)
	{
		if((current_component->data[i] == 2) && (next_component->data[i] != 2))
			{
			count = 5;
			break;
			}
		else if (current_component->data[i] == next_component->data[i])
			continue;
		else
			count += 1;
	}
	if (count == 1)
		return true;
	else
		return false;
}

bool equals(char_array * a, char_array * b)
{
	if(a->length != b->length)
		return false;
	for(int i = 0; i < a->length; i++)
	{
		if(a->data[i] != b->data[i])
			return false;
	}
	return true;
}

char_array *create_empty_char_array(unsigned int length)
{
	char_array *result = malloc(sizeof(char_array));
	result->length = length;
	result->has_been_compared = false;
	result->data = malloc(length * sizeof(char));
	return result;
}

char_array *combine_components(char_array *currentComponent, char_array *nextComponent)
{
	char_array *to_be_added = create_empty_char_array(currentComponent->length);
	for (int i = 0; i < currentComponent->length; i++)
	{
		if (currentComponent->data[i] != nextComponent->data[i])
			to_be_added->data[i] = 2;
		else
			to_be_added->data[i] = currentComponent->data[i];
	}
	return to_be_added;
}

void add_to_meta_list_at(unsigned int index, list *base, char_array *data)
{
	while (base->length <= index)
	{
		add_to_end(base, create_empty_list());
	}
	list *selected = get_at(base, index)->data;
	add_to_end(selected, data);
}

unsigned int count_ones(char_array component)
{
	unsigned int result = 0;
	for (int i = 0; i < component.length; i++)
	{
		if (component.data[i] == 1)
			result++;
	}
	return result;
}

list *create_empty_list()
{
	list *result = malloc(sizeof(list *));
	result->length = 0;
	result->root = NULL;
	return result;
}

node *create_node(void *data)
{
	node *result = malloc(sizeof(node *));
	result->next = NULL;
	result->data = data;
	return result;
}

void free_meta_list_node(node *n)
{
	if (n->next != NULL)
		free_meta_list_node(n->next);
	free_list(n->data);
	free(n);
}

void free_meta_list(list *meta_list)
{
	if (meta_list->root != NULL)
		free_meta_list_node(meta_list->root);
	free(meta_list);
}

void free_list_node(node *n)
{
	if (n->next != NULL)
		free_list_node(n->next);
	free(n->data);
	free(n);
}

void free_list(list *l)
{
	if (l->root != NULL)
		free_list_node(l->root);
	free(l);
}

node *get_at_node(node *n, unsigned int i)
{
	if (i == 0)
		return n;
	if (n->next == NULL)
	{
		return NULL;
	}
	return get_at_node(n->next, i - 1);
}

node *get_at(list *l, unsigned int i)
{
	return get_at_node(l->root, i);
}

void add_to_end_node(node *n, void *data)
{
	if (n->next != NULL)
		add_to_end_node(n->next, data);
	else
		n->next = create_node(data);
}

void add_to_end(list *l, void *data)
{
	if (l->root == NULL)
		l->root = create_node(data);
	else
		add_to_end_node(l->root, data);
	l->length = l->length + 1;
}

node * remove_at_node(node * n, unsigned int i)
{
	if(i == 0)
	{
		return n->next;
	}
	else
	{
		if(n->next == NULL)
		{
			printf("ERROR: %d\n", i);
		}
		n->next = remove_at_node(n->next, i--);
		return n;
	}
}

void remove_at(list * l, unsigned int i)
{
	l->root = remove_at_node(l->root, i);
	//Could go horrible wrong if we actually didn't delete anything.
	if(l->length - 1 < 0)
		l->length = 0;
	else
		l->length = l->length - 1;
}

void remove_duplicates(list * l)
{
	for(int i = l->length - 2; i >= 0; i--)
	{
		char_array *current = get_at(l, i)->data;
		if(i >= l->length)
		{
			printf("What jsust happend??\n");
		}
		for(int j = l->length - 1; j > i; j--)
		{
			char_array *other = get_at(l, j)->data;
			if(equals(current, other) && i != j)
				remove_at(l, i);
		}
	}
}

void remove_duplicates_meta(list *l)
{
	for(int i = 0; i < l->length; i++)
	{
		list * current = get_at(l, i)->data;
		remove_duplicates(current);
	}

}

void print_map(list *results)
{
	int dont_care = 0;
	for (int i = 0; i < results->length; i++)
	{
		char_array *current_arr = get_at(results, i)->data;
		int count = 0;
		for (int j = 0; j < current_arr->length; j++)
		{
			int v = current_arr->data[j];
			if (v == 1)
				printf("%c", j + 'A');
			else if (v == 0)
				printf("%c", j + 'a');
			else count += 1;
		}
		if(count != current_arr->length)
			printf("\n");
		else dont_care = 1;
	}
	if(dont_care != 0)
		printf("it doesnt matter, always on :D\n");
}

void print_map_debug(list *results)
{
	for (int i = 0; i < results->length; i++)
	{
		char_array *current_arr = get_at(results, i)->data;
		for (int j = 0; j < current_arr->length; j++)
		{
			int v = current_arr->data[j];
			printf("%d", v);
		}
		printf("\n");
	}
}