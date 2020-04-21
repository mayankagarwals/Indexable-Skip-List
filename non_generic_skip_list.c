#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX_LEVELS 6


typedef struct node{
	int val;				
	int level;
	struct node** next;		//array of pointers to nodes. 
}node;

typedef struct sklist{
	node* header;					//link to header node. 
	int level;						//top most level in the whole skip list (0 indexed)
}sklist;

void init(sklist* list)
{
	list -> header = (node*)malloc(sizeof(node));		//header node
	list -> level = 0;									//we start off with just one level
	list -> header -> val = INT_MAX;
	list -> header -> next = (node**)malloc(sizeof(node*)*(MAX_LEVELS+1));	//array of pointers to next node at each level. 

	for(int i = 0; i <= MAX_LEVELS; ++i)				
	{
		list -> header -> next[i] = list -> header;		//initially all levels are pointing to itself (empty skip list)
	}	
}

static int rand_level()
{
    int level = 0;
    while (rand() < RAND_MAX/2 && level < MAX_LEVELS)				//1/2 probability to increase level.
      	++level;
    return level;
}

void insert_node(sklist* list, int key)
{

	node* newnode_prev[MAX_LEVELS];									//array of pointers to nodes. These are those nodes which will point to the  node being inserted.
	node* p = list -> header;								

	for(int i = list->level; i >= 0; --i)							
	{
		while(p -> next[i] -> val < key)
			p = p -> next[i];
		newnode_prev[i] = p;
	}
	p = p -> next[0];												//now p is the node with val either past the value or equal to value

	if(key == p->val)												//duplicate
		return;

	int newnode_level = rand_level();								//level of node to be inserted			
	if(newnode_level > list -> level)								//if the level is greater than list's level, list's level needs to be updated 
	{																//and appropriate pointers set
		for(int i = list->level + 1; i <= newnode_level; ++i)
			newnode_prev[i] = list -> header;
		list -> level = newnode_level;
	}

	p = (node*) malloc(sizeof(node));								//p is the node to be inserted
	p -> val = key;
	p -> level = newnode_level;	
	p -> next = (node**)malloc(sizeof(node*)*(newnode_level+1));

	for(int i = 0; i <= newnode_level; ++i)							//stitch new node between
	{
		p -> next[i] = newnode_prev[i] -> next[i];
		newnode_prev[i] -> next[i] = p;
	}

}

void delete_node(sklist *list, int key)
{
	node* newnode_prev[MAX_LEVELS];
	node* p = list -> header;

	for(int i = list->level; i >= 0; --i)
	{
		while(p -> next[i] -> val < key)
			p = p -> next[i];
		newnode_prev[i] = p;
	}
	p = p -> next[0];											//till here same as insert

	if(key == p->val)
	{
		for(int i = 0; i <= list -> level; ++i)					//adjust links pointing to p
		{
			if(newnode_prev[i] -> next[i] != p) break;		
			newnode_prev[i] -> next[i] = p -> next[i];		
		}

		free(p -> next);			
		free(p);

		while(list -> level > 0 && list -> header -> next[list -> level] == list -> header)		//adjust the list's level
			--(list -> level);
	}
}


node *search(sklist *list, int key)
{
    node *p = list->header;
    
    for (int i = list->level; i >= 0; --i)
    {
        while (p->next[i]->val < key)
            p = p -> next[i];
    }
    if(p -> next[0] -> val == key)
    	return p -> next[0];
    return NULL;
}

void print(sklist *list)
{
    node *p = list->header;
    while (p && p->next[0] != list->header) {						//traverse base list and print the maximum level;
        printf("Value:%d, Highest Level:%d\n", p->next[0]->val, p -> next[0] -> level);
        p = p -> next[0];
    }
    printf("\n");
}

int main()
{
	srand(3);							//seed the random function

	sklist list;						//instantiate skip list
	init(&list);						//initialize header node. 

	int arr[] = {3,7,5,2,8,11,15};
	for(int i = 0; i < sizeof(arr)/sizeof(int); ++i)
		insert_node(&list, arr[i]);

	print(&list);

	delete_node(&list, 7);

	print(&list);

	node* res = search(&list, 8);
	if(!res)
		printf("Not found!\n");
	else
		printf("Found %d\n", res->val);



}