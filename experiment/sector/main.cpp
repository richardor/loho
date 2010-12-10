#include<stdio.h>
#include<math.h>

int main()
{ 

    int m = 200;
    int n = 200;
    int a = 90;
    int x = 0;
    int y = 0;
    int R = 100;
    for(int i = 0; i <= 360; i+=45)
    {
       x = m + R*sin((i*(3.1415926))/180);
       y = n + R*cos((i*(3.1415926))/180);
       printf("i(%d),x(%d),y(%d)\n",i, x, y);

   }	
     
    return 0;   
}
