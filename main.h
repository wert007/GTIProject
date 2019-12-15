
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

char_array2d *create_empty_char_array2d(unsigned int width, unsigned int height);

unsigned int count_ones(char_array component);
void compare(unsigned int ones, list *current, list *next, list *, list *, list *new_meta_list);
bool is_off_by_one_bit(char_array *currentComponent, char_array *nextComponent);
char_array *combine_components(char_array *currentComponent, char_array *nextComponent);

bool a_can_be_converted_to_b(char_array *a, char_array *b);
void remove_column(list *meta_table, int index);
void remove_row(list *meta_table, int index);
void remove_submissive_rows(list * meta_table); //( ͡° ͜ʖ ͡°)
void remove_dominant_columns(list * meta_table); //( ͡° ͜ʖ ͡°)

bool is_meta_table_empty(list * meta_table);

void collect_essentials(list *meta_table, list *result, list *primeimplicants);

list *convert_to_table(list *primeimplicants, char_array2d *minterms);
void add_to_meta_list_at(unsigned int index, list *base, char_array *data);

void foo(char_array2d *args);
void do_the_phase_ONE(list *meta_list, list *);
list * do_the_phase_DOS(list *l, char_array2d *minterms);
void parse_args(int argc, char **argv, char_array2d **values);

void print_char_array(char_array *arr);
void print_map(list *);
void print_map_debug(list *results);
