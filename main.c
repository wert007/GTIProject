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
	char_array data;
} node;

typedef struct {
	node* root;
	unsigned int length;
} char_array_list;

int main(int argc, char ** argv){
	printf("Hallo Welt!");
	char_array2d* values;
	parseArgs(argc, argv, values);
	foo(values);
	return 0;
}

void foo(char_array2d* args)
{
	listOfLists _list;
	for(int i = 0; i < args->length; i++)
	{
		int ones = countOnes(args->data[i]);
		addToListOfListAt(ones, _list, args->data[i]);
	}
	subFoo(_list);
}

void subFoo(listOfLists _list)
{
	bool success = false;
	listOfLists newList;
	for(int i = 0; i < _list.length - 1; i++)
	{
		list current = _list.data[i];
		list next = _list.data[i + 1];
		compare(current, next, &success, &newList);
		
	}
	if(success)
	{
		subFoo(newList);
	}
}

void compare(list current, list next, bool * success, listOfLists * newList)
{
	if(current.length == 0 || next.length == 0) return;
	
	for(int i = 0; i < current.length; i++)
	{
		bool currentInsertedIntoList = false;
		for(int j = 0; j < next.length; j++)
		{
			if(OffByOneBit(current.data[i], next.data[j]))
			{
				//current.data[i] 					[x]
				char_array* component = combineComponents(current.data[i], next.data[j]);	

				int ones = countOnes(component);
				addToListOfListAt(ones, newList, component);
				currentInsertedIntoList = true;
				*success = true;
			}
		}
		if(!currentInsertedIntoList)
		{
			//TODO: We dont need to do that. Because that doesnt change
			int ones = countOnes(current.data[i]);
			addToListOfListAt(ones, newList, current.data[i]);
		}
	}
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