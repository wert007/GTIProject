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

node * getAt(list * l, unsigned int i);
list * createEmptyList();
node * createNode(void * data);
void addToEnd(list * l, void * data);

void addToMetaListAt(unsigned int index, list * base, char_array * data);


int main(int argc, char ** argv){
	printf("Hallo Welt!");
	char_array2d* values;
	parseArgs(argc, argv, values); //TODO: wert007
	foo(values);
	return 0;
}



void foo(char_array2d* args)
{
	list *_list = malloc(sizeof(list));
	for(int i = 0; i < args->length; i++)
	{
		int ones = countOnes(args->data[i]);
		addToListOfListAt(ones, _list, &args->data[i]); //TODO wert007
	}
	_list = doThePhaseONE(_list);
	doThePhaseDOS(_list); //T O D O
}

list *doThePhaseONE(list *_list)
{
	bool success = false;
	list* newList = malloc(sizeof(list));
	for(int i = 0; i < _list->length - 1; i++)
	{
		//todo wert007
		//should work..
		list *current = getAt(_list, i)->data;
		list *next = getAt(_list, i + 1)->data;
		compare(i, current, next, &success, newList);
		
	}
	if(success)
	{
		doThePhaseONE(newList);
	}
	freeList(_list);
	return newList;
}

void compare(unsigned int ones, list *current, list *next, bool * success, list * newList)
{
	if(current->length == 0 || next->length == 0)
	{
		//TODO: How do we need to set success?
		return;
	}

	for(int i = 0; i < current->length; i++)
	{
		bool currentInsertedIntoList = false;
		char_array* currentComponent = getAt(current, i)->data;
		for(int j = 0; j < next->length; j++)
		{
			char_array* nextComponent = getAt(next, j)->data;
			//TODO ranplax
			if(OffByOneBit(currentComponent, nextComponent))
			{
				//TODO ranplax
				char_array* component = combineComponents(currentComponent, nextComponent);	

				addToMetaListAt(ones, newList, component);
				//current.data[i] 					[x]
				currentInsertedIntoList = true;
				*success = true;
			}
		}
		if(!currentInsertedIntoList)
		{
			addToMetaListAt(ones, newList, currentComponent);
		}
	}
}

void addToMetaListAt(unsigned int index, list * base, char_array * data)
{
	while(base->length < index)
	{
		addToEnd(base, createEmptyList());
	}
	list * selected = getAt(base, index)->data;
	addToEnd(selected, data);
}

unsigned int countOnes(char_array component)
{
	unsigned int result = 0;
	for(int i = 0; i < component.length; i++)
	{
		if(component.data[i] == 1)
			result++;
	}
	return result;
}

list * createEmptyList()
{
	list * result = malloc(sizeof(list*));
	result->length = 0;
	result->root = NULL;
	return result;
}

node * createNode(void * data)
{
	node * result = malloc(sizeof(node*));
	result->next = NULL;
	result->data = data;
	return result;
}

node * getAtNode(node * n, unsigned int i)
{
	if(i == 0)
		return n;
	return getAtNode(n->next, i - 1);
}

node * getAt(list * l, unsigned int i)
{
	return getAtNode(l->root, i);
}



void addToEndNode(node * n, void * data)
{
	if(n->next != NULL)
		addToEndNode(n->next, data);
	else
		n->next = createNode(data);
}

void addToEnd(list * l, void * data)
{
	if(l->root == NULL)
		l->root = createNode(data);
	else
		addToEndNode(l->root, data);
	l->length = l->length + 1;
}