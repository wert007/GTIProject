//lol
#include "main.h"

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
	list *meta_list = create_empty_list();
	list *result_list = create_empty_list();
	for (int i = 0; i < args->length; i++)
	{
		int ones = count_ones(*args->data[i]);
		add_to_meta_list_at(ones, meta_list, args->data[i]);
	}
	do_the_phase_ONE(meta_list, result_list);

	list *r = do_the_phase_DOS(result_list, args);
	print_map(r);
}

void do_the_phase_ONE(list *meta_list, list *result_list)
{
	//TODO: Is this if ever true?
	if (meta_list->length == 0)
		return;
	//Reset Loop
	for (int i = 0; i < meta_list->length; i++)
	{
		list *current_list = get_at(meta_list, i);
		for (int j = 0; j < current_list->length; j++)
		{
			char_array *currentComp = get_at(current_list, j);
			currentComp->has_been_compared = false;
		}
	}
	list *new_meta_list = create_empty_list();

	//Main Loop, compares the elemts with each other
	for (int i = 0; i < meta_list->length - 1; i++)
	{
		list *current = get_at(meta_list, i);
		list *next = get_at(meta_list, i + 1);
		compare(current, next, meta_list, new_meta_list);
	}
	//checks for last element if it was compared
	list *current = get_at(meta_list, (meta_list->length - 1));
	compare(current, create_empty_list(), meta_list, new_meta_list);

	//Loop to check, if at least one element has been compared
	for (int i = 0; i < meta_list->length; i++)
	{
		list *comp_list = get_at(meta_list, i);
		for (int j = 0; j < comp_list->length; j++)
		{
			char_array *comp_arr = get_at(comp_list, j);
			if (comp_arr->has_been_compared)
			{
				wrap_it_up(false, new_meta_list, result_list);
				return;
			}
		}
	}
	wrap_it_up(true, new_meta_list, result_list);
}

void wrap_it_up(bool is_done, list * new_meta_list, list * result_list)
{
	if(is_done)	//We didn't compare anything with eachother, so we can stop now
	{
		for (int i = 0; i < new_meta_list->length; i++)
		{
			list *current = get_at(new_meta_list, i);
			for (int j = 0; j < current->length; j++)
				add_to_end(result_list, get_at(current, j));
		}
		//Removing possible duplicates, so we have a cleaner list to work on
		remove_duplicates(result_list);
	}
	else
		do_the_phase_ONE(new_meta_list, result_list);
}

bool is_meta_table_empty(list *meta_table)
{
	if (meta_table->length == 0)
		return true;
	for (int i = 0; i < meta_table->length; i++)
	{
		list *current = get_at(meta_table, i);
		if (current->length > 0)
			return false;
	}
	return true;
}

void print_meta_table(list *meta_table)
{
	for (int y = 0; y < meta_table->length; y++)
	{
		list *current = get_at(meta_table, y);
		for (int x = 0; x < current->length; x++)
		{
			char val = *(char *)get_at(current, x);
			printf("%d", val);
		}
		printf("\n");
	}
}

list *do_the_phase_DOS(list *l, char_array2d *minterms)
{
	list *meta_table = convert_to_table(l, minterms);

	list *result = create_empty_list();
	int meta_table_length = -1;
	while (!is_meta_table_empty(meta_table))
	{
		meta_table_length = meta_table->length;
		//Collect essential implicants and transform table
		collect_essentials(meta_table, result, l);
		remove_unimportant_rows_and_columns(meta_table);
		//If we didn't find any essential primimplicants
		//Remove just any element.
		if (meta_table_length == meta_table->length)
		{
			do_something_random_xD(meta_table, result, l);
			remove_unimportant_rows_and_columns(meta_table);
		}
	}
	return result;
}

void remove_unimportant_rows_and_columns(list * meta_table)
{
	if(is_meta_table_empty(meta_table))
		return;
	remove_submissive_rows(meta_table);
	if(is_meta_table_empty(meta_table))
		return;
	remove_dominant_columns(meta_table);
}

