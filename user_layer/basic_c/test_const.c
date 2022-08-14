#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	int a = 3;
	int b = 9;
	const int *p1 = &a;
	int *const p2 =&a;
	//a++;
	
	//(*p1)++;此句会报错，原因是const int *p1 意味着p1指向一个静态变量，
	//不能通过指针来改变自己指向的值
	printf("p1 pointer a, *p1=%d\n", *p1);

	(*p2)++;
	//p2 = &b; 此句报错，原因是int *const p2意味着p2指向一个int类型的
	//变量，但p2是一个静态指针，不能指向其他的地方。
	printf("p2 pointer a, *p2=%d\n", *p2);
	return 0;
}

