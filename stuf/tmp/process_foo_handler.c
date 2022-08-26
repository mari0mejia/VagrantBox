#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void foo(int sig)
{
 printf("SC"); 
}
int main()
{
 signal(SIGCHLD,foo);
 int pid;
 pid = fork();
 sleep(1);
 if(pid == 0)
 {
   printf("HC");
 }
 else
 {
   pid_t p; int status;
   printf("HP");
   if((p=wait(&status)) > 0)
   {
     printf("CR");
   }
 }
}