void do_something_random_xD(list *meta_table, list *result, list *primeimplicants)
{
	//TODO: Remove the one primeimplicant, which
	//has the most ones.
	char_array *l = get_at(primeimplicants, 0);
	remove_at(primeimplicants, 0);
	add_to_end(result, l);
	remove_at(meta_table, 0);
	list * cur = get_at(meta_table, 0);
	for (int i = cur->length - 1; i >= 0; i--)
	{
		char val = *(char*)get_at(cur, i);
		if (val != 0)
			remove_column(meta_table, i);
	}
}

void remove_dominant_columns(list *meta_table) //(  ͡°  ͜ʖ  ͡° )
{
	list *current = get_at(meta_table, 0);
	for (int x = current->length - 1; x >= 0; x--)
	{
		for (int x2 = current->length - 1; x2 >= 0; x2--)
		{
			if (x == x2)
				continue;
			bool should_be_removed = true;
			for (int y = meta_table->length - 1; y >= 0; y--)
			{
				current = get_at(meta_table, y);
				char curVal = *(char *)get_at(current, x);
				char othVal = *(char *)get_at(current, x2);
				if (curVal == 0 && othVal == 1)
				{
					should_be_removed = false;
				}
			}
			if (should_be_removed)
			{
				remove_column(meta_table, x);
				break;
			}
		}
	}
	for(int i = meta_table->length - 1; i >= 0; i--)
	{
		list * cur = get_at(meta_table, i);
		bool found_one = false;
		for(int j = 0; j < cur->length; j++)
		{
			char val = *(char*)get_at(cur, j);
			if(val == 1)
				found_one = true;
		}
		if(!found_one)
		{
			remove_at(meta_table, i);
		}
	}
}

void remove_submissive_rows(list *meta_table) //( ͡° ͜ʖ ͡°)
{
	for (int i = meta_table->length - 1; i >= 0; i--)
	{
		list *current = get_at(meta_table, i);
		for (int j = meta_table->length - 1; j >= 0; j--)
		{
			if (j == i)
				continue;
			list *other = get_at(meta_table, j);
			bool should_be_removed = true;
			for (int k = 0; k < current->length; k++)
			{
				char curElem = *(char *)get_at(current, k);
				char othElem = *(char *)get_at(other, k);
				if (curElem == 1 && othElem == 0)
					should_be_removed = false;
			}
			if (should_be_removed)
			{
				//Remove current row.
				remove_at(meta_table, i);
				break;
			}
		}
	}
}

//TODO: Are we allowed to change primeimplicnats? Would be better if we could.
void collect_essentials(list *meta_table, list *result, list *primeimplicants)
{
	if (meta_table->length == 1)
	{
		list *l = get_at(primeimplicants, 0);
		remove_at(primeimplicants, 0);
		add_to_end(result, l);
		remove_at(meta_table, 0);
		return;
	}
	list *current = get_at(meta_table, 0);
	for (int x = current->length - 1; x >= 0; x--)
	{
		int ones_in_row = 0;
		if (x > current->length - 1)
			x = current->length - 1;
		int index_of_last_found = -1;
		for (int y = 0; y < meta_table->length; y++)
		{
			current = get_at(meta_table, y);
			char val = *(char *)(get_at(current, x));
			if (val == 1)
			{

				ones_in_row++;
				index_of_last_found = y;
			}
		}

		if (ones_in_row == 1)
		{
			list *l = get_at(primeimplicants, index_of_last_found);
			remove_at(primeimplicants, index_of_last_found);
			add_to_end(result, l);
			list *row = get_at(meta_table, index_of_last_found);
			for (int i = row->length - 1; i >= 0; i--)
			{
				char value = *(char *)get_at(row, i);
				if (value == 1)
				{
					remove_column(meta_table, i);
				}
			}
			remove_row(meta_table, index_of_last_found);

			if (is_meta_table_empty(meta_table))
				return;
			current = get_at(meta_table, 0);
		}
	}
}

void remove_row(list *meta_table, int index)
{
	remove_at(meta_table, index);
}

void remove_column(list *meta_table, int index)
{
	for (int i = 0; i < meta_table->length; i++)
	{
		list *current = get_at(meta_table, i);
		remove_at(current, index);
	}
}

list *convert_to_table(list *primeimplicants, char_array2d *minterms)
{
	list *result = create_empty_list();
	for (int y = 0; y < primeimplicants->length; y++)
	{
		list *current = create_empty_list();
		add_to_end(result, current);
		for (int x = 0; x < minterms->length; x++)
		{
			char_array *b = get_at(primeimplicants, y);
			char_array *a = minterms->data[x];

			char *value = malloc(sizeof(char));
			value[0] = a_can_be_converted_to_b(a, b);
			add_to_end(current, value);
		}
	}
	return result;
}

