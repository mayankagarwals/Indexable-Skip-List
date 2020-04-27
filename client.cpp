#include <iostream>
#include "generic_skip_list.cpp"

using namespace std;

int main()
{
	srand(3);
	SkipList<int> list(6);
	int arr[] = {3, 7, 5, 2, 9, 11, 4};
	for (int i = 0; i < sizeof(arr) / sizeof(int); ++i)
		list.insert_node(arr[i]);

	list.print();
	//list.back_link_test();

	auto res = list[4];
	cout << res << '\n';

#if 0
//copy constructor test
	SkipList<int> l2(list);
	l2.print();
	l2.back_link_test();

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
	SkipList<int>::iterator res = list.search(2);
	if (res == list.end())
		cout << "Not found!\n";
	else
		cout << "Found: " << *res << endl;
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
}