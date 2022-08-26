#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int val = 10;
void handler(int sig)
{
  val +=5;
  return;
}
int main()
{
 int pid;
 signal(SIGCHLD,handler);
 if((pid=fork()) ==0)
 {
    val-=3;
    printf("in the child\n");
    printf("val=%d\n",val);
    exit(0);
 }
 sleep(1);
 waitpid(pid,NULL,0);//waits on 
 printf("val=%d\n",val);
 exit(0);
}
