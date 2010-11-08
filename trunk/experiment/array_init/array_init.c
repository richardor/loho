#include<stdio.h>

static void test_array_no_init()
{
	int array[10][10];
	int i,j;

	for(i = 0; i < 10; i++)
	{
		for(j = 0; j < 10; j++)
		{
			printf("i(%d), j(%d), value(%d)\n", i, j, array[i][j]);
		}
	}

}

static void test_array_init()
{
	int array[10][10] = {0};
	int i,j;

	for(i = 0; i < 10; i++)
	{
		for(j = 0; j < 10; j++)
		{
			printf("i(%d), j(%d), value(%d)\n", i, j, array[i][j]);
		}
	}
}

int main(int argc ,char *argv)
{

	printf("test array init\n");
	test_array_init();

	printf("test array no  init\n");
	test_array_no_init();
	return 0;

}
