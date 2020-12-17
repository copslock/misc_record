
















#include <iostream>
using namespace std;
 
void checkCPU()
{
	union MyUnion{
		int a;
		char c;
	}test;
	test.a = 64;
	cout<<"test.a: "<<test.a<<endl;
	if (test.c == 64)
	  { //BARACKET NEEDED FOR MULTI OPERATIONS
	  cout<<"test.c: "<<test.c<<endl;
		cout << "little endian" <<endl;
		}
	else 
	  cout << "big endian" <<endl;
}
 
int main()
{
	checkCPU();
	return 0;
}



