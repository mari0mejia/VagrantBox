#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void end(end)
{
  printf("2");
}
int main()
{
 if(fork() ==0)
 {
    atexit(end);
 }
 if(fork() ==0)
 {
   printf("0");
 }
 else
 {
  printf("1");
 }
 exit(0);
}
