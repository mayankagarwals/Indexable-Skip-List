#include <iostream>
#include <algorithm>
#include "generic_skip_list.cpp"

using namespace std;

struct MyPair
{
	int first;
	int second;

	MyPair() {}
	MyPair(int first_, int second_) : first(first_), second(second_) {}

	bool operator<(const MyPair& rhs) const
	{
		return first < rhs.first;
	}

	bool operator==(const MyPair& rhs) const
	{
		return first == rhs.first && second == rhs.second;
	}

	friend ostream& operator<<(ostream& lhs, const MyPair& rhs)
	{
		return lhs << "first: " << rhs.first << " second: " << rhs.second;
	}
};

int main()
{
	srand(3);

#if 1
	SkipList<int> list(6);
	int arr[] = {3, 7, 5, 2, 9, 11, 4};
	for (int i = 0; i < sizeof(arr) / sizeof(int); ++i)
		list.insert_node(arr[i]);

	list.print();
	//list.back_link_test();

	// auto res = list[4];
	// cout << res << '\n';

	// cout << "\nPrinting with indices" << endl;
	// for (int i = 0; i < sizeof(arr)/sizeof(int); i++)
	// {
	// 	cout << list[i] << endl;
	// }
#endif
#if 1
//copy constructor test
	SkipList<int> l2(list);
	l2.print();
	// l2.back_link_test();

	cout << boolalpha << (list == l2) << '\n';

#endif
#if 0
//copy assignment test
	SkipList<int> l3 = list;
	l3.print();
	l3.back_link_test();
#endif

#if 0
	list.delete_node(7);

	list.print();
	// list.back_link_test();
#endif

#if 0
	SkipList<int>::iterator res1 = list.search(2);
	if (res1 == list.end())
		cout << "Not found!\n";
	else
		cout << "Found: " << *res1 << endl;
#endif

#if 0
	cout << "Iterator test" << endl;
	for (auto &i : list)
	{
		cout << "val: " << i << endl;
	}
	cout << endl;
	SkipList<int>::iterator i1;
	for (i1 = list.end(); i1 != list.begin(); --i1)
	{
		if (i1 != list.end())
		{
			cout << "val: " << (*i1) << endl;
		}
	}
	cout << (*i1);
#endif

	
#if 0
	// class test with pairs
	MyPair pairarr[] = {
		MyPair(3, 10000),
		MyPair(7, 45),
		MyPair(5, 55),
		MyPair(2, 88),
		MyPair(9, 99),
		MyPair(11, 33),
		MyPair(4, 22)
	};

	SkipList<MyPair> list_pair(6);
	for (auto &p : pairarr)
	{
		list_pair.insert_node(p);
	}
	
	cout << "\n\n";
	list_pair.print();

	cout << "\nPrinting with indices" << endl;
	for (int i = 0; i < sizeof(pairarr)/sizeof(MyPair); i++)
	{
		cout << list_pair[i] << endl;
	}

	for (auto &p : pairarr)
	{
		list_pair.delete_node(p);
	}

	cout << "\n\n";
	list_pair.print();

	for (auto &p : pairarr)
	{
		list_pair.insert_node(p);
	}
	
	cout << "\n\n";
	list_pair.print();

	cout << "\nPrinting with indices" << endl;
	for (int i = 0; i < sizeof(pairarr)/sizeof(MyPair); i++)
	{
		cout << list_pair[i] << endl;
	}
#endif

#if 0
	SkipList<int> list(6);
	int arr[] = {3, 7, 5, 2, 9, 11, 4};
	for (int i = 0; i < sizeof(arr) / sizeof(int); ++i)
		list.insert_node(arr[i]);

	auto p = find(list.begin(), list.end(), 11);
	if(p != list.end())
		cout << "Found: " << *p << endl;
	else 
		cout << "Not found" << endl;
	
	cout << list.size() << endl;
#endif
}