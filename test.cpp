#include <iostream>

using namespace std;

int main()
{
	for( int k = 0; k < 5; )
		cout << "Post-increment k = " << k++ << endl;

	for( int k = 0; k < 5; )
		cout << "Pre-increment k = " << ++k << endl;

}
