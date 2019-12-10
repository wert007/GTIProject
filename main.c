#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	unsigned int length;
	char* data; //KEIN STRING. ZAHLEN!
} char_array;

typedef struct {
	unsigned int length;
	char_array** data; //KEIN STRING. ZAHLEN!
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
void free_list(list * l);
void free_meta_list(list * meta_list);
void add_to_end(list * l, void * data);

unsigned int count_ones(char_array component);
void compare(unsigned int ones, list *current, list *next, bool * success, list * new_meta_list);
bool is_off_by_one_bit(char_array* currentComponent, char_array* nextComponent);
char_array * combine_components(char_array * currentComponent, char_array * nextComponent);

void add_to_meta_list_at(unsigned int index, list * base, char_array * data);

void foo(char_array2d* args);
list *do_the_phase_ONE(list *meta_list);
void do_the_phase_DOS(list* meta_list); //TODO
void parse_args(int argc, char ** argv, char_array2d** values);



int main(int argc, char ** argv){
	printf("Hallo Welt!\n");
	char_array2d* values;
	parse_args(argc, argv, &values); //TODO: wert007
	printf("values = ");
	for(int x = 0; x < values->length; x++)
	{
		for(int c = 0; c < values->data[x]->length; c++)
		{
			printf("%c", values->data[x]->data[c] + '0');
		}
		printf(", ");
	}
	printf("\n");
	foo(values);
	return 0;
}

void parse_args(int argc, char ** argv, char_array2d** values)
{
	int length = argc - 1;
	*values = malloc(sizeof(char_array2d));
	(*values)->length = length;
	(*values)->data = malloc(length * sizeof(char_array*));
	for(int i = 0; i < length; i++)
	{
		char_array* cur = malloc(sizeof(char_array));
		cur->length = strlen(argv[i + 1]);
		cur->data = calloc(length, sizeof(char));
		for(int c = 0; c < cur->length; c++)
		{
			switch(argv[i + 1][c])
			{
				//TODO: Use aB-Cde
				case '1':
					cur->data[c] = 1;
					break;
				case '0':
					cur->data[c] = 0;
					break;
				case '-':
					cur->data[c] = 3;
					break;
				default:
					printf(">%d<", argv[i + 1][c]);
					break;
			}
		}
		(*values)->data[i] = cur;
	}
}

void foo(char_array2d* args)
{
	list *meta_list = malloc(sizeof(list));
	for(int i = 0; i < args->length; i++)
	{
		int ones = count_ones(*args->data[i]);
		printf("ones = %d\n", ones);
		add_to_meta_list_at(ones, meta_list, args->data[i]); //TODO wert007
	}
	meta_list = do_the_phase_ONE(meta_list);
	//do_the_phase_DOS(meta_list); //T O D O
}

list *do_the_phase_ONE(list *meta_list)
{
	bool success = false;
	list* new_meta_list = malloc(sizeof(list));
	for(int i = 0; i < meta_list->length - 1; i++)
	{
		//todo wert007
		list *current = get_at(meta_list, i)->data;
		list *next = get_at(meta_list, i + 1)->data;

		printf("current->length = %d\n", current->length);
		printf("next->length = %d\n", next->length);
		printf("meta_list->length = %d\n", meta_list->length);
		compare(i, current, next, &success, new_meta_list);
	}
	free_meta_list(meta_list);
	if(success)
	{
		do_the_phase_ONE(new_meta_list);
	}
	return new_meta_list;
}

void compare(unsigned int ones, list *current, list *next, bool * success, list * new_meta_list)
{
	if(current->length == 0 || next->length == 0)
	{
		//TODO: How do we need to set success here? ?_?
		return;
	}

	for(int i = 0; i < current->length; i++)
	{
		//TODO: Use something smarter. o-o^
		bool is_current_inserted_into_list = false;
		char_array* current_component = get_at(current, i)->data;
		for(int j = 0; j < next->length; j++)
		{
			char_array* next_component = get_at(next, j)->data;
			printf("i = %d\n", i);
			printf("j = %d\n", j);
			printf("current_component->length = %d\n", current_component->length);
			printf("next_component->length = %d\n", next_component->length);
			//TODO: Here we have segmentation faults. >.<"
			if(is_off_by_one_bit(current_component, next_component) && true)
			{
				char_array* component = combine_components(current_component, next_component);	

				add_to_meta_list_at(ones, new_meta_list, component);
				//current.data[i] 					[x]
				is_current_inserted_into_list = true;
				*success = true;
			}
			//Maybe til here, but i'm not too sure >^.^<

		}
		if(!is_current_inserted_into_list)
		{
			add_to_meta_list_at(ones, new_meta_list, current_component);
		}
	}
}

bool is_off_by_one_bit(char_array* currentComponent, char_array* nextComponent)
{
	int count = 0;
	for(int i = 0;i < currentComponent->length;i++){
		if(currentComponent->data[i] == nextComponent->data[i]) continue;
		count+=1;
	}
	if(count == 1) return true;
	else return false;
}

char_array * create_empty_char_array(unsigned int length)
{
	char_array * result = malloc(sizeof(char_array));
	printf("length = %d\n", length);
	result->length = length;
	result->data = malloc(length * sizeof(char));
	return result;
}

char_array * combine_components(char_array * currentComponent, char_array * nextComponent)
{
	char_array * to_be_added = create_empty_char_array(currentComponent->length);
	
	for(int i = 0;i<currentComponent->length;i++){
		if(currentComponent->data[i] != nextComponent->data[i])
			to_be_added->data[i] = 2;
		else 
			to_be_added->data[i] = currentComponent->data[i];
	}
	return to_be_added;
}

void add_to_meta_list_at(unsigned int index, list * base, char_array * data)
{
	while(base->length <= index)
	{
		printf("base->length = %d\n", base->length);
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

void free_meta_list_node(node * n)
{
	if(n->next != NULL)
		free_meta_list_node(n->next);
	free_list(n->data);
	free(n);
}

void free_meta_list(list * meta_list)
{
	if(meta_list->root != NULL)
		free_meta_list_node(meta_list->root);
	free(meta_list);
}

void free_list_node(node * n)
{
	if(n->next != NULL)
		free_list_node(n->next);
	free(n->data);
	free(n);
}

void free_list(list * l)
{
	if(l->root != NULL)
		free_list_node(l->root);
	free(l);
}

node * get_at_node(node * n, unsigned int i)
{
	if(i == 0)
		return n;
	if(n->next == NULL)
	{
		printf("ERROR: Out ouf bounds exception: index was %d to large.", i);
		return NULL;
	}
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

//Since the segfault is in the 0th element
//it could be something with lists?
//and how we add our root element???
//but i really don't know...
void add_to_end(list * l, void * data)
{
	if(l->root == NULL)
		l->root = create_node(data);
	else
		add_to_end_node(l->root, data);
	l->length = l->length + 1;
}

void print_map(list * results){
	for(int i = 0; i < results->length; i++)
	{
		char_array *current = get_at(results, i)->data;
		for(int j = 0;;j++)
		{
			if(current->data[j] == 1) printf("%c",(j+'a'));
			if(current->data[j] == 0) printf("%c",(j+'A'));
		}
		if((i+1)<results->length) printf(" or ");
		else printf("\n");
	}
}
//to be continued :D
/*
list * prime_implicant(list * to_check){
	for(int i = 0;i<to_check->length;i++)
	{
		char_array * current = get_at(to_check,i)->data;
		for(int j = 0;j<to_check->length;j++)
		{
			if(j!=i){
			char_array * comp_array = get_at(to_check,j);
			for(int k = 0;k<comp_array.length;k++)
			{
				if(current->data[k] == comp_array->data[k]){

				}
			}
		}
	}
	return;
}
*/