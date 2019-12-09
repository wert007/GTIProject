#include <stdio.h>
#include <stdbool.h>

typedef struct {
	unsigned int length;
	char* data; //KEIN STRING. ZAHLEN!
} char_array;

typedef struct {
	unsigned int length;
	char_array* data; //KEIN STRING. ZAHLEN!
} char_array2d;

typedef struct node {
	struct node * next;
	void * data;
} node;

typedef struct {
	node* root;
	unsigned int length;
} list;

node * get_at(list * l, unsigned int i);
list * create_empty_list();
node * create_node(void * data);
void add_to_end(list * l, void * data);

void add_to_meta_list_at(unsigned int index, list * base, char_array * data);

void foo(char_array2d* args);
list *do_the_phase_ONE(list *meta_list);
void do_the_phase_DOS(list* meta_list); //TODO


int main(int argc, char ** argv){
	printf("Hallo Welt!");
	char_array2d* values;
	parse_args(argc, argv, &values); //TODO: wert007
	foo(values);
	return 0;
}

void parse_args(int argc, char ** argv, char_array2d** values)
{
	int length = argc - 1;
	*values = malloc(sizeof(char_array2d));
	(*values)->length = length;
	(*values)->data = malloc(length * sizeof(char_array));
	for(int i = 0; i < length; i++)
	{
		char_array* cur =malloc(sizeof(char_array));
		cur->length = strlen(argv[i]);
		cur->data = calloc(length, sizeof(char));
		for(int c = 0; c < cur->length; c++)
		{
			switch(argv[i][c])
			{
				//TODO: Use aB-Cde
				case '1':
					cur->data[i] = 1;
					break;
				case '0':
					cur->data[i] = 0;
					break;
				case '-':
					cur->data[i] = 3;
					break;
			}
		}
	}
}

void foo(char_array2d* args)
{
	list *meta_list = malloc(sizeof(list));
	for(int i = 0; i < args->length; i++)
	{
		int ones = count_ones(args->data[i]);
		add_to_meta_list_at(ones, meta_list, &args->data[i]); //TODO wert007
	}
	meta_list = do_the_phase_ONE(meta_list);
	do_the_phase_DOS(meta_list); //T O D O
}

list *do_the_phase_ONE(list *meta_list)
{
	bool success = false;
	list* new_meta_list = malloc(sizeof(list));
	for(int i = 0; i < meta_list->length - 1; i++)
	{
		//todo wert007
		//should work..
		list *current = get_at(meta_list, i)->data;
		list *next = get_at(meta_list, i + 1)->data;
		compare(i, current, next, &success, new_meta_list);
		
	}
	if(success)
	{
		do_the_phase_ONE(new_meta_list);
	}
	free_list(meta_list);
	return new_meta_list;
}

void compare(unsigned int ones, list *current, list *next, bool * success, list * new_meta_list)
{
	if(current->length == 0 || next->length == 0)
	{
		//TODO: How do we need to set success?
		return;
	}

	for(int i = 0; i < current->length; i++)
	{
		bool is_current_inserted_into_list = false;
		char_array* current_component = get_at(current, i)->data;
		for(int j = 0; j < next->length; j++)
		{
			char_array* next_component = get_at(next, j)->data;
			//TODO ranplax
			if(is_off_by_one_bit(current_component, next_component))
			{
				//TODO ranplax
				char_array* component = combine_components(current_component, next_component);	

				add_to_meta_list_at(ones, new_meta_list, component);
				//current.data[i] 					[x]
				is_current_inserted_into_list = true;
				*success = true;
			}
		}
		if(!is_current_inserted_into_list)
		{
			add_to_meta_list_at(ones, new_meta_list, current_component);
		}
	}
}

void add_to_meta_list_at(unsigned int index, list * base, char_array * data)
{
	while(base->length < index)
	{
		add_to_end(base, create_empty_list());
	}
	list * selected = get_at(base, index)->data;
	add_to_end(selected, data);
}

unsigned int count_ones(char_array component)
{
	unsigned int result = 0;
	for(int i = 0; i < component.length; i++)
	{
		if(component.data[i] == 1)
			result++;
	}
	return result;
}

list * create_empty_list()
{
	list * result = malloc(sizeof(list*));
	result->length = 0;
	result->root = NULL;
	return result;
}

node * create_node(void * data)
{
	node * result = malloc(sizeof(node*));
	result->next = NULL;
	result->data = data;
	return result;
}

node * get_at_node(node * n, unsigned int i)
{
	if(i == 0)
		return n;
	return get_at_node(n->next, i - 1);
}

node * get_at(list * l, unsigned int i)
{
	return get_at_node(l->root, i);
}

void add_to_end_node(node * n, void * data)
{
	if(n->next != NULL)
		add_to_end_node(n->next, data);
	else
		n->next = create_node(data);
}

void add_to_end(list * l, void * data)
{
	if(l->root == NULL)
		l->root = create_node(data);
	else
		add_to_end_node(l->root, data);
	l->length = l->length + 1;
}