bool a_can_be_converted_to_b(char_array *a, char_array *b)
{
	if (a->length != b->length)
		return false;
	for (int i = 0; i < a->length; i++)
	{
		switch (a->data[i])
		{
		case 0:
			if (b->data[i] == 1)
				return false;
			break;
		case 1:
			if (b->data[i] == 0)
				return false;
			break;
		}
	}
	return true;
}

void compare(list *current, list *next, list *meta_list, list *new_meta_list)
{
	//Main Loop to compare elements
	for (int i = 0; i < current->length; i++)
	{
		char_array *current_component = get_at(current, i);
		for (int j = 0; j < next->length; j++)
		{
			char_array *next_component = get_at(next, j);

			//if there is only one digit differnt
			if (is_off_by_one_bit(current_component, next_component))
			{
				char_array *component = combine_components(current_component, next_component);

				//add the combined elemnts to new list
				int newIndex = count_ones(*component);
				add_to_meta_list_at(newIndex, new_meta_list, component);
			}
		}
	}
	//if the current element is not comparable, add it directly to the final list
	for (int i = 0; i < current->length; i++)
	{
		char_array *current_component = get_at(current, i);
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
		if ((current_component->data[i] == 2) && (next_component->data[i] != 2))
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

bool equals(char_array *a, char_array *b)
{
	if (a->length != b->length)
		return false;
	for (int i = 0; i < a->length; i++)
	{
		if (a->data[i] != b->data[i])
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

char_array2d *create_empty_char_array2d(unsigned int width, unsigned int height)
{
	char_array2d *result = malloc(sizeof(char_array2d));
	result->length = height;
	result->data = malloc(sizeof(char_array *) * height);
	for (int i = 0; i < height; i++)
	{
		result->data[i] = create_empty_char_array(width);
	}
	return result;
}

char_array *combine_components(char_array *current_component, char_array *next_component)
{
	current_component->has_been_compared = true;
	next_component->has_been_compared = true;
	char_array *to_be_added = create_empty_char_array(current_component->length);
	for (int i = 0; i < current_component->length; i++)
	{
		if (current_component->data[i] != next_component->data[i])
			to_be_added->data[i] = 2;
		else
			to_be_added->data[i] = current_component->data[i];
	}
	return to_be_added;
}

void add_to_meta_list_at(unsigned int index, list *base, char_array *data)
{
	while (base->length <= index)
	{
		add_to_end(base, create_empty_list());
	}
	list *selected = get_at(base, index);
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

void *get_at(list *l, unsigned int i)
{
	return get_at_node(l->root, i)->data;
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

node *remove_at_node(node *n, unsigned int i)
{
	if (i == 0)
	{
		return n->next;
	}
	else
	{
		n->next = remove_at_node(n->next, i - 1);
		return n;
	}
}

void remove_at(list *l, unsigned int i)
{
	l->root = remove_at_node(l->root, i);
	//Could go horrible wrong if we actually didn't delete anything.
	if (l->length - 1 < 0)
		l->length = 0;
	else
		l->length = l->length - 1;
}

void remove_duplicates(list *l)
{
	for (int i = l->length - 2; i >= 0; i--)
	{
		char_array *current = get_at(l, i);
		for (int j = l->length - 1; j > i; j--)
		{
			char_array *other = get_at(l, j);
			if (equals(current, other) && i != j){
				remove_at(l, j);
			}

		}
	}
}

void print_map(list *results)
{
	int dont_care = 0;
	for (int i = 0; i < results->length; i++)
	{
		char_array *current_arr = get_at(results, i);
		int count = 0;
		for (int j = 0; j < current_arr->length; j++)
		{
			int v = current_arr->data[j];
			if (v == 1)
				printf("%c", j + 'A');
			else if (v == 0)
				printf("%c", j + 'a');
			else
				count += 1;
		}
		if (count != current_arr->length)
			printf("\n");
		else
			dont_care = 1;
	}
	if (dont_care != 0)
		printf("Don't care situation: Always on!\n");
}