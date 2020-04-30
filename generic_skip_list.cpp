#if !defined(SKIPLIST_DEF)
#define SKIPLIST_DEF

#include <iostream>
#include <unordered_map>
#include <vector>

//T should have follwoing defined: ==, sizeof, <

template <typename T>
class SkipList
{

private:
	class Node
	{
	public:
		T val;
		int level;
		Node **next; //array of pointers to next node at each level.
		Node *prev;	 //level 0 pointer.

		int *widths; // array of widths per link

		explicit Node(int level_) : level(level_),
									 next(new Node *[level_ + 1]), widths(new int[level_ + 1]),
									 prev(nullptr)
		{
		}

		Node(const T &val_, int level_) : val(val_), level(level_),
										  next(new Node *[level_ + 1]), widths(new int[level_ + 1]),
										  prev(nullptr)
		{
		}

		~Node()
		{
			delete[] next;
			delete[] widths;
		}
	};

public:
	explicit SkipList(int max_level) : max_level_(max_level), _size(0)
	{
		level = 0; //we start off with just one level

		head = new Node(max_level_); //value of head node doesn't matter.

		for (int i = 0; i <= max_level_; ++i)
		{
			head->next[i] = head; //initially all levels are pointing to itself (empty skip list)
			head->widths[i] = 1;
		}

		head->prev = head; //level 0 back link.
	}

	~SkipList()
	{
		Node *p = head->next[0];
		Node *next;
		while (p != head)
		{
			next = p->next[0];
			delete p;
			p = next;
		}
		delete p; //free head
	}

	SkipList(const SkipList &rhs) : level(rhs.level), max_level_(rhs.max_level_), _size(rhs._size)
	{
		std::unordered_map<Node *, Node *> ht; //need a map for a mapping between original nodes and clone nodes
										  //The map is essential to correctly assign all links of node

		head = new Node(rhs.max_level_);



		Node *p_clone = head;
		Node *p = rhs.head;
		ht[rhs.head] = head;

		while (p->next[0] != rhs.head) //iterate creating new nodes and filling the map
		{
			p_clone->next[0] = new Node(p->next[0]->val, p->next[0]->level);
			ht[p->next[0]] = p_clone->next[0];
			p_clone = p_clone->next[0];
			p = p->next[0];
		}
		p_clone->next[0] = head; //last node should point to head.


		for(int i = 0; i <= max_level_; ++i)
		{
			head->widths[i] = rhs.head->widths[i];
			head->next[i] = ht[rhs.head->next[i]];
		}

		head->prev = ht[rhs.head->prev];
		p = rhs.head->next[0];
		p_clone = head->next[0];
		while (p != rhs.head) //do the same for all rest nodes
		{
			for (int i = p->level; i >= 0; --i) //correct all  links
			{
				if(i!= 0)
				p_clone->next[i] = ht[p->next[i]];
				p_clone->widths[i] = p->widths[i];
			}
			p_clone->prev = ht[p->prev];
			p = p->next[0];
			p_clone = p_clone->next[0];
		}
	}
	SkipList &operator=(const SkipList &rhs)
	{
		if (this != &rhs)
		{
			//delete the while skip list.
			Node *p = head->next[0];
			Node *next;
			while (p != head)
			{
				next = p->next[0];
				delete p;
				p = next;
			}
			delete p;

			std::unordered_map<Node *, Node *> ht; //need a map for a mapping between original nodes and clone nodes
			//The map is essential to correctly assign all links of node

			head = new Node(rhs.max_level_);
			Node *p_clone = head;
			p = rhs.head;
			ht[rhs.head] = head;

			while (p->next[0] != rhs.head) //iterate creating new nodes and filling the map
			{
				p_clone->next[0] = new Node(p->next[0]->val, p->next[0]->level);
				ht[p->next[0]] = p_clone->next[0];
				p_clone = p_clone->next[0];
				p = p->next[0];
			}
			p_clone->next[0] = head; //last node should point to head.

			for (int i = level; i > 0; --i) //correct all head links
			{
				head->next[i] = ht[rhs.head->next[i]];
				head->widths[i] = rhs.head->widths[i];
			}
			head->prev = ht[rhs.head->prev];
			p = rhs.head->next[0];
			p_clone = head->next[0];
			while (p != rhs.head) //do the same for all rest nodes
			{
				for (int i = p->level; i > 0; --i) //correct all head links
				{
					p_clone->next[i] = ht[p->next[i]];
					p_clone->widths[i] = p->widths[i];
				}
				p_clone->prev = ht[p->prev];
				p = p->next[0];
				p_clone = p_clone->next[0];
			}
		}
		return *this;
	}

