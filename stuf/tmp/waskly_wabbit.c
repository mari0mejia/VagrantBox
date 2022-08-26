#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

pid_t pid; //global pid
void bar(int sig)
{
  printf("wabbit\n");
  printf("pid %d signalled\n",pid);
  kill(pid,SIGUSR1); // sends kill to global process id(pid)
}
void baz(int sig)
{
  printf("twerrible\n");
}
void foo(int sig)
{
  printf("waskly\n");
  printf("pid %d signalled\n",pid);
  kill(pid,SIGUSR1); // sends kill to global process id(pid)
  exit(0);
}
int main()
{
 signal(SIGUSR1,baz);
 signal(SIGCHLD,bar);
 pid=fork();
 if(pid ==0)
 {
   signal(SIGUSR1, foo);
   kill(getppid(),SIGUSR1);//kills child proceess before infinite for loop
   printf("inside for\n");
   for(;;);
 }
 else
 {
   pid_t p; int status;
   if((p=wait(&status)) > 0)
   {
     printf("that\n");
   }
 }
 return 0;
}
