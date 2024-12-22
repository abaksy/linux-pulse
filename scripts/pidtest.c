#include<unistd.h>

int main()
{
	for(int i = 0; i<10000; ++i)
		getpid();
	return 0;
}
