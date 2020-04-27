# should actually generate object files but this is also fine
a.out: generic_skip_list.cpp client.cpp
	g++ client.cpp