	// bidirectional iterator
	class iterator
	{
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;

		explicit iterator(Node *p_it = nullptr) : p_it_(p_it) {}

		bool operator==(const iterator &rhs) const
		{
			return p_it_ == rhs.p_it_;
		}
		bool operator!=(const iterator &rhs) const
		{
			return !(*this == rhs);
		}

		const T &operator*()
		{
			return p_it_->val;
		}

		iterator &operator++()
		{
			p_it_ = p_it_->next[0];
			return *this;
		}
		iterator operator++(int)
		{
			iterator temp(*this);
			++*this;
			return temp;
		}

		iterator &operator--()
		{
			p_it_ = p_it_->prev;
			return *this;
		}
		iterator &operator--(int)
		{
			iterator temp(*this);
			--*this;
			return temp;
		}

	private:
		Node *p_it_;
	};

	iterator begin()
	{
		return iterator(head->next[0]);
	}
	iterator end()
	{
		return iterator(head);
	}

	const T &operator[](int index)
	{
		Node *p = head;
		index += 1;
		for (int i = level; i >= 0; --i)
		{
			while (p->widths[i] <= index)
			{
				index -= p->widths[i];
				p = p->next[i];
			}
		}
		return p->val;
	}

	bool operator==(const SkipList& rhs)
	{
		if(max_level_ != rhs.max_level_) return false;
		if(level != rhs.level) return false;
		if(_size != rhs._size) return false;

		Node* first_list = head;
		Node* second_list = rhs.head;

		do
		{
			if(first_list -> level != second_list -> level) return false;
			for(int i = 0; i <= first_list -> level; ++i)
			{
				if(first_list -> widths[i] != second_list -> widths[i]) return false;
				if(first_list -> next[i] -> val != second_list -> next[i] -> val) return false;
			}
			if(first_list -> prev -> val != second_list -> prev -> val) return false;

			first_list = first_list -> next[0];
			second_list = second_list -> next[0];
		}while(first_list != head && second_list != rhs.head);

		return true;

	}

	void insert_node(const T &key);
	void delete_node(const T &key);
	typename SkipList<T>::iterator search(const T &key);
	void back_link_test();
	void print();
	int size();

private:
	int rand_level()
	{
		int level = 0;
		while (rand() > RAND_MAX / 2 && level < max_level_) //1/2 probability to increase level., we keep level < max_level_ as condition so that even if level overflows it is a valid level.
			++level;
		return level;
	}

	Node *head;
	int level;
	int max_level_;
	int _size;
};

