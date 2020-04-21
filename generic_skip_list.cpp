
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

class Node
{
public:
	int val;
	int level;
	Node** next;  //array of pointers to nodes.
};

class SkipList
{
public:
	explicit SkipList(int max_level) : max_level_(max_level) 
	{
		head = (Node*)malloc(sizeof(Node));		//header node
		level = 0;									//we start off with just one level
		head-> val = INT_MAX;
		head -> next = (Node**)malloc(sizeof(Node*)*(max_level_+1));	//array of pointers to next node at each level. 

		for(int i = 0; i <= max_level_; ++i)				
		{
			head -> next[i] = head;		//initially all levels are pointing to itself (empty skip list)
		}	

		srand(3);
	}

	~SkipList()
	{
		Node* p = head->next[0];
		Node* next;
		while(p != head)
		{
			next = p -> next[0];
			free(p);
			p = next;
		}
		free(p);
	}

	SkipList(const SkipList&) = delete;					//TODO: Implement Copy constructor
	SkipList& operator= (const SkipList&) = delete;		//TODO: Implement Copy Assignment

	void insert_node(int key);
	void delete_node(int key);
	Node* search(int key);
	void print();

private:

	int rand_level()
	{
		int level = 0;
		while (rand() < RAND_MAX/2 && level < max_level_)				//1/2 probability to increase level.
			++level;
		return level;
	}

	Node* head;
	int level;
	int max_level_;

};

void SkipList::insert_node(int key)
{
	Node* newnode_prev[max_level_];									//array of pointers to nodes. These are those nodes which will point to the  node being inserted.
	Node* p = head;								

	for(int i = level; i >= 0; --i)							
	{
		while(p -> next[i] -> val < key)
			p = p -> next[i];
		newnode_prev[i] = p;
	}
	p = p -> next[0];												//now p is the node with val either past the value or equal to value

	if(key == p->val)												//duplicate
		return;

	int newnode_level = rand_level();								//level of node to be inserted			
	if(newnode_level > level)								//if the level is greater than list's level, list's level needs to be updated 
	{																//and appropriate pointers set
		for(int i = level + 1; i <= newnode_level; ++i)
			newnode_prev[i] = head;
		level = newnode_level;
	}

	p = (Node*) malloc(sizeof(Node));								//p is the node to be inserted
	p -> val = key;
	p -> level = newnode_level;	
	p -> next = (Node**)malloc(sizeof(Node*)*(newnode_level+1));

	for(int i = 0; i <= newnode_level; ++i)							//stitch new node between
	{
		p -> next[i] = newnode_prev[i] -> next[i];
		newnode_prev[i] -> next[i] = p;
	}

}

void SkipList::delete_node(int key)
{
	Node* newnode_prev[max_level_];
	Node* p = head;

	for(int i = level; i >= 0; --i)
	{
		while(p -> next[i] -> val < key)
			p = p -> next[i];
		newnode_prev[i] = p;
	}
	p = p -> next[0];											//till here same as insert

	if(key == p->val)
	{
		for(int i = 0; i <= level; ++i)					//adjust links pointing to p
		{
			if(newnode_prev[i] -> next[i] != p) break;		
			newnode_prev[i] -> next[i] = p -> next[i];		
		}

		free(p -> next);			
		free(p);

		while(level > 0 && head -> next[level] == head)		//adjust the list's level
			--level;
	}
}

Node* SkipList::search(int key)
{
    Node *p = head;
    
    for (int i = level; i >= 0; --i)
    {
        while (p->next[i]->val < key)
            p = p -> next[i];
    }
    if(p -> next[0] -> val == key)
    	return p -> next[0];
    return NULL;
}

void SkipList::print()
{
    Node *p = head;
    while (p && p->next[0] != head) {						//traverse base list and print the maximum level;
        printf("Value:%d, Highest Level:%d\n", p->next[0]->val, p -> next[0] -> level);
        p = p -> next[0];
    }
    printf("\n");
}

int main()
{
	SkipList list(5);
	int arr[] = {3,7,5,2,8,11,15};
	for(int i = 0; i < sizeof(arr)/sizeof(int); ++i)
		list.insert_node(arr[i]);

	list.print();

	list.delete_node(7);

	list.print();

	Node* res = list.search(8);
	if(!res)
		printf("Not found!\n");
	else
		printf("Found %d\n", res->val);

	
}