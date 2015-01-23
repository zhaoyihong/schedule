
#include <cstring>
#include <iostream>
#include <cassert>
using namespace  std;

void memecpy(const void *src,void *dest,int len)
{
	assert(src!=NULL && dest !=NULL && len!=0);
	
	const char *s = static_cast<const char *>(src);
	char * d = static_cast<char *>(dest);
	if( src < dest)
	{
		cout << "1" << endl;
	}
	else
	{
		cout << "2" << endl;;
	}

	cout << dest << endl;
	cout << src << endl;
	

}


int main()
{
	void *p1 = (void *)0x00ff;
	void *p2 = (void *)0x01100;
	memcpy(p1,p2,10);
	

	return 0;
}