template <typename T>
void SkipList<T>::insert_node(const T &key)
{
	Node *newnode_prev[max_level_ + 1]; //array of pointers to nodes. These are those nodes which will point to the  node being inserted.
	Node *p = head;

	std::vector<int> steps(max_level_ + 1);		//max_level_ needed as later widths can be upto newnode_level and no way of knowing that value beforehand
	for (int i = level; i >= 0; --i)
	{
		while ((p->next[i]->val < key) && (p->next[i] != head))
		{
			p = p->next[i];
			steps[i] += 1;
		}
		newnode_prev[i] = p;
	}

	p = p->next[0]; //now p is the node with val either past the value or equal to value

	if (key == p->val) //duplicate
		return;

	int newnode_level = rand_level(); //level of node to be inserted
	if (newnode_level > level)		  //if the level is greater than list's level, list's level needs to be updated
	{								  //and appropriate pointers set
		for (int i = level + 1; i <= newnode_level; ++i)
			newnode_prev[i] = head;
		level = newnode_level;
	}

	std::vector<int> widths(newnode_level + 1, 1);
	for (int i = 1; i <= newnode_level; ++i)
	{
		widths[i] = steps[i - 1] + widths[i - 1];
	}

	p = new Node(key, newnode_level); //p is the node to be inserted
	p->widths[0] = 1;				  //base level widths is 1.

	newnode_prev[0]->next[0]->prev = p; //stiching back link.
	p->prev = newnode_prev[0];			//back link for created node


	std::cout << '\n';

	for (int i = 0; i <= newnode_level; ++i) //stitch new node between
	{
		p->next[i] = newnode_prev[i]->next[i];
		newnode_prev[i]->next[i] = p;
		p->widths[i] = newnode_prev[i]->widths[i] - widths[i] + 1;
		newnode_prev[i]->widths[i] = widths[i];
	}

	if (newnode_level < level)
	{
		for (int i = newnode_level + 1; i <= level; ++i)
			newnode_prev[i]->widths[i] += 1;
	}

	for (int i = level + 1; i <= max_level_; ++i)
		head->widths[i] += 1;

	// std::cout << "Inserting : " << key << '\n';
	// print();
	++_size;

}

template <typename T>
void SkipList<T>::delete_node(const T &key)
{
	Node *newnode_prev[max_level_ + 1];
	Node *p = head;

	for (int i = level; i >= 0; --i)
	{
		while ((p->next[i]->val < key) && (p->next[i] != head))
			p = p->next[i];
		newnode_prev[i] = p;
	}
	p = p->next[0]; //till here same as insert

	if (key == p->val)
	{
		p->next[0]->prev = p->prev;		 //stiching back link.
		for (int i = 0; i <= level; ++i) //adjust links pointing to p
		{
			if (newnode_prev[i]->next[i] != p)
				break;
			newnode_prev[i]->next[i] = p->next[i];
		}



		for(int i = 0; i <= p->level; ++i)
			newnode_prev[i] -> widths[i] += p->widths[i] - 1; //adjust widths
		for(int i = p->level+1; i <= level; ++i)
			newnode_prev[i] -> widths[i] -= 1;
		for(int i = level+1; i <= max_level_; ++i)
			head -> widths[i] -= 1;


		delete p;
		--_size;

		while (level > 0 && head->next[level] == head) //adjust the list's level
			--level;
	// std::cout << "Deleting : " << key << '\n';
	// print();
	}
}

template <typename T>
typename SkipList<T>::iterator SkipList<T>::search(const T &key)
{
	Node *p = head;

	for (int i = level; i >= 0; --i)
	{
		while ((p->next[i]->val < key) && (p->next[i] != head))
			p = p->next[i];
	}
	if (p->next[0]->val == key)
		return iterator(p->next[0]);
	return end();
}

template <typename T>
int SkipList<T>::size()
{
	return _size;
}

template <typename T>
void SkipList<T>::print()
{
	Node *p = head;
	std::cout << "Widths: ";
	for(int i = 0; i <= p->level; ++i)
		std::cout << p -> widths[i];
	std::cout << '\n';
	while (p->next[0] != head)
	{ //traverse base list and print the maximum level;
		std::cout << "Value: " << p->next[0]->val << ", Highest Level: " << p->next[0]->level << ' ';
		std::cout << "Widths: ";
		for(int i = 0; i <= p->next[0]->level; ++i)
			std::cout << p -> next[0] -> widths[i] << ' ';
		std::cout << '\n';
		p = p->next[0];
	}
	std::cout << '\n';
}

template <typename T>
void SkipList<T>::back_link_test()
{
	Node *p = head->prev;
	while (p != head)
	{
		std::cout << p->val << ' ';
		p = p->prev;
	}
	std::cout << "\nOutput should be decreasing order\n";
}

#endif // SKIPLIST_DEF
