#include<stdio.h>
#include<stdlib.h>

#define FREE_MEMORY(p) if(p){\
		free(p);\
		p = NULL;\
		}


int main(int argc ,char *argv[])
{
   char *ch = (char *)malloc(100);
   FREE_MEMORY(ch);
   if(ch == NULL)
   {

      printf("ch null\na");
   }

    return 0;
}
